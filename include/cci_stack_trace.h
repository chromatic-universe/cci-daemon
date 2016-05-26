//cci_stack_trace.h    william k. johnson 2016

#pragma once
#pragma GCC diagnostic ignored "-Wformat"

#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <execinfo.h>
#include <dlfcn.h>
#include <cxxabi.h>

#include <iostream>
#include <sstream>
#include <vector>


namespace cci_daemon_impl
{

          //demangled stack backtrace of the caller function to out stream
          extern void print_stacktrace( FILE *out = stderr, unsigned max_frames = 63 );
          //back trace walk
          extern void** get_ebp( int dummy );
          extern void print_walk_backtrace( FILE *out = stderr);

}
