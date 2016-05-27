//cci_test_daemon.h   william k. johnson 2016


//gtest
#include <gtest/gtest.h>
//cci
#include <cci_daemon.h>
#include <cci_daemon_generic.h>
#include <cci_shared_lib.h>
#include <cci_stack_trace.h>

using namespace cci_daemon_impl;


int main( int argc , char* argv[] )
{
    try
    {
        auto cci_shared( std::make_unique<cci_shared_lib>() ) ;
        cci_shared_t st = cci_shared->load( "cci_expansion" );
    }
    catch( std::runtime_error& e )
    {
        std::cerr << e.what() << "\n";
    }

    return 0;
}

