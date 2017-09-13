
//cci_daemon.cpp    william k. johnson 2017
#include <cci_daemon.h>
#include <stack>

//namespace
using namespace cdi;
using namespace cci_daemonize;

//immutable
const unsigned cci_daemon_facade::buffer_size = 1024;
const unsigned cci_daemon_facade::cpf_cloexec = 1;
const std::string cci_daemon_facade::path_config = "/etc/chromatic-universe/cci-daemon.conf";
const std::string cci_daemon_facade::log_path = "/var/log/cci-daemon/cci-daemon.log";
const std::string cci_daemon_facade::path_pid = "/var/run/cci-daemon.pid";


//--------------------------------------------------------------------------------------
cci_daemon_intf::~cci_daemon_intf()
{
    //
}

//--------------------------------------------------------------------------------------
cci_daemon_facade::cci_daemon_facade( int argc , char* argv[] ) : m_dw_flags { 0L } ,
                                                                  m_log_fp { nullptr } ,
                                                                  m_b_opened { false } ,
                                                                  m_service_proc { service_proc::sp_default_service } ,
                                                                  m_str_conf { cci_daemon_facade::path_config } ,
                                                                  m_str_pid_path { cci_daemon_facade::path_pid } ,
                                                                  m_backtrace { false } ,
                                                                  m_argc{ argc } ,
                                                                  m_argv { argv } ,
                                                                  m_ptr_kernel_context( new kernel_context ) ,
                                                                  m_chroot_dir { "/" }
{
      //
}

//---------------------------------------------------------------------------------------------------
cci_daemon_facade::~cci_daemon_facade()
{
        _bt();

        if( context() )
        {
            if( context()->kernel_ref )
            {
                try
                {
                    this->context()->unmake_kernel( context() );
                    log_message( "...kernel unloaded...." );
                }
                catch( ... )
                {  log_message( "...error in unloading kernel...." ); }
            }
        }

}

//---------------------------------------------------------------------------------------------------
int cci_daemon_facade::daemonize()
{
 
    return ( daemonize_( flags() ) );
}

//---------------------------------------------------------------------------------------------------
bool cci_daemon_facade::log_message( const std::string& msg )
{
     bool b_ret { false };
     if( m_log_fp )
     {

            const char *TIMESTAMP_FMT = "%F %X";        //YYYY-MM-DD HH:MM:SS
        #define TS_BUF_SIZE sizeof("YYYY-MM-DD HH:MM:SS")//includes '\0'
            char timestamp[TS_BUF_SIZE];
            time_t t;
            struct tm *loc;

            t = time(NULL);
            loc = localtime(&t);
            if (loc == NULL ||
                   strftime(timestamp, TS_BUF_SIZE, TIMESTAMP_FMT, loc) == 0)
                fprintf(m_log_fp, "???Unknown time????: ");
            else
                fprintf(m_log_fp, "%s: ", timestamp);

            fprintf ( m_log_fp , "%s" , (char*) msg.c_str() );
            fprintf ( m_log_fp , "%s" , "\n" );

            b_ret = true;
     }

     _bt();

     return b_ret;
}


//---------------------------------------------------------------------------------------------------
bool cci_daemon_facade::open_log()
{
        mode_t m;
        bool b_ret { true };

        m = umask(077);
        if( access( (char*) cci_daemon_facade::log_path.c_str() , R_OK | W_OK ) == -1 )
        {
            fprintf ( stderr , "cannot access syslog..not enough permissions...are you root?\n" );
            exit( 1 );
        }
        m_log_fp = fopen((char*) cci_daemon_facade::log_path.c_str() , "a");
        umask( m );

        if ( m_log_fp == NULL )
           b_ret = false;

        //disable stdio buffering
        setbuf( m_log_fp, NULL );

        return b_ret;

}

//---------------------------------------------------------------------------------------------------
void cci_daemon_facade::close_log()
{
    if( m_log_fp ) { fclose( m_log_fp ); }
}

//---------------------------------------------------------------------------------------------------
bool cci_daemon_facade::read_config_file( const std::string& config_path )
{
        file_ptr fp;
        char str[cci_daemon_facade::buffer_size];

        bool b_ret = false;

        fp = fopen( (char*) config_path.c_str() , "r" );
        if ( fp != NULL )
        {

            //ignore nonexistent file
            if ( fgets( str, cci_daemon_facade::buffer_size , fp) == NULL )
            {
                str[0] = '\0';
            }
            else
            {
                str[strlen(str) - 1] = '\0';
            }
            //strip trailing '\n'
            std::ostringstream ostr;
            ostr << "read config file: " << str << std::ends;
            b_ret = log_message( ostr.str() );

            fclose( fp );
        }

        _bt();

        return b_ret;

}

//---------------------------------------------------------------------------------------------------
int cci_daemon_facade::lock_region( int fd ,
                                    int type ,
                                    int whence ,
                                    int start ,
                                    int len )
{
        struct flock fl;

        fl.l_type = type;
        fl.l_whence = whence;
        fl.l_start = start;
        fl.l_len = len;

        return fcntl( fd , F_SETLK , &fl );

}

//---------------------------------------------------------------------------------------------------
bool cci_daemon_facade::write_pid( const std::string& pid_file ,
                                   int flags  )
{
        int fd;
        char buffer[cci_daemon_facade::buffer_size];
        bool b_ret { false };

        fd = open( (char*) pid_file.c_str() , O_RDWR | O_CREAT , S_IRUSR | S_IWUSR );
        if( fd == -1 )
        {  log_message( "could not write pid file...." );  return false; }

        if ( flags & cci_daemon_facade::cpf_cloexec )
        {

            //close-on-exec file descriptor flag
            flags = fcntl( fd , F_GETFD );
            //fetch flags
            if( flags == -1 )
            { log_message( "could not get flags for pid file" ); return false; }

            flags |= FD_CLOEXEC;
            //turn on FD_CLOEXEC
             if ( fcntl( fd , F_SETFD , flags ) == -1 )
             { log_message( "could not set flags for pid file" ); return false; }
        }

        if ( lock_region( fd , F_WRLCK , SEEK_SET , 0 , 0 ) == -1 )
        {

            if ( errno == EAGAIN || errno == EACCES )
            { log_message( "pid file is locked; probably daemon is already running" ); }
            else { log_message( "unable to lock pid file" );  }
        }
        else
        {
            if ( ftruncate( fd , 0 ) == -1 ) { log_message( "could not truncate pid file" ); }
            else  { b_ret = true; }
        }

        //write pid
        if( b_ret )
        {
            log_message( "writing pid file...." );
            snprintf( buffer ,
                      cci_daemon_facade::buffer_size ,
                      "%ld\n" ,
                      (long) getpid() );
            if ( write( fd , buffer , strlen( buffer ) ) != strlen( buffer ) )
            {
                log_message( "writing to pid file failed" );
                b_ret = false;
            }
        }

        _bt();

        return b_ret;
}

//---------------------------------------------------------------------------------------------------
bool cci_daemon_facade::remove_pid( const std::string& pid_file )
{

        _bt();

        return std::remove( pid_file.c_str() ) ? true : false;
}


//---------------------------------------------------------------------------------------------------
int cci_daemon_facade::daemon_default_exec( const std::string& params , const unsigned long dw_flags )
 {
          int options = 0;
          options |= LOG_PID;
          const char* identity = "cci-daemon";

          //daemonize has closed all file handles , reopen log file if closed
          if( m_b_opened == false )
          {
              m_b_opened = open_log();
              if( m_b_opened == true )
              {
                std::string msg( "cci_daemon.....using default module.....william k. johnson 2017" );
                log_message( msg );
                read_config_file( config_path() );

                syslog ( LOG_USER | LOG_INFO | LOG_PID , "%s", "activating default cci-daemon procedure...." );

              }
          }

          const int sleep_time = 15;
          int unslept = sleep_time;
          struct sigaction sa ;

          //handle sighup
          sigemptyset( &sa.sa_mask );
          sa.sa_flags = SA_RESTART;
          sa.sa_handler = sighup_handler;
          if( sigaction( SIGHUP , &sa , nullptr ) == -1 )  { cdi::chromatic_terminate(); }
          if( signal( SIGTERM , sigterm_handler ) == SIG_ERR )  { cdi::chromatic_terminate(); }


          if ( ! write_pid( pid_path() , 0 ) )
          { log_message( "pid write failed"  ); }

          for( ;; )
          {

              //returns 0 if interrupted
              unslept = sleep( unslept );

              if( m_b_opened == true )
              {
                  if( cdi::hup_received )
                  {
                      //
                      openlog( identity , options , LOG_USER );
                      syslog ( LOG_USER | LOG_INFO , "%s", "cci-daemon received sighup....reading config....continuing" );
                      closelog();
                      //next sighup
                      cdi::hup_received = 0;
                      close_log();
                      open_log();
                      read_config_file( config_path() );
                  }
                  if( unslept == 0 )
                  {
                     unslept = sleep_time;
                     std::string msg( "working hard....." );
                     log_message( msg );

                  }

              }
          }

          remove_pid( pid_path() );

          return ( 0 );
}


//--------------------------------------------------------------------------------
lib_handle_t cci_daemon_facade::load_lib( const std::string& lib )
{

            std::ostringstream ostr;
            ostr << "lib"
                 << lib
                 << ".so"
                 << std::ends;
            std::string path_with_extension = ostr.str();
            lib_handle_t shared_object = ::dlopen( path_with_extension.c_str() ,
                                                   RTLD_NOW) ;
            if( shared_object == nullptr )
            {
                throw std::runtime_error( std::string( "could not load '")
                                          + path_with_extension + "'"
                                          + "..."
                                          + dlerror() );
            }

            std::cerr << "...loaded library...."
                      << path_with_extension
                      << "...\n";




            return shared_object;
}

//---------------------------------------------------------------------------------------------------
void cci_daemon_facade::bootstrap_coordinator()
{
          int options = 0;
          options |= LOG_PID;
          const char* identity = "cci-daemon";


          if( ! m_b_opened )
          {
              m_b_opened = open_log();
              if( m_b_opened == true )
              {
                std::string msg( "cci_daemon.....bootstrapping default coordinator....william k. johnson 2017" );
                log_message( msg );
                read_config_file( config_path() );
                //read config closed the log , reopen it
                if( m_b_opened == false )
                {
                  m_b_opened = open_log();
                }
                log_message( msg );
              }
          }

          //load bootstrap lib and kernel explicitly
          //we're going through these gyrations
          //for  a good cause: isolate the daemon
          //proper from dependencies of the plugins
          //dynamic procs , and kernel

          //library monikers
          std::string bootstrap { "cci_daemon_bootstrap" };
          std::string kernel { "cci_daemon_kernel" };
          //function monikers
          std::string bootstrap_str { "bootstrap_default_coordinator" };


          std::string kernel_str { "make_kernel" };
          std::string remove_kernel_str( "unmake_kernel" );
          std::string mount_memory_cache_str( "mount_memory_cache" );
;

          try
          {
              //load the kernel
              //load library  - this will throw if the load fails
              log_message( "...loading kernel" );
              context()->lib_ref = load_lib( kernel );
              assert(  context()->lib_ref  );


              //map the kernel stack
              //we just have a library pointer
              //we have to query for symbosls by design
              map_kernel();


              //load library  - this will throw if the load fails
              auto bootstrap_lib = load_lib( bootstrap );
              assert( bootstrap_lib );
              //assign the library address to our member attribute
              bootstrap_function_address = get_function_pointer<bootstrap_function>
                          ( bootstrap_lib ,bootstrap_str );
              if( bootstrap_function_address == nullptr )
              {  log_message( "could not retrieve bootstrap function addresss" ); }
              else
              //call our dynamic library function
              {
                    //for debugging library symbols
                    log_message( "...bootstrapping cooordinator..." );
                    int dw = this->bootstrap( this->m_argc , this->m_argv , (void*) this );
                    if( dw == 0 )
                    {

                    }
              }

          }
          catch( std::runtime_error& err )
          {
              log_message( err.what() );

              exit( 1 );
          }

}

//---------------------------------------------------------------------------------------------------
void cci_daemon_facade::map_kernel()
{
          //this is also a desiged entry point for mocks and tests
          //among other things the kernel is only
          //am array of function pointers
          //to the application level  all deletions and allocations
          //are made by the shared library , so the pointers
          //( and the kernel itself) can be removed or replaced in real time
          //also note , nothing is is operating as a priviliged
          //account. except for a todo=workaround using shell fto
          //mount the cache. plans were to have the ccifs mounted from
          //the service maanger , but this could not be made to work.
          //pontificated with the idea of using some objdump/readelf
          //hijinks to do this mapping transparently but talked myself
          //out of it
          std::string mk { "make_kernel" };
          if( context()->lib_ref )
          {

                context()->pm.clear();
                context()->make_kernel = get_function_pointer<call_kernel_function>
                              ( context()->lib_ref , mk );
                //runtime error , mismatched shared libs
                if( ! context()->make_kernel )
                { throw std::runtime_error( "...could not retrieve function addres=>:make_kernel..." ); }                                    //call
                context()->make_kernel( context() );
                if( ! context()->kernel_ref )
                { throw std::runtime_error( "...kernel object instantiation failed..." ); }
                //all other servcie swill be loaded into
                //this address space. a global
                //kernel pointer is not made available  here
                //but other objects will have to populate
                //their own kernel structures for their own
                //ends because there is only runtime linkage
                //we're essentially trying for a python like
                //unbound object arena. the kernel in any library
                //is a singleton , meaning different kernels can loaded
                //but only one of each. since the functions are unbound,
                //this is the reason for the context structure, which may
                //appear soemwhat recursive , but is still more
                //straigjforward than metaprogramming , which becomes
                //bynzantine complex with memory allocations out of libraries
                //and trying to swap objects in andout of memory, The C
                //calling convention allows us the simplicity of justt using a context
                //structure , while inside the library itself we can do
                //whatever c++ magic we wish , since the library is
                //doing all the allocations
                //
                //the only interface promise the kernel advertises is that a
                //advertiseed kernel string well result in a callable function
                //poiner.

                //map rest of stack
                context()->pm.clear();
                context()->mount_memory_cache = get_function_pointer<call_kernel_function>
                              ( context()->lib_ref , "mount_memory_cache" );
                k_( context()->mount_memory_cache );
                context()->pm.clear();
                context()->unmake_kernel = get_function_pointer<call_kernel_function>
                              ( context()->lib_ref , "unmake_kernel" );
                k_( context()->unmake_kernel );
                context()->pm.clear();
                context()->unmount_memory_cache = get_function_pointer<call_kernel_function>
                              ( context()->lib_ref , "unmount_memory_cache" );
                k_( context()->unmount_memory_cache );


          }
          else
          {
              throw std::runtime_error( "...kernel mapping failed...library reference invalid..." );
          }
          log_message( "...mapped_kernel...." );

}



