//logging_policy.hpp    william k. johnson   2017

//contrib
#include "ace/Log_Msg.h"

namespace cci_policy
{

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
}
