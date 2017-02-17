//cci_daemon_bootstrap.cpp  chromatic unvierse william k. johnson 2017


#include <cci_daemon_bootstrap.h>
#include <proc_ace_acceptor.h>

using namespace proc_ace;

static int  dw_thread_id;


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
                                  ACE_DEBUG(( LM_INFO , "...cci-daemon-dispatcher protocol stack..sigsegv received..\n" ) );

                                  exit( 1  );
                            }
                        case SIGINT :
                        case SIGTERM :

                            ACE_DEBUG(( LM_INFO , "...cci-daemon-dispatcher protocol stack..sigint received..\n" ) );
                            ACE_Thread_Manager::instance()->join( dw_thread_id );
                            //unravel protocol stack
                            ACE_DEBUG ((LM_INFO,
                                          "(%P|%t) ...unraveling protocol stack...\n"));
                            ACE_Service_Config::fini_svcs();
                            ACE_DEBUG ((LM_DEBUG,
                                                  "(%P|%t) shutting down dispatch protocol stack coordinator...\n"));

                            ACE_Reactor::instance()->end_reactor_event_loop();
                            ACE_Proactor::instance()->proactor_end_event_loop();

                            //_exit( 0  );

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
extern "C" int bootstrap_default_coordinator( int argc , char* argv[] )
{

                  syslog ( LOG_USER | LOG_INFO | LOG_PID ,
                        "%s",
                        "default cci-dispatcher coordinator initializing...." );
                  ACE_LOG_MSG->open ( argv[0] , ACE_Log_Msg::SYSLOG, "cci-daemon-dispatcher" );
                  ACE_LOG_MSG->set_flags ( ACE_Log_Msg::STDERR );


                  //register signals
                  bootstrap_signal_handler* handler = new bootstrap_signal_handler();
                  ACE_Reactor::instance()->register_handler( SIGINT ,  handler );
                  ACE_Reactor::instance()->register_handler( SIGTERM ,  handler );
                  ACE_Reactor::instance()->register_handler( SIGSEGV ,  handler );


                  ACE_Trace _( ACE_TEXT( "HA_proc_acceptor::init" ) , __LINE__ );

                  //daemonizing has switched us to the root directory
                  int dw = ::chdir( "/cci/dev_t/bin" );
                  if( dw != 0 )
                  {
                    ACE_ERROR_RETURN( ( LM_ERROR , "%D (%t) could not chdir to working directory..exiting\n"   ) , 1  );
                  }
                  ACE_DEBUG( ( LM_INFO , "%D (%t) changed directory to working directory....\n" ) );
                  dw = ACE_Service_Config::open( argc , argv );
                  if( dw != 0 )
                  {
                      ACE_DEBUG ((LM_ERROR , "(%P|%t) ...unraveling protocol stack error..%d.\n" , dw )  );
                      ACE_ERROR_RETURN( ( LM_ERROR , "(%t) service config returned with errors..exiting\n"   ) , 1  );
                  }
                  //activate proactor
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

