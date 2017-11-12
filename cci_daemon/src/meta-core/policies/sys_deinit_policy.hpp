//sys_deinit_policy.hpp    william k. johnson   2017


#pragma once

#include <cci_meta_daemon.h>

//contrib
#include "ace/Log_Msg.h"
//c runtime
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>


std::string shared_memory_moniker { "cci-stream-mta-mmc" };
std::string shared_memory_semaphore_moniker{ "cci-stream-mta-mmc-sema" };
size_t shared_memory_size = 4096L;





namespace cci_policy
{

		//
		//deinit policies
		//
		template<typename T>
		class runtime_sys_deinit
		{
			public :
				

			
			protected :
				
				//dtor
				~runtime_sys_deinit()
				{ ACE_LOG_MSG->clr_flags (ACE_Log_Msg::OSTREAM); }

			public :
 
				void configure_deinit( T meta )
				{
					//
				}
	

		};
		
	
	
}	
