//cci_stack_trace.h    william k. johnson 2016

#pragma once

#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <execinfo.h>
#include <cxxabi.h>

#include <iostream>
#include <sstream>
#include <vector>

namespace cci_daemon_impl
{
          //demangled stack backtrace of the caller function to out stream
          void print_stacktrace( FILE *out = stderr, unsigned int max_frames = 63 );

}
