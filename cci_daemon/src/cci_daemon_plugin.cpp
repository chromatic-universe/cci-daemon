//cci_daemon_plugin.cpp    william k. johnson 2016


#include <cci_daemon_plugin.h>


using namespace cci_daemon_impl;



//------------------------------------------------------------------------------------
cci_daemon_plugin::cci_daemon_plugin( const std::string& filename ) :  m_shared_lib  { nullptr } ,
                                                                       m_ref_count { nullptr }  ,
                                                                       m_str_filename { filename } ,
                                                                       get_engine_version_address { nullptr } ,
                                                                       register_plugin_address { nullptr }
{

            m_shared_lib = cci_shared_lib::load( filename );
            try
            {
                get_engine_version_address = cci_shared_lib::get_function_pointer<get_engine_version_function>
                        ( m_shared_lib , "get_engine_version" );
                register_plugin_address = cci_shared_lib::get_function_pointer<register_plugin_function>
                        ( m_shared_lib , "register_plugin" );


                m_ref_count = size_ptr_t( 1 );
            }
            catch( std::exception& )
            {
                cci_shared_lib::unload( m_shared_lib );

                throw;
            }

}

//------------------------------------------------------------------------------------
cci_daemon_plugin::cci_daemon_plugin( const cci_daemon_plugin& cdp ) : m_shared_lib { cdp.m_shared_lib } ,
                                                                       m_ref_count { cdp.m_ref_count } ,
                                                                       get_engine_version_address{ cdp.get_engine_version_address } ,
                                                                       register_plugin_address{ cdp.register_plugin_address }
{

            if( m_ref_count )
            {
                ++( *m_ref_count );
            }
}

//------------------------------------------------------------------------------------
cci_daemon_plugin::~cci_daemon_plugin()
{

            int remaining_refs = --*( m_ref_count );
            if( remaining_refs == 0 )
            {
                delete m_ref_count;
                cci_shared_lib::unload( m_shared_lib );
            }
}
