//cci_shared_lib.cpp    chromatic universe william k. johnson 2017
//


#include <iostream>
#include <cci_shared_lib.h>


using namespace cci_daemon_impl;


//--------------------------------------------------------------------------------
cci_handle_t cci_shared_lib::load( const std::string& lib )
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

            std::cerr << "...loaded library...."
                      << path_with_extension
                      << "...\n";




            return shared_object;
}

//--------------------------------------------------------------------------------
void  cci_shared_lib::unload( cci_handle_t shared_handle )
{
          if ( shared_handle ) { dlclose( shared_handle); }
}


