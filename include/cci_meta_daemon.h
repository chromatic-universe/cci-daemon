//cci_meta_daemon  william k. johnson   2017

//c++ standard
#include <iostream>
#include <string>
#include <memory>
#include <sstream>
#include <exception>

//contrib
#include "ace/Log_Msg.h"

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
	//meta
	//
	//creation policies
	//
	template<typename T>
	class op_new_creator
	{
		public :
			
			static T* create()
			{
				return new T;
			}

		protected :

			//dtor
			~op_new_creator()
			{}

	};	
	//
	template<typename T>
	class runtime_creator
	{
		public :
			
			static T* create()
			{
				void* mem_buffer = std::malloc( sizeof(T) );
				return mem_buffer ? new(mem_buffer) T : nullptr;
			}

		protected :
			
			//dtor
			~runtime_creator()
			{}

	};
	//
	template<typename T>
	class prototype_creator
	{
		public :

			//ctor
			prototype_creator( T* obj_ptr = nullptr) 
				: m_ptr_prototype( obj_ptr )
			{}
			
			T* create()
			{
				return m_ptr_prototype ? m_ptr_prototype->clone()
	            	                               : nullptr;
			}	
			
		private :

			//attributes
			T* 	m_ptr_prototype;

		protected :

			//dtor
			~prototype_creator()
			{}


		public :

			//accessors-inspectors
			T* prototype() const { return m_ptr_prototype; }
			//mutators
			void prototype( T* proto ) { m_ptr_prototype = proto; };
	};
	//
	//stream handle policies
	//
	template<typename T>
	class close_all_descriptors
	{
		public :
			

			void configure_descriptors()
			{
				//
			}


		protected :

			//dtor
			~close_all_descriptors()
			{}


	};
	//
	//environment policies
	//
	template<typename T>
	class default_environment_context
	{
		public :
			

			void configure_environment()
			{
			   ACE_TRACE("defailt_environment_context::configure_environment_context");
			}

		protected :
			
			//dtor
			~default_environment_context()
			{}


	};
	//
	template<typename T>
	class chroot_environment_context
	{
		public :
			

			void configure_environment()
			{
				//
			}

		protected :
			
			//dtor
			~chroot_environment_context()
			{}


	};
	//
	//logging policies
	//
	template<typename T>
	class sysroot_logging_context
	{
		public :
			

			void configure_logging_context()
			{
				//
			}

		protected :
			
			//dtor
			~sysroot_logging_context()
			{}


	};
	//
	template<typename T>
	class ace_framework_logging_context
	{
		public :
		
			//ctora
			ace_framework_logging_context() : m_str_arg( "cci-meta-daemon-dispatcher" )
			{}
				

			void configure_logging_context()
			{

				ACE_LOG_MSG->open ( m_str_arg.c_str() );
				//output to default destination (stderr)
				ACE_TRACE ("ace_framework_logging_context::configure_logging_context");				
			}
		
		private :
			
			//attributes
			std::string 	m_str_arg;


		protected :
			
			//dtor
			~ace_framework_logging_context()
			{}


	};	
        //
	//system init policies
	//
	template<typename T>
	class runtime_sys_init
	{

		private :

			//attributes
			std::unique_ptr<std::string> 	m_runtime_data;

		protected :
			
			//dtor
			~runtime_sys_init()
			{}

		public :

			//accessors-inspctiors
			std::string runtime_data() const noexcept { return *m_runtime_data.get(); }

			//services
			void configure_init()
			{
				//
			}


	};
	//
	template<typename T>
	class custom_sys_init
	{
		private :

			//attributes
			std::unique_ptr<std::string> 	m_runtime_data;

		protected :
			
			//dtor
			~custom_sys_init()
			{}

		public :

			//accessors-inspctiors
			std::string runtime_data() const noexcept { return *m_runtime_data.get(); }
			
			//services
			void configure_init()
			{
				//
			}




	};
	//
	//daemon procedure policies
	//
	template<typename T>
	class default_daemon_procedure
	{

		public :

		
			int proc_init()
			{
				return 0;
			}
			int perform()
			{
				return 0;
			}
			int fini()
			{
				return 0;
			}


		protected :

			//dtor
			~default_daemon_procedure()
			{}


	};

	

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


