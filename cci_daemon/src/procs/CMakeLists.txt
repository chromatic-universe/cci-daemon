#cmake cci daemon server proc library  william k. johnson 2017

#version
cmake_minimum_required(VERSION 2.82)
#project
project(PROCACEACCEPTOR)


#cpp flags
set(CMAKE_CXX_FLAGS_DEBUG "-fPIC -Wall -g -O0  -rdynamic -Qunused-arguments  -DDEBUG -std=c++14")
set(CMAKE_CXX_FLAGS_RELEASE "-fPIC -Wall -O3  -rdynamic -Qunused-arguments  -DNDEBUG -std=c++14")

#includes
include_directories(../../../include
                    $ENV{DEV}/include
                    $ENV{ACE_ROOT})
link_directories(${PROCACEACCEPTOR_SOURCE_DIR}
                 $ENV{DEV}/lib/cci
                 $ENV{DEV}/lib)


#executable
add_library(proc_ace_acceptor SHARED proc_ace_acceptor.cpp
                                     proc_ace_thread_pool
                                     proc_ace_handler)


#link libs
target_link_libraries(proc_ace_acceptor -lstdc++ 
                                        -lpthread 
                                        -ldl
                                        -lACE)

#install
install_targets( /lib/cci proc_ace_acceptor)

