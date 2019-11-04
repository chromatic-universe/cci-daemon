//cci_test_daemon.h   william k. johnson 2016


//gtest
#include <gtest/gtest.h>
//cci
#include <cci_daemon.h>
#include <cci_daemon_generic.h>
#include <cci_shared_lib.h>
#include <cci_stack_trace.h>
#include <cci_daemon_kernel.h>
#include <cci_daemon_plugin.h>

using namespace cci_daemon_impl;

void try_open_server( publish_and_subscribe_server& pbs )
{
              try
              {
                    //this method will succeed only if the plugin is registered
                    //to the p & b server
                    std::unique_ptr<publish_and_subscribe_intf> pb = pbs.open_broker( "cci-aws-1:9092" ) ;
                    if( pb )
                    {
                        std::cerr << "...broker successfully opened..."
                                  << "pub_sub = "
                                  << pb->moniker()
                                  << "\n";
                    }
              }
              catch( const std::runtime_error &exception )
              {
                   std::cout << "...error opening server: "
                             << exception.what()
                             << "\n";
              }

}

int main( int argc , char* argv[] )
{

            try
            {
                
            	if( argc < 2 )
            	{
                	std::cerr << "...please specify library moniker...'\n";
                	_exit( 1 );
            	}
            	std::string moniker { argv[1] };


                std::string plug( moniker );
                auto cci_kernel = cci_daemon_kernel::instance();
                cci_kernel->load_plugin( plug );
                std::cerr << "...loaded plugin...."
                          << plug
                          << "\n";
                //try_open_server( cci_kernel->get_pb_server() );
                cci_kernel->unload_plugin( plug );

            }
            catch( std::runtime_error& e )
            {
                std::cerr << "...cci_daemon runtime error..."
                          << e.what()
                          << "\n";
            }
            catch( std::exception& e )
            {
                std::cerr << e.what()
                          << "\n";
            }

            cci_daemon_kernel::dispose_instance();


            return 0;
}


