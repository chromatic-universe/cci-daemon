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
        pbs.open_broker( "cci-aws-1:9092" ) ;

        std::cout << "\tbroker successfully opened" << std::endl;
      }
      catch(const std::exception &exception)
      {
          std::cout << "\terror opening server: " << exception.what() << std::endl;
      }

      std::cout << std::endl;
}

int main( int argc , char* argv[] )
{
    auto cci_kernel( std::make_unique<cci_daemon_kernel>() );

    try_open_server( cci_kernel->get_pb_server() );
    /*
    {
        auto cci_shared( std::make_unique<cci_shared_lib>() ) ;
        cci_handle_t st = cci_shared->load( "cci_expansion" );
    }
    catch( std::runtime_error& e )
    {
        std::cerr << e.what() << "\n";
    }*/

    return 0;
}

