#cmake cci_daemon  william k. johnson 2017

#version
cmake_minimum_required(VERSION 2.82)
#project
project(CCIDAEMON)


#cpp flags
set(CMAKE_CXX_FLAGS_DEBUG "-fPIC -Wall -g -O0  -rdynamic -Qunused-arguments  -DDEBUG -std=c++1z")
set(CMAKE_CXX_FLAGS_RELEASE "-fPIC -Wall -O3  -rdynamic -Qunused-arguments  -DNDEBUG -std=c++1z")

#includes
include_directories(../../../include
                    $ENV{DEV}/include
                    ../cpr/include
                    $ENV{DEV}/include/cci)
link_directories(${CCIDAEMON_SOURCE_DIR}
                    $ENV{DEV}/lib
                    $ENV{DEV}/lib/cci)

#executable
add_executable(cci-daemon-dispatcher   cci_daemon_main.cpp)

#link libs
target_link_libraries(cci-daemon-dispatcher  -lstdc++
                                             -lpthread 
					     cci-daemon-core 
                                             -ldl) 

#install
install_targets( /bin/ cci-daemon-dispatcher)
install (DIRECTORY ../../../include/ 
         DESTINATION include/cci )


