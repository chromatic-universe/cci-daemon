//cci_daemon_bootstrap.cpp  chromatic unvierse william k. johnson 2017


#include <cci_daemon_bootstrap.h>
#include <proc_ace.h>


//signal handles
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

                            ACE_DEBUG(( LM_INFO , "...bootstrap sigint received...\n" ) );

                            break;
                    }

                    return 0;
                }
};


//coordinator
extern "C" void bootstrap_default_coordinator( const std::string& params )
{
        syslog ( LOG_USER | LOG_INFO | LOG_PID ,
                "%s",
                "default cci-dispatcher coordinator initializing...." );
}

