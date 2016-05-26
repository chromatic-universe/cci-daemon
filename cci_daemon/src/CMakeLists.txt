#cmake cci_daemon  william k. johnson 2016

#version
cmake_minimum_required(VERSION 2.82)
#project
project(CCIDAEMON)


#cpp flags
set(CMAKE_CXX_FLAGS_DEBUG "-fPIC -Wall -g -O0  -Qunused-arguments  -DDEBUG -std=c++14")
set(CMAKE_CXX_FLAGS_RELEASE "-fPIC -Wall -O3  -Qunused-arguments  -DNDEBUG -std=c++14")

#includes
include_directories(../../include)
link_directories(${CCIDAEMON_SOURCE_DIR})

#executable
add_executable(cci-daemon  cci_daemon.cpp 
                           cci_daemon_main.cpp )

#link libs
target_link_libraries(cci-daemon -lstdc++ -lpthread)

#install
install_targets( /bin cci-daemon)

