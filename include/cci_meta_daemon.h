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
			placeholder()
			{}
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
			cci_daemon_dispatcher( T meta )  : m_meta( meta )
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

				cci_daemonize::daemon_proc dp = cci_daemonize::daemon_proc::dp_fork_background_proc;
				while( dp != cci_daemonize::daemon_proc::dp_error )
				{
					switch( dp )
					{
						case cci_daemonize::daemon_proc::dp_fork_background_proc :
						{
							
							//fork inot background;terminaal session config
							//continues on success to session leader
							dp = this->configure_init();
							break;
						}
						case cci_daemonize::daemon_proc::dp_make_session_leader :
						{
							//become leader of new session
							dp = make_session_leader();
							// go tothe next state || signal dameonized || exit on error
							if( dp == cci_daemonize::daemon_proc::dp_success ) 
					                { dp = cci_daemonize::daemon_proc::dp_fork_no_session_leader; }
							      
							break;
						}
					        case cci_daemonize::daemon_proc::dp_daemonized :
						case cci_daemonize::daemon_proc::dp_error :
						{
							//
						}
						default :
							break;
					}
							/*if( dp == cci_daemonize::daemon_proc::dp_success  )
							{
								//logging
								this->configure_logging_context();
								//descriptors and streams
								this->configure_descriptors();
								//environment and directory root context
								this->configure_environment();
								//procedure
								this->proc_init();
							}
					      }
					}*/
					if ( dp == cci_daemonize::daemon_proc::dp_daemonized ) { break; }

								
				}
			
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


