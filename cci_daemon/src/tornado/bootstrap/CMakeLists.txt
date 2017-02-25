# cmake arm_cci_bootstrap  william k. johnson 2016


# version
cmake_minimum_required(VERSION 2.8)
# project
project(arm_cci_bootstrap)


#cpp flags
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -g -rdynamic -O0 -DDEBUG -std=c99")
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -rdynamic -O9 -DRELEASE -std=c99")



# includes
include_directories($ENV{CCI_PRIVATE}/include/python3.6m 
                    ${CPR_INCLUDE_DIRS})

# link
link_directories($ENV{CCI_PRIVATE}/lib)
                
#binary
add_executable(cci-bootstrap cci_bootstrap.c)

#link libs
target_link_libraries(cci-bootstrap -lpython3.6m)
#install
install_targets(/bin cci-bootstrap)

