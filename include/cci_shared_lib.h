//cci_shared_lib.h      william k. johnson 2016


#pragma once

#include <string>
#include <stdexcept>
#include <sstream>
#include <dlfcn.h>

#include <cassert>

namespace cci_daemon_impl
{

          //forward


          //enumerations


          //aliases
          using cci_handle_t = void*;


          ///shared library loading and access
          class cci_shared_lib
          {

                ///loads the shared object from the specified path
                public :

                    //ctor
                    explicit cci_shared_lib() : m_str_libpath { "" }
                    {}
                    //ctor
                    virtual ~cci_shared_lib() = default;

                    //no copy
                    cci_shared_lib( const cci_shared_lib& chl ) = delete;
                    //no assign
                    cci_shared_lib& operator= ( const cci_shared_lib& chl ) = delete;

                private :

                    //attributes
                    std::string     m_str_libpath;

                protected :

                    //

                public :

                    //services
                    ////shared object with the specified handle
                    static cci_handle_t load( const std::string& lib )
                    {
                        std::ostringstream ostr;
                        ostr << "lib"
                             << lib
                             << ".so"
                             << std::ends;
                        std::string path_with_extension = ostr.str();
                        cci_handle_t shared_object = ::dlopen( path_with_extension.c_str() ,
                                                               RTLD_NOW) ;
                        if( shared_object == nullptr )
                        {
                            throw std::runtime_error( std::string( "could not load '")
                                                      + path_with_extension + "'");
                        }

                        return shared_object;
                    }

                    ////handle of the shared object that will be unloaded
                    static void unload( cci_handle_t shared_handle )
                    {
                          if ( shared_handle ) { dlclose( shared_handle); }
                    }

                    ///looks up a function exported by the shared object
                    ///handle of the shared object in which the function will be looked up
                    ///returns casted pointer to the specified function
                    template<typename T_signature>
                    static T_signature* get_function_pointer( cci_handle_t shared_handle ,
                                                              const std::string &function_name )
                    {
                         // clear error value
                         ::dlerror();

                          assert( shared_handle );

                          cci_handle_t function_address = ::dlsym( shared_handle ,
                                                          function_name.c_str() );

                          //check for error
                          const char *error = ::dlerror();
                          if( error != nullptr )
                          {
                            throw std::runtime_error( "could not find exported function" );
                          }

                      return reinterpret_cast<T_signature*>( function_address );
                    }

          };

}
