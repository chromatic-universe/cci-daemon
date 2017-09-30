# cmake arm_cci_bootstrap  william k. johnson 2016


# version
cmake_minimum_required(VERSION 2.8)
# project
project(cci_bootstrap)


#cpp flags
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -g -rdynamic -O0 -DDEBUG -std=c++14")
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -rdynamic -O3 -DRELEASE -std=c++14")



# includes
include_directories($ENV{CCI_PRIVATE}/include/python3.6m 
                    $ENV{ACE_ROOT}
                    ../../../../include
                    ../include)
                    
# link
link_directories($ENV{CCI_PRIVATE}/lib
                 $ENV{DEV}/lib)
                
#binary
add_library(HA_python_bootstrap SHARED HA_python_bootstrap.cpp)

#link libs
target_link_libraries(HA_python_bootstrap -ldl 
                                          -lpthread
                                          -lpython3.6m
                                          -lACE)
#install
install_targets(/lib HA_python_bootstrap)



####
