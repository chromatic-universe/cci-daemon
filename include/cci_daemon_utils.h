//cci_daemon_utils.h      william k. johnson 2017


#pragma once


//c++ standard
#include <string>
#include <stdexcept>
#include <sstream>
#include <cassert>
#include <cstring>
#include <ctime>
//c runtime
#include <unistd.h>
#include <dlfcn.h>
#include <sys/stat.h>
#include <json.h>


namespace cci_daemon_utils
{

            //enumerations
            enum pipe_file_descriptros
            {
               read_fd  = 0,
               write_fd = 1
            };

            enum constants
            {
               buffer_size = 100
            };

            enum reading
            {
                blocking = 0 ,
                non_blocking = 1
            };

            /*
            extern "C"
            {
                //---------------------------------------------------------------------------
                static void buffer_stream_from_open( const std::string& params ,
                                                     const std::string& file_name ,
                                                     std::ostream& ostr ,
                                                     reading r ,
                                                     int& pid_ );

            }*/


}
