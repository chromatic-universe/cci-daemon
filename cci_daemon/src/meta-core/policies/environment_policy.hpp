//environment_policy.hpp    willian k. johnson 2017

#include <map>
//contrib
#include "ace/Log_Msg.h"
#include "ace/Signal.h"
#include "ace/Sig_Handler.h"

//cci
#include <cci_time_utils.h>
#include <cci_daemonize.h>

using namespace cpp_real_stream;

namespace
{

	class daemon_signal_handler : public ACE_Event_Handler
	{


		public :
			
			  daemon_signal_handler ( int signum ) : signum_( signum ) ,
								 m_tutils( std::make_unique<time_utils>()  )

			  {}

			  virtual ~daemon_signal_handler()
			  {}


			
		private :
			
		  	int 						signum_;
			std::unique_ptr<cpp_real_stream::time_utils> 	m_tutils;

		public:

		  //accessors-inspectors
		  cpp_real_stream::time_utils_ptr _t() { return m_tutils.get(); }

		  //services
		  virtual int handle_signal( int signum ,
                                             siginfo_t * = 0 , 
                                             ucontext_t * = 0 )
		  {
			    ACE_TRACE ( "daemon_signal_handler::handle_signal" );

			    _t()->color( stamp_color::red );
	 		    _t()->null_stamp();


			    ACE_ASSERT( signum == this->signum_ );
				
			    switch( signum_ ) 
			    {
			     	case SIGTERM :
			
			    		ACE_DEBUG ( ( LM_DEBUG, ACE_TEXT ( "%D %P sigterm occurred....shutting down server\n" )  ) );
					_t()->clear_color();

					exit( 1 );

				case SIGINT :
						
					ACE_DEBUG ( ( LM_DEBUG, ACE_TEXT ( "%D %P sigint occurred...shutting down server..\n" )  ) );
					_t()->clear_color();

					exit( 0 );
					
				default :
					_t()->clear_color();
					return 0;
			    }					    			    
		  }

	};
	daemon_signal_handler h1(SIGTERM);
	daemon_signal_handler h2(SIGINT);

				
}

namespace cci_policy
{
	
	//
	//environment policies
	//
	template<typename T>
	class default_environment_context
	{

	
	
		protected :
			
			//attributes
			ACE_Sig_Handlers handler;
			//daemon_signal_handler h1(SIGTERM);

			std::unique_ptr<cpp_real_stream::time_utils> 	m_tutils;
			
			//dtor
			~default_environment_context()
			{}
		
		public :
	
				//accessors-inspectors
			cpp_real_stream::time_utils_ptr _t() { return m_tutils.get(); }
	
			default_environment_context() : m_tutils( std::make_unique<time_utils>()  )
			{
			}

			cci_daemonize::daemon_proc configure_environment()
			{
			    ACE_TRACE("default_environment_context::configure_environment_context");
			    //signals
			    ACE_DEBUG( ( LM_INFO , "%D (%P) ...registering signals....\n" ) );

			    int flags { 0 };

			    handler.register_handler (SIGTERM, &h1);
			    handler.register_handler (SIGINT,  &h2);
			    
			    ACE_DEBUG( ( LM_INFO , "%D (%P) ...changing directory to root dir....\n" ) );

			    return cd_to_root( flags );

			}


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
		
}
