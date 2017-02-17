//cci_kernel_directives.h    william k. johnson 2017

#pragma once

//c++ std
#include <string>
#include <map>
#include <iostream>
#include <utility>
#include <sstream>
//c runtime
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//c++ contrib
#include <json.h>
#include <cci_daemon_kernel.h>


namespace cci_daemon_impl
{



        static size_t major_version   { 1 };
        static size_t minor_version   { 1 };
        static size_t release_version { 2 };
        static std::string version_string()
        {
                std::ostringstream ostr;
                ostr << major_version
                     <<"."
                     << minor_version
                     <<"."
                     << release_version;
                return ostr.str();
        }
        static std::string kernel_fifo{ "/etc/chromatic-universe/fifo" };
        static std::string supplicant_fifo( const std::string& supplicant )
        {
                 std::ostringstream ostr;
                 ostr << "/etc/chromatic-universe/supplicant-"
                      << supplicant;

                 return ostr.str();
        }
        static size_t supplicant_fifo_len { 64 };
}

