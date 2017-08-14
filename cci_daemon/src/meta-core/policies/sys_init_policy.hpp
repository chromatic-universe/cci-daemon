//sys_init_policy.hpp    william k. johnon    2017


#include <memory>
#include <string>

//contrib
#include "ace/Log_Msg.h"
#include "ace/Trace.h"


//cci
#include <cci_time_utils.h>
#include <cci_daemonize.h>

using namespace cpp_real_stream;




namespace cci_policy
{
		
				
        //
	//system init policies
	//
	template<typename T>
	class runtime_sys_init
	{

		public :

			//ctor
			runtime_sys_init() : m_tutils( std::make_unique<time_utils>()  )
			{}


		private :

			//attributes
			std::unique_ptr<std::string> 	m_runtime_data;
			std::unique_ptr<cpp_real_stream::time_utils> 	m_tutils;


		protected :
			
			//dtor
			~runtime_sys_init()
			{}

		public :

			//accessors-inspctiors
			std::string runtime_data() const noexcept { return *m_runtime_data.get(); }
			cpp_real_stream::time_utils_ptr _t() { return m_tutils.get(); }
			//mutators
			void runtime_data( std::unique_ptr<std::string>& data ) 
			{ m_runtime_data = std::move( data ); }


			//services
			cci_daemonize::daemon_proc configure_init()
			{
				
				_t()->color( stamp_color::green );
	 			_t()->null_stamp();

				cci_daemonize::daemon_proc dp = cci_daemonize::daemon_proc::dp_error;

				ACE_TRACE ("runtime_sys_init::configure_init");	
				dp =  make_into_background();
 				if( dp == cci_daemonize::daemon_proc::dp_error )
				{ 
					ACE_DEBUG(( LM_ERROR , "%D (%P) ...could not fork into background.....\n" ) ); 
				}
						
				_t()->clear_color();

				return dp;

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



	
}
