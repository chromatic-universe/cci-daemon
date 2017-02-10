//proc_ace_harness.cpp chromatic universe william k. johnson 2017


#include <proc_ace_acceptor.h>
#include "ace/OS_main.h"

using namespace proc_ace;

unsigned long g_dw_finished = 0L;

//log callback
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
class proc_signal_handler : public ACE_Event_Handler
{
            public:

                proc_signal_handler() : ACE_Event_Handler()
                {}

                int handle_signal ( int signum ,
                                    siginfo_t * = 0 ,
                                    ucontext_t * = 0 )
                {

    	            ACE_Trace _( ACE_TEXT( "proc_signal_handler" ) , __LINE__ );

                    switch( signum )
                    {
                        case SIGINT :

                            ACE_DEBUG(( LM_INFO , "...cci-dameon-dispatcher oprotocol stack..sigint received..\n" ) );
                            g_dw_finished = 1L;

                            exit( 1 );
                    }

                    return 0;
                }
};


int ACE_TMAIN ( int argc , ACE_TCHAR * argv[] )
{


              auto callback = std::make_unique<log_callback>();
              ACE_LOG_MSG->set_flags (ACE_Log_Msg::MSG_CALLBACK);
              ACE_LOG_MSG->clr_flags (ACE_Log_Msg::STDERR);
              ACE_LOG_MSG->msg_callback ( callback.get() );

              //register signals
              proc_signal_handler* handler = new proc_signal_handler();
              ACE_Reactor::instance()->register_handler( SIGINT ,  handler );


              ///bring up our protocpol stack
              int dw = ACE_Service_Config::open( argc , argv );
              if( dw != 0 )
              {
                  ACE_ERROR_RETURN( ( LM_ERROR , "(%t) servicce config returned with errors..exiting\n") , 1 );
              }
              ACE_Reactor::instance()->run_reactor_event_loop();


              /*/fini
              ACE_Thread_Manager::instance()->cancel_all();
              ACE_Time_Value timeout( 5 );
              if ( ACE_Thread_Manager::instance()->wait( &timeout , 1 ) == -1 )
              {
                  ACE_ERROR_RETURN( ( LM_ERROR , "(%t) wait() failed\n") , 1 );
              }

               ACE_DEBUG ((LM_DEBUG,
                          "(%P|%t) shutting down ::cci-dispatch-daemon::proc_ace daemon\n"));

              */

              return 0;

}





