#cmake cci-daemon test william k. johnson 2016

#version
cmake_minimum_required(VERSION 2.82)
#project
project(CCCIDAEMON)

#cpp flags
set(CMAKE_CXX_FLAGS_DEBUG "-fPIC -Wall -g -O0  -Qunused-arguments  -DDEBUG -std=c++14")
set(CMAKE_CXX_FLAGS_RELEASE "-fPIC -Wall -O3  -Qunused-arguments  -DNDEBUG -std=c++14")

#includes
include_directories(../../include
                    $ENV{DEV}/include)

#executable
add_executable(cci_daemon_test_run  test_cci_daemon.cpp
                                    ../src/cci_daemon_engine.cpp
                                    ../src/cci_daemon_plugin.cpp
                                    ../src/cci_stack_trace.cpp )
add_executable(cci_shared_test_run  test_shared_lib.cpp
                                    ../src/cci_daemon_engine.cpp
                                    ../src/cci_daemon_plugin.cpp
                                    ../src/cci_stack_trace.cpp )

#link libs
target_link_libraries(cci_daemon_test_run  -lpthread -lgtest -lgtest_main -ldl)
target_link_libraries(cci_shared_test_run  -lpthread -ldl)


