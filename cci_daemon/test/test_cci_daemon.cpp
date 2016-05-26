//test_cci_daemon.cpp   william k. johnson 2016


//cci
#include <cci_test_daemon.h>

using namespace cci_daemon_impl;

int main( int argc , char **argv )
{

    ::testing::InitGoogleTest( &argc , argv );

    return RUN_ALL_TESTS();
}
