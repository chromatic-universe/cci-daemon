
//cci_daemon.cpp    william k. johnson 2017
#include <cci_daemon.h>
#include <cci_daemon_kernel.h>
//namespace
using namespace cdi;

//immutable
const unsigned cci_daemon_facade::bd_no_chdir = 01;
const unsigned cci_daemon_facade::bd_no_close_files = 02;
const unsigned cci_daemon_facade::bd_no_reopen_std_fds = 04;
const unsigned cci_daemon_facade::bd_no_umask_0 = 010;
const unsigned cci_daemon_facade::bd_max_handles = 8192;
const unsigned cci_daemon_facade::buffer_size = 1024;
const unsigned cci_daemon_facade::cpf_cloexec = 1;
const std::string cci_daemon_facade::path_config = "/etc/cci-daemon/cci-daemon.conf";
const std::string cci_daemon_facade::log_path = "/var/log/cci-daemon/cci-daemon.log";
const std::string cci_daemon_facade::path_pid = "/var/run/cci-daemon.pid";


//--------------------------------------------------------------------------------------
cci_daemon_facade::cci_daemon_facade()  : m_dw_flags { 0L } ,
                                          m_log_fp { nullptr } ,
                                          m_b_opened { false } ,
                                          m_service_proc { service_proc::sp_default_service } ,
                                          m_str_conf { cci_daemon_facade::path_config } ,
                                          m_str_pid_path { cci_daemon_facade::path_pid } ,
                                          m_backtrace { false }
{
      //
}

//---------------------------------------------------------------------------------------------------
cci_daemon_facade::~cci_daemon_facade()
{
    _bt();
}

//---------------------------------------------------------------------------------------------------
int cci_daemon_facade::daemonize()
{
    int maxfd , fd;

    daemon_proc dp = daemon_proc::dp_fork_background_proc;
    while( dp != daemon_proc::dp_error )
    {
       switch( dp )
       {
            case daemon_proc::dp_fork_background_proc :
            {

                //become background process
                switch( fork() )
                {
                    case -1 :
                        dp = daemon_proc::dp_error;
                        break;
                    case  0 :
                        dp = daemon_proc::dp_make_session_leader;
                        break;
                    default:
                        _exit( EXIT_SUCCESS );
                }
                break;
            }
            case daemon_proc::dp_make_session_leader :
            {
                //become leader of new session
                setsid() == -1 ? dp = daemon_proc::dp_error : dp = daemon_proc::dp_fork_no_session_leader;
                break;
            }
            case daemon_proc::dp_fork_no_session_leader :
            {
                //ensure we are not session leader
                switch( fork() )
                {
                    case -1 :
                        dp = daemon_proc::dp_error;
                        break;
                    case  0 :
                        dp = daemon_proc::dp_clear_file_create_mask;
                        break;
                    default:
                        _exit( EXIT_SUCCESS );
                }
                break;
            }
            case daemon_proc::dp_clear_file_create_mask :
            {
                //clear file creation mask
                if( !( flags() & cci_daemon_facade::bd_no_umask_0 ) ) { umask( 0 ); }
                dp =  daemon_proc::dp_change_to_root_dir;

                break;
            }
            case daemon_proc::dp_change_to_root_dir :
            {
                 //change to root directory
                 if( !( flags() & cci_daemon_facade::cci_daemon_facade::bd_no_chdir ) ) { int ret = chdir( "/" ); }
                 dp = daemon_proc::dp_close_all_open_files;

                 break;
            }
            case daemon_proc::dp_close_all_open_files :
            {
                //close all open files
                if( !( flags() & cci_daemon_facade::cci_daemon_facade::bd_no_close_files ) )
                {
                     maxfd = sysconf( _SC_OPEN_MAX );
                     //if limit is indeterminate , guess
                     if( maxfd == -1 ) { maxfd = cci_daemon_facade::bd_max_handles; }
                     for( fd = 0; fd < maxfd; fd++ ) { close( fd ); }

                     dp = daemon_proc::dp_reopen_streams_dev_null;

                     break;
                }
                break;
            }
            case daemon_proc::dp_reopen_streams_dev_null :
            {
                 //reopen standard streams /dev/null
                 if( !( flags() & cci_daemon_facade::bd_no_reopen_std_fds ) )
                 {
                     close( STDIN_FILENO );

                     fd = open( "/dev/null" , O_RDWR );
                     //fd shoudl be 0
                     if( fd != STDIN_FILENO ) { dp = daemon_proc::dp_error; break; }
                     if( dup2( STDIN_FILENO , STDOUT_FILENO ) != STDOUT_FILENO ) { dp = daemon_proc::dp_error; break; }
                     if( dup2( STDIN_FILENO , STDERR_FILENO ) != STDERR_FILENO ) { dp = daemon_proc::dp_error; break; }

                 }
                 dp = daemon_proc::dp_daemonized;
                 break;
            }
            case daemon_proc::dp_daemonized :
            case daemon_proc::dp_error :
            {
            	//
            }
            default :
            {
                break;
            }
       }
       if ( dp == daemon_proc::dp_daemonized ) { break; }
    }


    return ( dp != daemon_proc::dp_error ? 0 : -1 );
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
                std::string msg( "cci_daemon.....using default module.....william k. johnson 2016" );
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
                      //sighup
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

//---------------------------------------------------------------------------------------------------
void cci_daemon_facade::bootstrap_coordinator()
{
            //
}
