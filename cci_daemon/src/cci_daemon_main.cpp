#pragma GCC diagnostic ignored "-Wunused-function"

#include <cci_daemon.h>
#include <execinfo.h>
#include <cxxabi.h>
using namespace cdi;



//--------------------------------------------------------------------------
int main( int argc , char* argv[] )
{
            auto df( std::make_unique<cci_daemon_facade>() );
            df->backtrace( true );

            std::string str_default( "cci-daemon-default" );
            int options = 0;
            options |= LOG_PID;
            const char* identity = "cci-daemon";


            //daemonize
            if(  df->daemonize() == -1 )
            {
                std::set_terminate ( cdi::chromatic_terminate );

                syslog (LOG_USER | LOG_ERR , "%s", "error in cci-daemon start...." );

                throw;
            }
            openlog( identity , options , LOG_USER );
            syslog ( LOG_USER , "%s", "daemonizing cci-daemon...." );
            closelog();

            switch( df->proc() )
            {
                case service_proc::sp_default_coordinator  :
                    df->bootstrap_coordinator();
                    break;
                case service_proc::sp_custom_coordinator :
                    break;
                case service_proc::sp_default_service :
                default  :
                    df->daemon_default_exec( str_default , 0 );
            }


            exit( EXIT_SUCCESS );



}
