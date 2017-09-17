//cci_kernel_directives.h    william k. johnson 2017


#pragma once



//c++ std
#include <string>
#include <map>
#include <iostream>
#include <utility>
#include <sstream>
#include <set>

//c runtime
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//c++ contrib
#include <json.hpp>
#include <cci_daemon_kernel.h>


namespace cci_daemon_impl
{

	class kernel_directives
	{
		public :

			virtual size_t major_version() = 0;
			virtual size_t minor_version() = 0;
			virtual size_t release_version() = 0;
			virtual std::string version_string() = 0;
			virtual std::set<std::string> kernel_strings() = 0;			
	};


	class cci_kernel_directives : public kernel_directives
	{
		public :
			
			virtual size_t major_version() override { return 1; };
			virtual size_t minor_version() override { return 1; };
			virtual size_t release_version() override { return 2; };
			virtual std::string version_string() override
			{
				std::ostringstream ostr;
				ostr << major_version()
				     <<"."
				     << minor_version()
				     <<"."
				     << release_version();
				return ostr.str();

			}
			virtual std::set<std::string> kernel_strings() override
			{
				 std::set<std::string> kernel_strings = {  "make_kernel" ,
									   "unnmake_kernel" ,
									   "mount_memroy_cache" ,
									   "unmount_memory_cache"
									};
				return kernel_strings;
			}
	};
	static cci_kernel_directives cci_k_directives;
		

	
}


