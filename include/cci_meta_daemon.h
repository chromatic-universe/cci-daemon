//cci_meta_daemon  william k. johnson   2017

//c++ standard
#include <iostream>
#include <string>
#include <memory>
#include <sstream>
#include <exception>

//contrib
#include "ace/Log_Msg.h"
//policies
#include <stream_handles_policy.hpp>
#include <creator_policy.hpp>
#include <environment_policy.hpp>
#include <logging_policy.hpp>
#include <sys_init_policy.hpp>
#include <daemon_procedure_policy.hpp>
#include <command_line_policy.hpp>

namespace cci_policy
{
		
	//forward declarations
	

	//enumerations
	
	
	//
	//meta
	//
	//utils
	//
	
;	//
	//services
	//
	class placeholder
	{
		public :

			//ctor
			placeholder()
			{}
	};
	//
	//cci_daemon_dispatcher host
	//
	template<
		  template <class> class descriptor_policy ,
		  template <class> class environment_policy,
		  template <class> class logging_policy ,
		  template <class> class init_policy ,
		  template <class> class daemon_proc_policy
		>
	class cci_daemon_dispatcher : public descriptor_policy<placeholder> ,
	                              public environment_policy<placeholder> ,
				      public logging_policy<placeholder> ,
				      public init_policy<placeholder> ,
				      public daemon_proc_policy<placeholder>
	{

		public :


			//ctors
			explicit cci_daemon_dispatcher()
			{
			  ACE_TRACE ("cci_daemon_dispatcher::cci_daemon_dispatcher");
			}  

			//dtor
			~cci_daemon_dispatcher() = default;			


			//no copy
			cci_daemon_dispatcher( const cci_daemon_dispatcher& ) = delete;
			//no assign
			const cci_daemon_dispatcher& operator= ( const cci_daemon_dispatcher& cdd ) = delete;


			virtual void daemonize()
			{
				ACE_TRACE ("cci_daemon_dispatcher::daemonize");

				
				//logging
				this->configure_logging_context();
				//descriptors and streams
				this->configure_descriptors();
				//environment and directory root context
				this->configure_environment();
				//systen init and signals
				this->configure_init();
				//
				//procedure
				this->proc_init();				;
			}



	};
	using default_daemon_dispatcher = cci_daemon_dispatcher<close_all_descriptors ,
	                                                        default_environment_context ,
								ace_framework_logging_context ,
								runtime_sys_init ,
								default_daemon_procedure>;
	//
	//cci_daemon_mgr host
	//
	template<typename creation_policy>
	class cci_daemon_mgr : public creation_policy
	{
		//
	};
	
	
        	

 	

}


