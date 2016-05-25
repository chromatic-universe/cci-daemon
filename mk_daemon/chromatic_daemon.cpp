#include <chromatic_daemon.h>

//namespace
using namespace cdi;

//immutable
const unsigned long daemon_facade::bd_no_chdir = 01;
const unsigned long daemon_facade::bd_no_close_files = 02;
const unsigned long daemon_facade::bd_no_reopen_std_fds = 04;
const unsigned long daemon_facade::bd_no_umask_0 = 010;
const unsigned long daemon_facade::bd_max_handles = 8192;
const std::string daemon_facade::log_path = "/var/log/chromatic-daemon/chromatic-daemon.log";


//--------------------------------------------------------------------------------------
daemon_facade::daemon_facade()  : m_dw_flags( 0L ) ,
                                  m_b_opened( false ) ,
                                  m_service_proc( service_proc::sp_default_service )
{

}

//---------------------------------------------------------------------------------------------------
daemon_facade::~daemon_facade()
{

}

//---------------------------------------------------------------------------------------------------
int daemon_facade::daemonize()
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
                if( !( flags() & daemon_facade::bd_no_umask_0 ) ) { umask( 0 ); }
                dp =  daemon_proc::dp_change_to_root_dir;

                break;
            }
            case daemon_proc::dp_change_to_root_dir :
            {
                 //change to root directory
                 if( !( flags() & daemon_facade::daemon_facade::bd_no_chdir ) ) { int ret = chdir( "/" ); }
                 dp = daemon_proc::dp_close_all_open_files;

                 break;
            }
            case daemon_proc::dp_close_all_open_files :
            {
                //close all open files
                if( !( flags() & daemon_facade::daemon_facade::bd_no_close_files ) )
                {
                     maxfd = sysconf( _SC_OPEN_MAX );
                     //if limit is indertimante , guess
                     if( maxfd == -1 ) { maxfd = daemon_facade::bd_max_handles; }
                     for( fd = 0; fd < maxfd; fd++ ) { close( fd ); }

                     dp = daemon_proc::dp_reopen_streams_dev_null;

                     break;
                }
                break;
            }
            case daemon_proc::dp_reopen_streams_dev_null :
            {
                 //reopen standard streams /dev/null
                 if( !( flags() & daemon_facade::bd_no_reopen_std_fds ) )
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
bool daemon_facade::log_message( const std::string& msg )
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

     return b_ret;
}

//---------------------------------------------------------------------------------------------------
bool daemon_facade::open_log()
{
    mode_t m;
    bool b_ret { true };

    m = umask(077);
    m_log_fp = fopen((char*) daemon_facade::log_path.c_str() , "a");
    umask( m );

    if ( m_log_fp == NULL )
       b_ret = false;

    //disable stdio buffering
    setbuf( m_log_fp, NULL );

    return b_ret;

}

//---------------------------------------------------------------------------------------------------
void daemon_facade::close_log()
{
    fclose( m_log_fp );
}

//---------------------------------------------------------------------------------------------------
bool daemon_facade::read_config_file( const std::string& config_path )
{
        FILE *configfp;
    #define SBUF_SIZE 100
        char str[SBUF_SIZE];

        bool b_ret = false;

        configfp = fopen( (char*) config_path.c_str() , "r" );
        if (configfp != NULL)
        {
            //ignore nonexistent file
            if (fgets(str, SBUF_SIZE, configfp) == NULL)
                str[0] = '\0';
            else
                str[strlen(str) - 1] = '\0';
            //strip trailing '\n'
            std::ostringstream ostr;
            ostr << "Read config file: " << str << std::ends;
            b_ret = log_message( ostr.str() );

            fclose( configfp );
        }

        return b_ret;

}
//---------------------------------------------------------------------------------------------------
int daemon_facade::daemon_default_exec( const std::string& params , const unsigned long dw_flags )
 {
      int options = 0;
      options |= LOG_PID;
      const char* identity = "chromatic-daemon";

      //daemonize has closed all file handles , reopen log file if closed
      if( m_b_opened == false )
      {
          m_b_opened = open_log();
          if( m_b_opened == true )
          {
            std::string msg( "chromatic_daemon.....using default module.....william k. johnson 2015" );
            log_message( msg );
            read_config_file( "/dev_src/config/chromatic_daemon.conf" );

           // syslog ( LOG_USER | LOG_INFO , "%s", "Activating chromatic-daemon...." );

          }

      }

      const int sleep_time = 5;
      int unslept = sleep_time;
      struct sigaction sa ;

      //handle sighup
      sigemptyset( &sa.sa_mask );
      sa.sa_flags = SA_RESTART;
      sa.sa_handler = sighup_handler;
      if( sigaction( SIGHUP , &sa , nullptr ) == -1 )  { cdi::chromatic_terminate(); }
      if( signal( SIGTERM , sigterm_handler ) == SIG_ERR )  { cdi::chromatic_terminate(); }

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
                  syslog ( LOG_USER | LOG_INFO , "%s", "chromatic-daemon received sighup....reading config....continuing" );
                  closelog();
                  //next sighup
                  cdi::hup_received = 0;
                  close_log();
                  open_log();
                  read_config_file( "/dev_src/config/chromatic_daemon.conf" );
              }
              if( unslept == 0 )
              {
                 unslept = sleep_time;
                 std::string msg( "working....." );
                 log_message( msg );

              }

          }
      }

      return ( 0 );
}

