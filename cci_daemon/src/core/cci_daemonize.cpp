//cci_daemonize.cpp    william k. johnson   2017


#include <cci_daemonize.h>
#include <thread>

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
         return setsid() == -1 ? daemon_proc::dp_error  : daemon_proc::dp_success;
}

//--------------------------------------------------------------------------------------
daemon_proc make_into_background()
{
	  daemon_proc dp = daemon_proc::dp_error;

	  //become background process
	  switch( fork() )
  	  {
	    case -1 :
		dp = daemon_proc::dp_error;
		break;
	    case  0 :
		dp = daemon_proc::dp_success;
		break;
	    default:
		_exit( EXIT_SUCCESS );
	  } 


          return dp;
}

//--------------------------------------------------------------------------------------
daemon_proc make_fork_no_session_leader()
{
	  daemon_proc dp = daemon_proc::dp_error;

	  //ensure we are not session leader
	  switch( fork() )
	  {
	    case -1 :
		dp = daemon_proc::dp_error;
		break;
	    case  0 :
		dp = daemon_proc::dp_success;
		break;
	    default: 
		_exit( EXIT_SUCCESS );
	 }
	 
	 return dp;

}

//--------------------------------------------------------------------------------------
daemon_proc clear_file_create_mask( const int flags )
{
	daemon_proc dp = daemon_proc::dp_error;

 	//clear file creation mask
        if( !( flags & bd_no_umask_0 ) )
	{
		 umask( 0 ); 
       		 dp =  daemon_proc::dp_success;
	}

	return dp;	
}

//--------------------------------------------------------------------------------------
daemon_proc close_all_open_files( const int flags )
{

	int maxfd , fd;
	daemon_proc dp = daemon_proc::dp_error;

        if( !( flags & bd_no_close_files ) )
	{
	     maxfd = sysconf( _SC_OPEN_MAX );
	     //if limit is indeterminate , guess
	     if( maxfd == -1 ) { maxfd = bd_max_handles; }
	     for( fd = 0; fd < maxfd; fd++ ) { close( fd ); }

	     dp = daemon_proc::dp_success;
	     
	}

	return dp;

}

//--------------------------------------------------------------------------------------
daemon_proc reopen_streams_to_dev_null( const int flags )
{
 		
		int fd;
		daemon_proc dp = daemon_proc::dp_success;

		//reopen standard streams /dev/null
                 if( !( flags & bd_no_reopen_std_fds ) )
                 {
                     close( STDIN_FILENO );

                     fd = open( "/dev/null" , O_RDWR );
                     //fd shoudl be 0
                     if( fd != STDIN_FILENO ) { dp = daemon_proc::dp_error;}
                     if( dup2( STDIN_FILENO , STDOUT_FILENO ) != STDOUT_FILENO ) { dp = daemon_proc::dp_error; }
                     if( dup2( STDIN_FILENO , STDERR_FILENO ) != STDERR_FILENO ) { dp = daemon_proc::dp_error; }

                 }

		return dp;	
}

//--------------------------------------------------------------------------------------
daemon_proc cd_to_root( const int flags )
{
	 	daemon_proc dp = daemon_proc::dp_error;

		//change to root directory
                if( !( flags & bd_no_chdir ) ) 
		{ 
		   int ret = chdir( "/" ); 
		   ret == 0 ? dp = daemon_proc::dp_success : daemon_proc::dp_error; 
		}
                
		return dp;
	
}

