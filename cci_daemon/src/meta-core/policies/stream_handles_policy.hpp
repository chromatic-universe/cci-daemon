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
		class close_all_descriptors
		{
			public :
			

				cci_daemonize::daemon_proc configure_descriptors()
				{

					int flags { 0 };

					ACE_TRACE ("close_all_descriptors::configure_descriptors");

					cci_daemonize::daemon_proc dp = cci_daemonize::daemon_proc::dp_error;
					
					//clear file create mask
					ACE_DEBUG(( LM_ERROR , "%D (%P) ...clearing file create mask.....\n" ) );
					dp = clear_file_create_mask( flags );
					if( dp == cci_daemonize::daemon_proc::dp_success )
					{
						dp = cci_daemonize::daemon_proc::dp_error;
						ACE_DEBUG(( LM_ERROR , "%D (%P) ...closing all open files....\n" ) );
						dp = close_all_open_files( flags );
						if( dp == cci_daemonize::daemon_proc::dp_success )
						{
							dp = cci_daemonize::daemon_proc::dp_error;
							ACE_DEBUG(( LM_ERROR , "%D (%P) ...reopening streams to /dev/null....\n" ) );
							dp = reopen_streams_to_dev_null( flags );
						}
					}

					return dp; 
				}


			protected :

				//dtor
				~close_all_descriptors()
				{}


		};

	
}
