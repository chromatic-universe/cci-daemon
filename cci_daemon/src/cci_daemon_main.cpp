#pragma GCC diagnostic ignored "-Wunused-function"

#include <cci_daemon.h>
#include <execinfo.h>
#include <cxxabi.h>

using namespace cdi;

//cmd line
static char* cmd_option( char ** begin, char ** end, const std::string & option );
static bool  cmd_option_exists( char** begin, char** end, const std::string& option );


//--------------------------------------------------------------------------
int main( int argc , char* argv[] )
{
            auto df( std::make_unique<cci_daemon_facade>() );
            df->backtrace( true );

            std::string str_default( "cci-daemon-default" );
            int options = 0;
            options |= LOG_PID;
            const char* identity = "cci-daemon";
            bool daemonize { true };

            if( cmd_option_exists( argv, argv+argc , "-a" ) )
            { df->proc( service_proc::sp_default_coordinator ); }
            else if( cmd_option_exists( argv, argv+argc , "-c" ) )
            { df->proc( service_proc::sp_custom_coordinator ); }
            //run from terminal
            if( cmd_option_exists( argv, argv+argc , "-t" ) )
            { daemonize = false; }

            //daemonize
            if( daemonize )
            {
                if(  df->daemonize() == -1 )
                {
                    std::set_terminate ( cdi::chromatic_terminate );
                    syslog (LOG_USER | LOG_ERR , "%s", "error in cci-daemon start...." );

                    throw;
                }
                openlog( identity , options , LOG_USER );
                syslog ( LOG_USER , "%s", "daemonized cci-daemon-dispatcher...." );
                closelog();

            }

            switch( df->proc() )
            {
                case service_proc::sp_default_coordinator  :
                    df->bootstrap_default_coordinator();
                    break;
                case service_proc::sp_custom_coordinator :
                    break;
                case service_proc::sp_default_service :
                default  :
                    df->daemon_default_exec( str_default , 0 );
                    break;
            }


            exit( EXIT_SUCCESS );



}

//--------------------------------------------------------------------------------
char* get_cmd_option( char ** begin , char ** end , const std::string & option)
{
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return 0;
}

//--------------------------------------------------------------------------------
bool cmd_option_exists( char** begin , char** end , const std::string& option )
{
    return std::find( begin , end , option ) != end;
}


