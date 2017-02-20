#cmake cci daemon server proc library  william k. johnson 2017

#version
cmake_minimum_required(VERSION 2.82)
#project
project(PROCACEACCEPTOR)


#cpp flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -fPIC -Wall -g -O0  -rdynamic -Qunused-arguments  -DDEBUG -std=c++14")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -fPIC -Wall -O3  -rdynamic -Qunused-arguments  -DNDEBUG -std=c++14")

#includes
include_directories(../../../include
                    $ENV{DEV}/include
                    $ENV{ACE_ROOT})
link_directories(${PROCACEACCEPTOR_SOURCE_DIR}
                 $ENV{DEV}/lib/cci
                 $ENV{DEV}/lib)


#library
add_library(proc_ace_acceptor SHARED proc_ace_acceptor.cpp
                                     proc_ace_thread_pool.cpp
                                     proc_ace_handler.cpp)
set_target_properties(proc_ace_acceptor PROPERTIES
                                        VERSION 1.0.0
                                        SOVERSION 1)                                 

#executable harness
add_executable(proc-ace-harness proc_ace_thread_pool.cpp
                                proc_ace_handler.cpp
                                proc_ace_harness.cpp) 


#link libs
target_link_libraries(proc_ace_acceptor -lstdc++ 
                                        -lpthread 
                                        -ldl
                                        -lACE)
target_link_libraries(proc-ace-harness  -lstdc++ 
                                        -lpthread 
                                        -ldl
                                        -lACE)

                                    

#install
install_targets( /lib proc_ace_acceptor)
install_targets( /bin proc-ace-harness)

