//cci_daemon_generic.h    william k. johnson 2018

#pragma once
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-function"


//c++ standard
#include <cassert>
#include <string>
#include <dlfcn.h>
#include <sys/stat.h>


namespace cci_daemon_impl
{

	//aliases
	using  lib_handle_t = void*;

       	//-----------------------------------------------------------------------
        class publish_and_subscribe_intf
        {
            public :
                virtual std::string broker_spec() = 0;
                virtual std::string moniker() const noexcept = 0;
                virtual bool gen_endpoint( std::string& endpoint_prefix ) = 0;
                virtual bool enumerate_endpoints( std::vector<std::string>& endpoints );
                virtual ~publish_and_subscribe_intf();
        };


	    //generic
        //looks up a function exported by the sharedd object
        ///handle of the shared object in which the function will be looked up
        ///returns casted pointer to the specified function
        template<typename T_signature>
        static T_signature* get_function_pointer( lib_handle_t shared_handle ,
                                                  const std::string &function_name )
        {
                         // clear error value
                         ::dlerror();

                          assert( shared_handle );

                          lib_handle_t function_address = ::dlsym( shared_handle ,
                                                          function_name.c_str() );

                          //check for error
                          const char *error = ::dlerror();
                          if( error != nullptr )
                          {
                            throw std::runtime_error( "could not find exported function" );
                          }

                          return reinterpret_cast<T_signature*>( function_address );
         }

}
