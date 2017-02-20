
#cpp flags
set(CMAKE_CXX_FLAGS_DEBUG "-fPIC -Wall -g -O0  -rdynamic -Qunused-arguments  -DDEBUG -std=c++14")
set(CMAKE_CXX_FLAGS_RELEASE "-fPIC -Wall -O3  -rdynamic -Qunused-arguments  -DNDEBUG -std=c++14")

#includes
include_directories(../../../include
                    $ENV{DEV}/include
                    $ENV{DEV}/include/cci)
link_directories(${CCIDAEMON_SOURCE_DIR}
                 $ENV{DEV}/lib/cci)

#library
add_library(ps_kafka_facade SHARED ps_kafka_facade.cpp)

#link libs
target_link_libraries(ps_kafka_facade -lstdc++ 
                                      -lpthread 
                                      -ldl)

#install
install_targets( /lib ps_kafka_facade)

