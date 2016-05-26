#pragma GCC diagnostic ignored "-Wunused-function"

#include <cci_daemon.h>

using namespace cdi;



//--------------------------------------------------------------------------
int main( int argc , char* argv[] )
{
    auto df( std::make_unique<cci_daemon_facade>() );

    std::string str( "cci-daemon-default" );
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

    df->daemon_default_exec( str , 0 );


    exit( EXIT_SUCCESS );



}
