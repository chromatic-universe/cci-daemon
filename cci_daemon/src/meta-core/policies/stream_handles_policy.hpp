//stram_handle_policies.hpp   william k. johnson 2017

#include <memory>
#include <string>

//contrib
#include "ace/Log_Msg.h"
#include "ace/Trace.h"


//cci
#include <cci_time_utils.h>
#include <cci_daemonize.h>


namespace cci_policy
{
	
		
		//meta
		//
		//stream handle policies
		//
		template<typename T>
		class ace_stream_context
		{
			

			public :

				//ctor
				ace_stream_context( int flags ) : m_flags{ 0 } ,
					m_str_arg( "cci-meta-daemon-dispatcher" ) ,
					m_ostr( std::make_unique<std::ofstream>( "/var/log/chromatic/cci_meta_daemon.log" ) )
				{}

			private :

				int     			m_flags;
				std::string 			m_str_arg;
				std::unique_ptr<std::ostream> 	m_ostr;

					
			protected :

				//dtor
				~ace_stream_context()
				{}

			public :
			

				cci_daemonize::daemon_proc configure_streams()
				{

					int flags { 0 };

					ACE_TRACE ("ace_stream_context:::configure_streams");

					cci_daemonize::daemon_proc dp = cci_daemonize::daemon_proc::dp_error;
					
					//clear file create mask
					ACE_DEBUG(( LM_ERROR , "%D (%P) ...clearing file create mask.....\n" ) );
					dp = clear_file_create_mask( m_flags );
					if( dp == cci_daemonize::daemon_proc::dp_success )
					{
				
						//ACE_LOG_MSG->open ( m_str_arg.c_str() , ACE_Log_Msg::SYSLOG , "chromatic universe");
						//ACE_LOG_MSG->set_flags (ACE_Log_Msg::STDERR);
						//ACE_LOG_MSG->msg_ostream ( m_ostr.get() , 0 );
						//ACE_LOG_MSG->set_flags (ACE_Log_Msg::OSTREAM);
						ACE_DEBUG(( LM_ERROR , "%D (%P) ...opened streams to context....\n" ) );
					}

					

					return dp; 
				}			


		};
		//
		template<typename T>
		class close_all_descriptors
		{
			

			public :

				//ctor
				close_all_descriptors( int flags ) : m_flags{ 0 } ,
					m_str_arg( "cci-meta-daemon-dispatcher" ) ,
					m_ostr( std::make_unique<std::ofstream>( "cci_meta_daemon.log" ) )
				{}

			private :

				int     			m_flags;
				std::string 			m_str_arg;
				std::unique_ptr<std::ostream> 	m_ostr;

					
			protected :

				//dtor
				~close_all_descriptors()
				{}

			public :
			

				cci_daemonize::daemon_proc configure_streams()
				{

					int flags { 0 };

					ACE_TRACE ("close_all_descriptors:::configure_streams");

					cci_daemonize::daemon_proc dp = cci_daemonize::daemon_proc::dp_error;
					
					//clear file create mask
					ACE_DEBUG(( LM_ERROR , "%D (%P) ...clearing file create mask.....\n" ) );
					dp = clear_file_create_mask( m_flags );
					if( dp == cci_daemonize::daemon_proc::dp_success )
					{
						dp = cci_daemonize::daemon_proc::dp_error;
						ACE_DEBUG(( LM_ERROR , "%D (%P) ...closing all open files....\n" ) );
						dp = close_all_open_files( m_flags );
						if( dp == cci_daemonize::daemon_proc::dp_success )
						{
							dp = cci_daemonize::daemon_proc::dp_error;
							dp = reopen_streams_to_dev_null( m_flags );
						}
					}

					

					return dp; 
				}			

		};


	
}
