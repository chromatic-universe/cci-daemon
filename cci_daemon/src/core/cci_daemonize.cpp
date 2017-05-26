//cci_daemonize.cpp    william k. johnson   2017


#include <cci_daemonize.h>


using namespace cci_daemonize;


//-----------------------------------------------------------------------------------------
int daemonize_( const unsigned long flags )
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
                if( !( flags & bd_no_umask_0 ) ) { umask( 0 ); }
                dp =  daemon_proc::dp_change_to_root_dir;

                break;
            }
            case daemon_proc::dp_change_to_root_dir :
            {
                 //change to root directory
                 if( !( flags & bd_no_chdir ) ) { int ret = chdir( "/" ); }
                 dp = daemon_proc::dp_close_all_open_files;

                 break;
            }
            case daemon_proc::dp_close_all_open_files :
            {
                //close all open files
                if( !( flags & bd_no_close_files ) )
                {
                     maxfd = sysconf( _SC_OPEN_MAX );
                     //if limit is indeterminate , guess
                     if( maxfd == -1 ) { maxfd = bd_max_handles; }
                     for( fd = 0; fd < maxfd; fd++ ) { close( fd ); }

                     dp = daemon_proc::dp_reopen_streams_dev_null;

                     break;
                }
                break;
            }
            case daemon_proc::dp_reopen_streams_dev_null :
            {
                 //reopen standard streams /dev/null
                 if( !( flags & bd_no_reopen_std_fds ) )
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

//--------------------------------------------------------------------------------------
daemon_proc make_session_leader()
{
	 //become leader of new session
         return setsid() == -1 ? daemon_proc::dp_error : daemon_proc::dp_success;	
}

