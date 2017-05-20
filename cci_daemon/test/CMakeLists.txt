#cmake cci-daemon test william k. johnson 2016

#version
cmake_minimum_required(VERSION 2.82)
#project
project(CCCIDAEMON)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
 
#cpp flags
set(CMAKE_CXX_FLAGS_DEBUG "-fPIC -Wall -g -O0  -Qunused-arguments  -DDEBUG -std=c++14")
set(CMAKE_CXX_FLAGS_RELEASE "-fPIC -Wall -O3  -Qunused-arguments  -DNDEBUG -std=c++14")

#includes
include_directories(../../include
                    $ENV{DEV}/include)
link_directories($ENV{DEV}/lib/cci
                 $ENV{DEV}/lib)

#executable
add_executable(cci_daemon_test_run  test_cci_daemon.cpp
                                    ../src/core/cci_daemon_engine.cpp
                                    ../src/kernel/cci_daemon_plugin.cpp
                                    ../src/kernel/cci_shared_lib.cpp
                                    ../src/kernel/cci_daemon_kernel.cpp
                                    ../src/core/cci_stack_trace.cpp )
add_executable(cci_shared_test_run  test_shared_lib.cpp
                                    ../src/core/cci_daemon_engine.cpp
                                    ../src/kernel/cci_daemon_plugin.cpp
                                    ../src/kernel/cci_daemon_kernel.cpp
                                    ../src/kernel/cci_shared_lib.cpp
                                    ../src/core/cci_stack_trace.cpp )

#link libs
target_link_libraries(cci_daemon_test_run  -lpthread -lgtest -lgtest_main -ldl)
target_link_libraries(cci_shared_test_run  -lpthread -ldl)


