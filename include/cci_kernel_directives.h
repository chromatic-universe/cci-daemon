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


static std::string kernel_fifo{ "/etc/chromatic-universe/fifo" };
static std::string supplicant_fifo( const std::string& supplicant )
{
         std::ostringstream ostr;
         ostr << "/etc/chromatic-universe/supplicant-"
              << supplicant;

         return ostr.str();
}
static size_t supplicant_fifo_len { 64 };

