//ccu_meta_daemon_main.cpp  william k. johnson  2017


#include <cci_meta_daemon.h>

using namespace cci_policy;

int main( int argc , char** argv )
{
	
	try
	{
		std::cerr << "cci-meta-daemon-dispatcher william k. johnson 2017"
		  	  << "\n";	
		auto cci = std::make_unique<default_daemon_dispatcher>();
		//daemonize
		cci->daemonize();
	        		
	}
	catch( std::exception& e )	
	{
		std::cerr << e.what()
			  << "\n";
	}

	return 0;
}
