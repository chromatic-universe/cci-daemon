#cmake cci_gk  william k. johnson 2017

#version
cmake_minimum_required(VERSION 2.82)
#project
project(CCIDAEMON)


#cpp flags
set(CMAKE_CXX_FLAGS_DEBUG "-fPIC -Wall -g -O0  -rdynamic -Qunused-arguments  -DDEBUG -std=c++14")
set(CMAKE_CXX_FLAGS_RELEASE "-fPIC -Wall -O3  -rdynamic -Qunused-arguments  -DNDEBUG -std=c++14")

#includes
include_directories(../../../include
                    $ENV{DEV}/include)
link_directories(${CCIDAEMON_SOURCE_DIR}
                    $ENV{DEV}/lib)

#executable
add_executable(cci-meta-daemon  cci_meta_daemon.cpp 
                                cci_meta_daemon_main.cpp)

#link libs
target_link_libraries(cci-meta-daemon -lstdc++
                             	      -lpthread 
			              -lACE
                                      -ldl) 

#install
install_targets( /bin/ cci-meta-daemon)
install (DIRECTORY ../include/ 
         DESTINATION include )


