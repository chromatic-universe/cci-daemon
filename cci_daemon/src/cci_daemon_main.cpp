#pragma GCC diagnostic ignored "-Wunused-function"

#include <cci_daemon.h>

using namespace cdi;



//--------------------------------------------------------------------------
int main( int argc , char* argv[] )
{
    std::unique_ptr<daemon_facade> df ( new daemon_facade() );
    std::string str( "foo" );
    int options = 0;
    options |= LOG_PID;
    const char* identity = "cci-daemon";


    //daemonize
    if(  df->daemonize() == -1 )
    {
        std::set_terminate ( cdi::chromatic_terminate );

        syslog (LOG_USER | LOG_ERR , "%s", "Error in chromatic-daemon start...." );

        throw;
    }
    openlog( identity , options , LOG_USER );
    syslog ( LOG_USER , "%s", "daemonizing chromatic-daemon...." );
    closelog();

    df->daemon_default_exec( str , 0 );


    exit( EXIT_SUCCESS );



}
