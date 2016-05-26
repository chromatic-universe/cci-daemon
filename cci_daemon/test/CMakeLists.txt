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
add_executable(cci_daemon_test_run  test_cci_daemon.cpp)

#link libs
target_link_libraries(cci_daemon_test_run  -lpthread -lgtest -lgtest_main)


