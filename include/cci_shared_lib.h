//cci_shared_lib.h      william k. johnson 2017


#pragma once


//c++ standard
#include <string>
#include <stdexcept>
#include <sstream>
#include <cassert>
//c runtime
#include <dlfcn.h>
#include <sys/stat.h>
//cci
#include <cci_daemon_utils.h>

namespace cci_daemon_impl
{

          //forward


          //enumerations



          //immutable
          //
          static const std::string ldd_moniker { "ldd" };
          static const std::string nm_moniker { "nm" };


          //aliases
          using cci_handle_t = void*;


          ///shared library loading and access
          class cci_shared_lib
          {

                ///loads the shared object from the specified path
                public :

                    //ctor
                    explicit cci_shared_lib()
                    {}
                    //ctor
                    virtual ~cci_shared_lib() = default;

                    //no copy
                    cci_shared_lib( const cci_shared_lib& chl ) = delete;
                    //no assign
                    cci_shared_lib& operator= ( const cci_shared_lib& chl ) = delete;

                private :

                    //attributes


                protected :

                    //

                public :

                    //accessors-inspectors

                    //services
                    ////shared object with the specified handle
                    static cci_handle_t load( const std::string& lib );
                    ////handle of the shared object that will be unloaded
                    static void unload( cci_handle_t shared_handle );
                    ///looks up a function exported by the sharedd object
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

                    //helpers
                    static bool file_exists( const std::string& file )
                    {
                        struct stat buf;
                        return ( stat( file.c_str() , &buf ) == 0 );
                    }

                    static std::string dependencies( const std::string& file_name  )
                    {
                        std::ostringstream ostr;
                        dependencies( file_name , ostr );

                        return ostr.str();
                    }

                    static void dependencies( const std::string& file_name , std::ostream& ostr  )
                    {
                        assert( file_exists( ldd_moniker ) );

                        try
                        {
                           //auto p = subprocess::Popen( { ldd_moniker.c_str() , file_name.c_str() } );
                           //auto obuf = p.communicate().first;

                           //ostr << obuf.buf.data();

                        }
                        catch( std::exception& e )
                        {
                            throw std::runtime_error( "could not retrieve library dependencies" );

                        }
                    }


          };

}


