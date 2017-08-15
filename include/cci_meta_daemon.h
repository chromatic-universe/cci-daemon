//cci_meta_daemon   chromatic universe  2017 william k. johnson   

//c++ standard
#include <iostream>
#include <string>
#include <memory>
#include <sstream>
#include <exception>

//contrib
#include "ace/Log_Msg.h"
#include <ace/Trace.h>
#include <json.hpp>


//policies
#include <stream_handles_policy.hpp>
#include <creator_policy.hpp>
#include <environment_policy.hpp>
#include <logging_policy.hpp>
#include <sys_init_policy.hpp>
#include <daemon_procedure_policy.hpp>
#include <command_line_policy.hpp>

//cci
#include <cci_daemonize.h>


namespace cci_policy
{
		
	//forward declarations
	

	//enumerations
	
	
	//
	//meta
	//
	//utils
	//
	
	//
	//services
	//
	class placeholder
	{	
				
		public :

			//ctor
			placeholder() : m_cur_proc { cci_daemonize::daemon_proc::dp_error }
			{}

		private :
			
			cci_daemonize::daemon_proc m_cur_proc;

		public :

			//accessors-inspectors
			cci_daemonize::daemon_proc state() const noexcept
			{ return m_cur_proc; }
			//mutators
			void state( cci_daemonize::daemon_proc proc )
			{ m_cur_proc = proc; }

			
	};
	//
	//cci_daemon_dispatcher host
	//
	template<
		  typename T ,
		  template <class> class descriptor_policy ,
		  template <class> class environment_policy,
		  template <class> class logging_policy ,
		  template <class> class init_policy ,
		  template <class> class daemon_proc_policy ,
		  template <class> class command_line_policy 
		>
	class cci_daemon_dispatcher : public descriptor_policy<T> ,
	                              public environment_policy<T> ,
				      public logging_policy<T> ,
				      public init_policy<T> ,
				      public daemon_proc_policy<T> ,
				      public command_line_policy<T>
	{

		public :


			//ctors
			cci_daemon_dispatcher( T meta )  : init_policy<T>( meta ) ,
                                                           m_meta( meta ) 

			{
			  	ACE_TRACE ("cci_daemon_dispatcher::cci_daemon_dispatcher");
			}  

			//dtor
			~cci_daemon_dispatcher() = default;			


			//no copy
			cci_daemon_dispatcher( const cci_daemon_dispatcher& ) = delete;
			//no assign
			const cci_daemon_dispatcher& operator= ( const cci_daemon_dispatcher& cdd ) = delete;
			

		private :

			T 	m_meta;

		public :
			
			//services
			//-------------------------------------------------------------------------------
			virtual void daemonize();
			//-----------------------------------------------------------------------------
			virtual void cli()
			{
				ACE_TRACE ("cci_daemon_dispatcher::cli");
			}



	};
	using default_daemon_dispatcher = cci_daemon_dispatcher<placeholder* , 
							        close_all_descriptors ,
	                                                        default_environment_context ,
								ace_framework_logging_context ,
								runtime_sys_init ,
								default_daemon_procedure ,
								default_command_line>;

	//-----------------------------------------------------------------------------------------------
	template<
		  typename T ,
		  template <class> class descriptor_policy ,
		  template <class> class environment_policy,
		  template <class> class logging_policy ,
		  template <class> class init_policy ,
		  template <class> class daemon_proc_policy ,
		  template <class> class command_line_policy 
		>
	void cci_daemon_dispatcher<T ,
				    descriptor_policy ,
				    environment_policy,
				    logging_policy ,
				    init_policy ,
				    daemon_proc_policy ,
				    command_line_policy>::daemonize()
 
 	{
				ACE_TRACE ("cci_daemon_dispatcher::daemonize");
				
				//logging
				this->configure_logging_context();
				//forks and terminsal seesion
				this->configure_init();
				//streans and descriptors
			        this->configure_descriptors();
				//signals and working directory
				this->configure_environment();
				//daemon pric perform
				this->proc_init();

							
	}

	//
	//cci_daemon_mgr host
	//
	template<typename creation_policy>
	class cci_daemon_mgr : public creation_policy
	{
		//
	};
	
	
        	

 	

}


