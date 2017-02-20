#cmake cci_daemon  william k. johnson 2017

#version
cmake_minimum_required(VERSION 2.82)
#project
project(CCIDAEMON)


#cpp flags
set(CMAKE_C_FLAGS "-D_FILE_OFFSET_BITS=64")
set(CMAKE_CXX_FLAGS_DEBUG "-fPIC -Wall -g -O0  -rdynamic -Qunused-arguments  -D_FILE_OFFSET_BITS=64 -DDEBUG -std=c++14")
set(CMAKE_CXX_FLAGS_RELEASE "-fPIC -Wall -O3  -rdynamic -Qunused-arguments  -D_FILE_OFFSET_BITS=64 -DNDEBUG -std=c++14")

#includes
include_directories(../../../include
                    $ENV{DEV}/include
                    ./include
                    $ENV{DEV}/include/cci)
link_directories(${CCIDAEMON_SOURCE_DIR}
                    $ENV{DEV}/lib
                    $ENV{DEV}/lib/cci)

#executable
add_library(HA_ccifs SHARED HA_ccifs_dynamic.cpp)

#link libs
target_link_libraries(HA_ccifs -lstdc++ 
                               -lpthread 
                               -ldl
                               -lACE)
                         

#install
install_targets( /lib HA_ccifs)



