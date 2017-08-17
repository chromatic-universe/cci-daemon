//cci_meta_daemon_main.cpp  william k. johnson  2017


#include <cci_meta_daemon.h>

using namespace cci_policy;

int main( int argc , char** argv )
{
	ACE_TRACE("cci_meta_daemon_main.cpp  william k. johnson  2017 : main");

	
	try
	{
		
		ACE_DEBUG(( LM_INFO , "%D (%P) cci-meta-daemon-dispatcher william k. johnson 2017\n" ) );
		
		/*daemon_signal_handler h1(SIGTERM);
		daemon_signal_handler h2(SIGINT);
		//attributes
		ACE_Sig_Handlers handler;
		//signals
		ACE_DEBUG( ( LM_INFO , "%D (%P) ...registering signals....\n" ) );
		handler.register_handler (SIGTERM, &h1);
		handler.register_handler (SIGINT,  &h2);*/
		
		auto holder = std::make_unique<placeholder>(); 
		auto json_str = std::make_unique<std::string> ( "{}" );
		auto cci = std::make_unique<default_daemon_dispatcher>( holder.get() );
		//config
		cci->runtime_data( json_str );
		//daemonize
	        cci->daemonize();	
	}
	catch( std::exception& e )	
	{
		ACE_DEBUG(( LM_ERROR , "%D (%P) cci-meta-daemon-dispatcher exception...%s." ,
        		        e.what() ) );
	}

	return 0;
}
