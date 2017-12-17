//daemon_procedure_policy.hpp    william k. johnon    2017


#include <memory>
#include <string>
#include <chrono>
#include <future>

//contrib
#include "ace/Log_Msg.h"
#include "ace/Trace.h"
#include <cci_time_utils.h>

using namespace cpp_real_stream;

namespace cci_policy
{
	
	//
	//daemon procedure policies
	//
	template<typename T>
	class default_daemon_procedure
	{
	
		public :

			default_daemon_procedure( T meta = nullptr ) : m_tutils( std::make_unique<time_utils>()  ) ,
								       m_meta { meta }
			{
				_t()->color( stamp_color::green );
	 			_t()->null_stamp(); 

				ACE_TRACE ("default_daemon_procedure::default_daemon_procedure");

				_t()->clear_color();
			}
	
		private :

			std::unique_ptr<cpp_real_stream::time_utils> 	m_tutils;
			T  						m_meta;

		public :

			//accessors-inspectors
			cpp_real_stream::time_utils_ptr _t() { return m_tutils.get(); }

			
			//services		
			int proc_init()
			{
				ACE_TRACE ("default_daemon_procedure::proc_init");

				perform();

				return 0;
			}

			int perform()
			{
				ACE_TRACE ("default_daemon_procedure::perform");
				
				daemon_signal_handler h1(SIGTERM);
				daemon_signal_handler h2(SIGINT);
				//attributes
				ACE_Sig_Handlers handler;
				//signals
				ACE_DEBUG( ( LM_INFO , "%D (%P) ...registering signals....\n" ) );
				handler.register_handler (SIGTERM, &h1);
				handler.register_handler (SIGINT,  &h2);
	

				for( ;; )
				{
									

					std::this_thread::sleep_for( std::chrono::seconds( 5 ) );

					_t()->color( stamp_color::green );
	 				_t()->null_stamp();

					ACE_DEBUG(( LM_INFO , "%D (%P) ....working.....\n" ) );

					_t()->clear_color();
				}

				return 0;
			}

			int fini()
			{
				ACE_TRACE ("default_daemon_procedure::fini");
				return 0;
			}


		protected :

			//dtor
			~default_daemon_procedure()
			{}


	};

		
} 
