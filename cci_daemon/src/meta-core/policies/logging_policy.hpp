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
			{ ACE_LOG_MSG->clr_flags (ACE_Log_Msg::OSTREAM); }


	};
	//
	template<typename T>
	class ace_framework_logging_context
	{
		public :
		
			//ctora
			ace_framework_logging_context() : m_str_arg( "cci-meta-daemon-dispatcher" ) ,
							  m_ostr( std::make_unique<std::ofstream>( "cci_meta_daemon.log" ) )
			{}
				

			void configure_logging_context()
			{

				ACE_TRACE ("ace_framework_logging_context::configure_logging_context");
				ACE_LOG_MSG->open ( m_str_arg.c_str() , ACE_Log_Msg::SYSLOG , "chromatic universe");
				//ACE_LOG_MSG->set_flags (ACE_Log_Msg::STDERR);
				//output to default destination (stderr)
				ACE_LOG_MSG->set_flags (ACE_Log_Msg::STDERR);				
				
				ACE_LOG_MSG->msg_ostream ( m_ostr.get() , 0 );
				ACE_LOG_MSG->set_flags (ACE_Log_Msg::OSTREAM);
			}
		
		private :
			
			//attributes
			std::string 			m_str_arg;
			std::unique_ptr<std::ostream> 	m_ostr;
			T 				m_meta;

	

		protected :
			
			//dtor
			~ace_framework_logging_context()
			{
				
			}

	};
}
