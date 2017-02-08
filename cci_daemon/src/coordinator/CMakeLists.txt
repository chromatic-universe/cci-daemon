
#cpp flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -fPIC -Wall -g -O0  -rdynamic -Qunused-arguments  -DDEBUG -std=c++14")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -fPIC -Wall -O3  -rdynamic -Qunused-arguments  -DNDEBUG -std=c++14")

#includes
include_directories(../../../include
                    $ENV{DEV}/include
                    ./include
                    $ENV{DEV}/include/cci
                    $ENV{ACE_ROOT})
link_directories(${CCIDAEMON_SOURCE_DIR}
                 $ENV{DEV}/lib/cci)

#library
if(WITH_ACE_COORDINATOR)
    add_library(cci_daemon_bootstrap SHARED cci_daemon_bootstrap.cpp)

    #link libs
    target_link_libraries(cci_daemon_bootstrap -lstdc++ 
                                               -lpthread 
                                               -ldl
                                               -lcci_expansion
                                               -lACE)

    #install
    install_targets( /lib/cci cci_daemon_bootstrap)
endif()

