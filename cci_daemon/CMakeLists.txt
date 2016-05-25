#cmake cci_daemon  william k. johnson 2016

#version
cmake_minimum_required(VERSION 2.82)
#project
project(CCIDAEMON)

#tests
add_subdirectory(src)

#cpp flags
set(CMAKE_CXX_FLAGS_DEBUG "-fPIC -Wall -g -O0  -Qunused-arguments  -DDEBUG -std=c++14")
set(CMAKE_CXX_FLAGS_RELEASE "-fPIC -Wall -O3  -Qunused-arguments  -DNDEBUG -std=c++14")


