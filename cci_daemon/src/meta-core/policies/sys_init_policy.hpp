//sys_init_policy.hpp    william k. johnon    2017


#include <memory>
#include <string>

//contrib
#include "ace/Log_Msg.h"

namespace cci_policy
{
		
				
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

			//mutators
			void runtime_data( std::unique_ptr<std::string>& data ) 
			{
				m_runtime_data = std::move( data );
			}

			//services
			void configure_init()
			{
				ACE_TRACE ("runtime_sys_init::configure_init");				
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
