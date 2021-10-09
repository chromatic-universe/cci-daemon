//cci_daemonize.h     william k. johnson  2017-2021

#pragma once
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-function"

//c runtime
#include <execinfo.h>
#include <cstdlib>
#include <cstdio>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <syslog.h>
#include <sys/stat.h>

//c++ standard
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <memory>
#include <exception>
#include <algorithm>

namespace cci_daemonize
{
		//enumerations
            	enum class daemon_proc : unsigned long
            	{
                	dp_fork_background_proc = 0 ,
                	dp_fork_no_session_leader ,
               	 	dp_make_session_leader ,
                	dp_clear_file_create_mask ,
                	dp_change_to_root_dir ,
                	dp_close_all_open_files ,
                	dp_reopen_streams_dev_null ,
                	dp_daemonized ,
                	dp_error ,
			        dp_success
            	};


	   //immutable
	   static const unsigned bd_no_chdir = 01;
       ///< don't close all open files
       static const unsigned bd_no_close_files = 02;
       ///< don;t reopen stdin , stderr and std:out to to /dev_null
       static const unsigned bd_no_reopen_std_fds = 04;
       ///< don't do a umask( 0 )
       static const unsigned bd_no_umask_0 = 010;
       ///< maximum file descriptors to close if sysconf is indeterminate
       static const unsigned bd_max_handles = 8192;
	   /// buffer size
	   static const unsigned buffer_size = 1024;
	   ///close exec
       static const unsigned cpf_cloexec = 1;
	   ///pid path
	   static const std::string path_pid { "/var/run/cci-daemon.pid" };




}
//-----------------------------------------------------------------------------------------
extern "C"
{
	//daemonize
	int daemonize_( const unsigned long flags );
	//seesion console leader
	cci_daemonize::daemon_proc make_session_leader();
	//fork daemon
	cci_daemonize::daemon_proc make_into_background();
	//fork session leader
	cci_daemonize::daemon_proc make_fork_no_session_leader();
	//clear create mask
	cci_daemonize::daemon_proc clear_file_create_mask( const int flags );
	//close open files
	cci_daemonize::daemon_proc close_all_open_files( const int flags );
	//reopen to /dev/null
	cci_daemonize::daemon_proc reopen_streams_to_dev_null( const int flags );
	//change to rott directory
	cci_daemonize::daemon_proc cd_to_root( const int flags );
	//lock region
	int lock_region( int fd ,
			 int type ,
			 int whence ,
			 int start ,
			 int len );
	//write pid
	bool write_pid( const std::string& pid_file ,
					   std::ostream& ostr ,
					   int flags  );
	//remove pd
	bool remove_pid( const std::string& pid_file );


}

