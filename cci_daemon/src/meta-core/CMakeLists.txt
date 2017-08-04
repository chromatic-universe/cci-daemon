#cmake cci_gk  william k. johnson 2017

#version
cmake_minimum_required(VERSION 2.82)
#project
project(CCIDAEMON)


#cpp flags
set(CMAKE_CXX_FLAGS_DEBUG "-fPIC -Wall -g -O0  -rdynamic -Qunused-arguments  -DACE_NTRACE=0 -DDEBUG -std=c++1z")
set(CMAKE_CXX_FLAGS_RELEASE "-fPIC -Wall -O3  -rdynamic -Qunused-arguments  -DACE_NTRACE=1 -DNDEBUG -std=c++1z")

#includes
include_directories(../../../include
		    ./policies
                    $ENV{DEV}/include)
link_directories(${CCIDAEMON_SOURCE_DIR}
                    $ENV{DEV}/lib)


#executable
add_executable(cci-meta-daemon-dispatcher cci_meta_daemon.cpp 
                                	  cci_meta_daemon_main.cpp)
#link libs
target_link_libraries(cci-meta-daemon-dispatcher -lstdc++
                             	      		 -lpthread 
						 cci-daemon-core
			              		 -lACE
                                      		 -ldl) 

#install
install_targets( /bin/ cci-meta-daemon-dispatcher)


