#cmake cci_daemon  william k. johnson 2016

#version
cmake_minimum_required(VERSION 2.82)
#project
project(CCIDAEMON)

add_subdirectory(plugins)
add_subdirectory(procs)

#cpp flags
set(CMAKE_CXX_FLAGS_DEBUG "-fPIC -Wall -g -O0  -rdynamic -Qunused-arguments  -DDEBUG -std=c++14")
set(CMAKE_CXX_FLAGS_RELEASE "-fPIC -Wall -O3  -rdynamic -Qunused-arguments  -DNDEBUG -std=c++14")

#includes
include_directories(../../include
                    $ENV{DEV}/include
                    $ENV{DEV}/include/cci)
link_directories(${CCIDAEMON_SOURCE_DIR}
                    $ENV{DEV}/lib
                    $ENV{DEV}/lib/cci)

#executable
add_executable(cci-daemon-dispatcher   cci_stack_trace.cpp 
                                       cci_daemon.cpp 
                                       cci_daemon_plugin.cpp
                                       cci_daemon_main.cpp
                                       cci_daemon_utils.cpp
                                       cci_daemon_engine.cpp)

#link libs
target_link_libraries(cci-daemon-dispatcher  -lstdc++
                                             -lpthread 
                                             -ldl 
                                             -lcci_expansion)

#install
install_targets( /bin/ cci-daemon-dispatcher)
install (DIRECTORY ../../include/ 
         DESTINATION include/cci )
install(FILES  ${CMAKE_SOURCE_DIR}/ldd
        PERMISSIONS WORLD_EXECUTE WORLD_WRITE WORLD_READ
        DESTINATION bin)


