//sys_init_policy.hpp    chromatic universe 2017-2020  william k. johnson


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
			runtime_sys_init( T meta ) : m_tutils( std::make_unique<time_utils>() ) ,
						     m_meta( meta )
			{}


		private :

			//attributes
			std::unique_ptr<std::string> 	m_runtime_data;
			std::unique_ptr<cpp_real_stream::time_utils> 	m_tutils;
			T 				m_meta;


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
			void configure_init ( T meta )
			{


				cci_daemonize::daemon_proc dp = cci_daemonize::daemon_proc::dp_fork_background_proc;

				ACE_TRACE ("runtime_sys_init::configure_init");

				while( dp != cci_daemonize::daemon_proc::dp_error )
				{
					switch( dp  )
					{

					    case cci_daemonize::daemon_proc::dp_fork_background_proc :
					    {

                            //become background process
                            switch( fork() )
                            {
                                case -1 :
                                dp = cci_daemonize::daemon_proc::dp_error;
                                break;
                                case  0 :
                                dp = cci_daemonize::daemon_proc::dp_make_session_leader;
                                break;
                                default:
                                _exit( EXIT_SUCCESS );
                            }
                            break;
					    }
					    case cci_daemonize::daemon_proc::dp_make_session_leader :
					    {
                            //become leader of new session
                            setsid() == -1 ? dp = cci_daemonize::daemon_proc::dp_error :
                                                                    dp = cci_daemonize::daemon_proc::dp_fork_no_session_leader;
                            break;
					    }
					    case cci_daemonize::daemon_proc::dp_fork_no_session_leader :
					    {
                            //ensure we are not session leader
                            switch( fork() )
                            {
                                case -1 :
                                dp = cci_daemonize::daemon_proc::dp_error;
                                break;
                                case  0 :
                                dp = cci_daemonize::daemon_proc::dp_daemonized;						break;
                                default:
                                _exit( EXIT_SUCCESS );
                            }
                            break;
					    }
                        default :
                            break;
					}
					if ( dp == cci_daemonize::daemon_proc::dp_daemonized ) { break; }
				}
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
