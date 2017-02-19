
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
                 $ENV{DEV}/lib/cci
                 $ENV{DEV}/lib)

#library
if(WITH_ACE_AUTH)
    
    add_library(HA_auth_mgr_acceptor SHARED  HA_manager_protocol_dynamic.cpp 
                                            auth_mgr_acceptor.cpp
                                            auth_mgr_handler.cpp
                                            auth_mgr_thread_pool.cpp)

    #link libs
    target_link_libraries(HA_auth_mgr_acceptor -lstdc++ 
                                               -lpthread 
                                               -ldl
                                               -lACE)

    #install
    install_targets( /lib/cci HA_auth_mgr_acceptor)
endif()

