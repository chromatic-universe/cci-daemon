//cci_daemon_plugin.cpp    william k. johnson 2016
//

#include <iostream>
#include <cci_daemon_plugin.h>


using namespace cci_daemon_impl;


//------------------------------------------------------------------------------------
cci_daemon_plugin::cci_daemon_plugin( const std::string& filename ) :  m_shared_lib  { nullptr } ,                                                                                                                                              m_str_filename { filename } ,
                                                                       get_engine_version_address { nullptr } ,
                                                                       register_plugin_address { nullptr } ,
                                                                       clear_context_address { nullptr }
{

            m_shared_lib = cci_shared_lib::load( filename );
            assert( m_shared_lib );

            try
            {
                get_engine_version_address = get_function_pointer<get_engine_version_function>
                        ( m_shared_lib , "get_engine_version" );
                if( get_engine_version_address == nullptr )
                { std::cerr << "could not retrieve version function addresss\n";                }
                register_plugin_address = get_function_pointer<register_plugin_function>
                        ( m_shared_lib , "register_plugin" );
                if( register_plugin_address == nullptr )
                { std::cerr << "could not retrieve register function address\n"; }
                clear_context_address = get_function_pointer<clear_context_function>
                        ( m_shared_lib , "clear_context" );
                if( clear_context_address == nullptr )
                { std::cerr << "could not retrieve clear context function address\n"; }


            }
            catch( std::exception& )
            {
                cci_shared_lib::unload( m_shared_lib );

                std::cerr << "exception in plugin load....\n";

                throw std::runtime_error( "..could not load plugin..." );
            }

}

//------------------------------------------------------------------------------------
cci_daemon_plugin::cci_daemon_plugin( const cci_daemon_plugin& cdp )
{
          m_shared_lib = cdp.m_shared_lib;
          register_plugin_address =  cdp.register_plugin_address;
          get_engine_version_address = cdp.get_engine_version_address;
          clear_context_address = cdp.clear_context_address;
}

//------------------------------------------------------------------------------------
cci_daemon_plugin::~cci_daemon_plugin()
{

    //cci_shared_lib::unload( m_shared_lib );

}
