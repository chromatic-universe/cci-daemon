//cci_daemon_bootstrap.cpp  chromatic unviverse    william k. johnson 2017
//


#include <cci_daemon_bootstrap.h>
#include <proc_ace_acceptor.h>

using namespace proc_ace;

static int  dw_thread_id;
static int  ccifs_timer_id;
static cci_daemon_impl::cci_daemon_facade_ptr g_facade_ptr;
std::ofstream ofstr;

//log callback
//------------------------------------------------------------------------
class log_callback : public ACE_Log_Msg_Callback
{
    public:

        void log (ACE_Log_Record &log_record)
        {
            log_record.print ( "", 0, cerr );
            log_record.print ( "", ACE_Log_Msg::VERBOSE , cerr );
        }
};

//------------------------------------------------------------------------------------
class ccifs_base_handler : public ACE_Event_Handler
{
           public:

                int handle_timeout( const ACE_Time_Value &current_time,
                                    const void * = 0 )
                {
                    ACE_DEBUG( ( LM_INFO ,  "%D cci-daemon-dispatcher...stack watchdog base handler...handle_timeout...\n" ) );

                    return 0;
                };
};


//signal handles
//------------------------------------------------------------------------
class bootstrap_signal_handler : public ACE_Event_Handler
{
            public:

                bootstrap_signal_handler() : ACE_Event_Handler()
                {}

                int handle_signal ( int signum ,
                                    siginfo_t * = 0 ,
                                    ucontext_t * = 0 )
                {

    	            ACE_Trace _( ACE_TEXT( "bootstrap_signal_handler" ) , __LINE__ );

                    switch( signum )
                    {
                        case SIGSEGV :
                            {
                                  ACE_DEBUG(( LM_INFO , "...cci-stream-mta protocol stack..sigsegv received..\n" ) );

                                  _exit( 1  );
                            }
                        case SIGINT :
                        case SIGTERM :

                            ACE_DEBUG(( LM_INFO , "...cci-daemon-dispatcher protocol stack..sigint received..\n" ) );
                            //we have to explicitly call facade destructor , seemingly because
                            //the service coordinator is being called from  a library and not ace tmain
                            if( g_facade_ptr )
                            { delete g_facade_ptr; }

                            ACE_Reactor::instance()->cancel_timer( ccifs_timer_id );

                            ACE_Thread_Manager::instance()->join( dw_thread_id );
                            //unravel protocol stack
                            ACE_DEBUG ((LM_INFO,
                                          "(%P|%t) ...unraveling protocol stack...\n"));
                            ACE_Service_Config::fini_svcs();
                            ACE_DEBUG ((LM_DEBUG,
                                                  "(%P|%t) shutting down dispatch protocol stack coordinator...\n"));

                            ofstr.close();
                            ACE_Reactor::instance()->end_reactor_event_loop();
                            ACE_Proactor::instance()->proactor_end_event_loop();

                            _exit( 0  );

                    }

                    return 0;
                }
};

//async reactor thread
//-----------------------------------------------------------------------
class HA_async_handler : public ACE_Task_Base
{
               public:

                  virtual int svc (void)
                  {
                    ACE_DEBUG
                      ((LM_DEBUG, ACE_TEXT ("(%t) ..kernel async handler thread..\n")));

                    ACE_Proactor::instance ()->proactor_run_event_loop ();

                    ACE_DEBUG
                      ((LM_DEBUG, ACE_TEXT ("(%t) ..kernel async exit..\n")));


                    return 0;

                  }
};


//coordinator
//------------------------------------------------------------------------
extern "C" int bootstrap_default_coordinator( int argc , char* argv[] , void* ptr )
{

                  g_facade_ptr = static_cast<cci_daemon_impl::cci_daemon_facade_ptr> ( ptr );
                  syslog ( LOG_USER | LOG_INFO | LOG_PID ,
                        "%s",
                        "default cci-dispatcher coordinator initializing...." );
                  ofstr.open ( "/var/log/cci-daemon/cci-daemon-dispatcher-trace.log",
                               std::ofstream::out | std::ofstream::app );
                  ACE_LOG_MSG->msg_ostream( &ofstr , 1 );
                  ACE_LOG_MSG->set_flags( ACE_Log_Msg::OSTREAM ) ;
                  ACE_LOG_MSG->set_flags( ACE_Log_Msg::STDERR );


                  //register signals
                  bootstrap_signal_handler* handler = new bootstrap_signal_handler();
                  ACE_Reactor::instance()->register_handler( SIGINT ,  handler );
                  ACE_Reactor::instance()->register_handler( SIGTERM ,  handler );
                  ACE_Reactor::instance()->register_handler( SIGSEGV ,  handler );


                  ACE_Trace _( ACE_TEXT( "HA_proc_acceptor::init" ) , __LINE__ );

                  //if daemonized , we're in the machine root directory
                  //move back to working directory
                  int dw = ::chdir( g_facade_ptr->chroot_dir().c_str() );
                  if( dw != 0 )
                  {
                    ACE_ERROR_RETURN( ( LM_ERROR , "%D (%t) could not chdir to working directory..exiting\n"   ) , 1  );
                  }
                  ACE_DEBUG( ( LM_INFO , "%D (%t) changed directory to working directory....\n" ) );

                  //bring up the stack
		  dw = ACE_Service_Config::open( argc , argv );
                  if( dw != 0 )
                  {
                      ACE_DEBUG ((LM_ERROR , "(%P|%t) ...unraveling protocol stack error..%d.\n" , dw )  );
                      ACE_ERROR_RETURN( ( LM_ERROR , "(%t) service config returned with errors..exiting\n"   ) , 1  );
                  }

                  //watchdong and utlity thread
                  ccifs_base_handler* hdlr = new ccifs_base_handler();
                  ACE_Time_Value initial_delay( 5 );
                  ACE_Time_Value interval( 5 );
                  ccifs_timer_id = ACE_Reactor::instance()->schedule_timer( hdlr ,
                                                                        0 ,
                                                                        initial_delay ,
                                                                        interval );

                  //activate proactor async services
                  HA_async_handler a_handler;
                  dw_thread_id = a_handler.activate( THR_NEW_LWP | THR_JOINABLE |  THR_SUSPENDED );
                  ACE_ASSERT( dw_thread_id == 0 );
                  ACE_DEBUG((LM_DEBUG ,
                                "(%t) the current thread count is %d\n",
                                a_handler.thr_count() ) );
                  ACE_DEBUG((LM_DEBUG , "(%t) the group identifier is %d\n",
                                        a_handler.grp_id() ) );
                  a_handler.resume();


                  ACE_DEBUG( ( LM_INFO , "%D (%t) opened coordinator....\n" ) );
                  ACE_DEBUG( ( LM_INFO , "%D (%t) starting event reactor....\n" ) );
                  ACE_Reactor::instance()->run_reactor_event_loop();

                  ACE_Service_Config::instance()->close();


                  return 0;

}

