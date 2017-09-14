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
	void handle_pid()
	{
		
		if( ! remove_pid( cci_daemonize::path_pid ) ) 
		{ ACE_DEBUG(( LM_ERROR , "%D (%P) ....remove pid failed...s\n"  ) ); }
		else{ ACE_DEBUG(( LM_INFO , "%D (%P) ....removed pid file...\n"  ) ); }

	}
	
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
					
					handle_pid();
					

					exit( 1 );

				case SIGINT :
						
					ACE_DEBUG ( ( LM_DEBUG, ACE_TEXT ( "%D %P sigint occurred...shutting down server..\n" )  ) );
					_t()->clear_color();

					
					handle_pid();

					exit( 0 );
					
				default :
					_t()->clear_color();

					return 0;
			  }					    			    
		  }

	};
	
	
				
}

namespace cci_policy
{
	
	//
	//environment policies
	//
	template<typename T>
	class default_environment_context
	{

	
		public :
	
			
			default_environment_context() : m_tutils( std::make_unique<time_utils>() ) 
			{
			}
	
		protected :
			
					//daemon_signal_handler h1(SIGTERM);

			std::unique_ptr<cpp_real_stream::time_utils> 	m_tutils;
			
			//dtor
			~default_environment_context()
			{}
		
		public :
	
			//accessors-inspectors
			cpp_real_stream::time_utils_ptr _t() { return m_tutils.get(); }
	
			cci_daemonize::daemon_proc configure_environment()
			{
			    ACE_TRACE("default_environment_context::configure_environment_context");
			    
			    int flags { 0 };
			   			    
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
