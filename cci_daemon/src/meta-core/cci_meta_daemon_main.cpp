//ccu_meta_daemon_main.cpp  william k. johnson  2017


#include <cci_meta_daemon.h>

using namespace cci_policy;

int main( int argc , char** argv )
{
	
	try
	{
		ACE_DEBUG(( LM_INFO , "%D (%P) cci-meta-daemon-dispatcher william k. johnson 2017\n" ) );
	
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
