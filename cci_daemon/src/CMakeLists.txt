#cmake cci_daemon  william k. johnson 2016

#version
cmake_minimum_required(VERSION 2.82)
#project
project(CCIDAEMON)

add_subdirectory(plugins)
add_subdirectory(procs)
add_subdirectory(coordinator)
add_subdirectory(manager)
add_subdirectory(core)
add_subdirectory(kernel)
add_subdirectory(ccifs)
add_subdirectory(tornado)
add_subdirectory(meta-core)



