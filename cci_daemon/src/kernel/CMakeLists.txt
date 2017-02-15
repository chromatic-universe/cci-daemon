#cmake cci_daemon  william k. johnson 2017

#version
cmake_minimum_required(VERSION 2.82)
#project
project(CCIDAEMON)


#cpp flags
set(CMAKE_CXX_FLAGS_DEBUG "-fPIC -Wall -g -O0  -rdynamic -Qunused-arguments  -DDEBUG -std=c++14")
set(CMAKE_CXX_FLAGS_RELEASE "-fPIC -Wall -O3  -rdynamic -Qunused-arguments  -DNDEBUG -std=c++14")

#includes
include_directories(../../../include
                    $ENV{DEV}/include
                    ./include
                    $ENV{DEV}/include/cci)
link_directories(${CCIDAEMON_SOURCE_DIR}
                    $ENV{DEV}/lib
                    $ENV{DEV}/lib/cci)

#executable
add_library(cci_daemon_kernel   SHARED cci_daemon_kernel.cpp 
                                       cci_daemon_plugin.cpp 
                                       cci_daemon_utils.cpp
                                       cci_shared_lib.cpp
                                       )
add_library(HA_daemon_kernel  SHARED   cci_daemon_kernel.cpp 
                                       cci_daemon_plugin.cpp 
                                       cci_daemon_utils.cpp
                                       cci_shared_lib.cpp
                                       HA_kernel_dynamic.cpp
                                       )
                                   

#link libs
target_link_libraries(cci_daemon_kernel  -lstdc++
                                         -lpthread 
                                         -ldl)
set_target_properties(cci_daemon_kernel PROPERTIES
                                        VERSION 1.1.2
                                        SOVERSION 1)                                                   

#install
install_targets( /lib/ cci_daemon_kernel)
install_targets( /lib/ HA_daemon_kernel)



