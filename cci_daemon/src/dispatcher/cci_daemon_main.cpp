#pragma GCC diagnostic ignored "-Wunused-function"

#include <cci_daemon.h>
#include <execinfo.h>
#include <cxxabi.h>
#include <linux/limits.h>

using namespace cdi;

//cmd line
static char* cmd_option( char ** begin, char ** end, const std::string & option );
static bool  cmd_option_exists( char** begin, char** end, const std::string& option );


//--------------------------------------------------------------------------
int main( int argc , char* argv[] )
{
            cci_daemon_facade_ptr df  = nullptr;

            //system( "ccifs /dev_cm /cci/dev_t/data" );

            try
            {
                df = new cci_daemon_facade( argc , argv );
                df->backtrace( false );
                char working_dir[PATH_MAX];
                df->chroot_dir( ::getcwd( working_dir , PATH_MAX ) );

                std::string str_default( "cci-daemon-default" );
                int options = 0;
                options |= LOG_PID;
                const char* identity = "cci-daemon";
                bool daemonize { true };

                //run protocol stack , else run default daemon exec
                if( cmd_option_exists( argv, argv+argc , "-a" ) )
                { df->proc( service_proc::sp_default_coordinator ); }
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
                        df->bootstrap_coordinator();
                        break;
                    case service_proc::sp_default_service :
                    default  :
                        df->daemon_default_exec( str_default , 0 );
                        break;
                }
            }
            catch( std::exception err )
            {
                if( df ) { delete df; }
            }

            if( df ) { delete df; }


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


