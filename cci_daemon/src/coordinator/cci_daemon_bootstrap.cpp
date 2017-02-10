//cci_daemon_bootstrap.cpp  chromatic unvierse william k. johnson 2017


#include <cci_daemon_bootstrap.h>
#include <proc_ace_acceptor.h>

using namespace proc_ace;

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
                        case SIGINT :
                        case SIGTERM :

                            ACE_DEBUG(( LM_INFO , "...cci-dameon-dispatcher protocol stack..sigint received..\n" ) );

                            //fini
                            ACE_Thread_Manager::instance()->cancel_all();
                            ACE_Time_Value timeout( 5 );
                            if ( ACE_Thread_Manager::instance()->wait( &timeout , 1 ) == -1 )
                            {
                                  ACE_ERROR_RETURN( ( LM_ERROR , "(%t) wait() failed\n") , 1 );
                            }

                            ACE_DEBUG ((LM_DEBUG,
                                          "(%P|%t) shutting down dispatch protocol stack coordinator...\n"));

                            //unravel protocol stack
                            ACE_DEBUG ((LM_INFO,
                                          "(%P|%t) ...unraveling protocol stack...\n"));
                            ACE_Service_Config::fini_svcs();

                            exit( 1 );
                    }

                    return 0;
                }
};


//coordinator
//------------------------------------------------------------------------
extern "C" int bootstrap_default_coordinator( const std::string& params )
{

         syslog ( LOG_USER | LOG_INFO | LOG_PID ,
                "%s",
                "default cci-dispatcher coordinator initializing...." );


          auto callback = std::make_unique<log_callback>();
          ACE_LOG_MSG->set_flags (ACE_Log_Msg::MSG_CALLBACK);
          ACE_LOG_MSG->clr_flags (ACE_Log_Msg::STDERR);
          ACE_LOG_MSG->msg_callback ( callback.get() );

          //register signals
          proc_signal_handler* handler = new proc_signal_handler();
          ACE_Reactor::instance()->register_handler( SIGINT ,  handler );
          ACE_Reactor::instance()->register_handler( SIGTERM ,  handler );

          ///bring up our protocpol stack
          int dw = ACE_Service_Config::open( argc , argv );
          if( dw != 0 )
          {
              ACE_ERROR_RETURN( ( LM_ERROR , "(%t) servicce config returned with errors..exiting\n") , 1 );
          }
          ACE_Reactor::instance()->run_reactor_event_loop();

}

