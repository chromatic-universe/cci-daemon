#cmake cci_daemon  william k. johnson 2020

#version
cmake_minimum_required(VERSION 3.2)
#project
project(CCIDAEMON)


#cpp flags
set(CMAKE_CXX_FLAGS_DEBUG "-fPIC -Wall -g -O0  -rdynamic -Qunused-arguments  -DDEBUG -std=c++1z")
set(CMAKE_CXX_FLAGS_RELEASE "-fPIC -Wall -O3  -Qunused-arguments  -DNDEBUG -std=c++1z")

#includes
include_directories(../../../include
                    $ENV{DEV}/include
                    ../cpr/include
                    $ENV{DEV}/include/cci)
link_directories(${CCIDAEMON_SOURCE_DIR}
                    $ENV{DEV}/lib
                    $ENV{DEV}/lib/cci)

#executable
add_library(cci-daemon-core SHARED  cci_stack_trace.cpp 
                                    cci_daemon.cpp 
                                    cci_daemonize.cpp
                                    cci_daemon_engine.cpp)
set_target_properties(cci-daemon-core PROPERTIES
                                      VERSION 1.0.0
                                      SOVERSION 1)   

#install
install_targets( /lib/ cci-daemon-core)
install (DIRECTORY ../../../include/ 
         DESTINATION include/cci )


