//cci_daemon_kernel.cpp    william k. johnson 2017

#include <iostream>
#include <cci_daemon_kernel.h>


using namespace cci_daemon_impl;
using json = nlohmann::json;

supported_dictionary cci_daemon_kernel::m_dict_supported = { { "publish_subscribe" ,
                                           "{ \"moniker\" : \"publish_and_subscibe\"" } };

//------------------------------------------------------------------------
void cci_daemon_kernel::load_plugin( const std::string &config )
{

              if( m_loaded_plugins->find( config ) == m_loaded_plugins->end() )
              {
                   std::cerr << "loading plugin..."
                             << config
                             << "...\n";

                   cci_daemon_plugin cdp = cci_daemon_plugin( config );
                   auto iter = m_loaded_plugins->insert( std::make_pair( config , cdp ) );
                   //library instance specific , we're assigning
                   //our kernel instance for use in the library
                   //address space through function pointers
                   //--everyobdy handles their own memory management
                   //--see the constructorof the cci_dameon_plugin
                   iter.first->second.register_plugin( *this );
                   std::cerr << "...version is "
                             << iter.first->second.get_engine_version()
                             << "\n";
              }
              else
              {
                 std::cerr << "...plugin already loaded...\n";
              }

}

//--------------------------------------------------------------------------
void cci_daemon_kernel::unload_plugin( const std::string& config )
{
            //deregister
            if( m_loaded_plugins->find( config ) != m_loaded_plugins->end() )
            {
                std::cerr << "...deregistering plugin..."
                          << config
                          << "\n";
                std::cerr << "...releasing local contexts..\n";
                destroy_contexts();
                m_loaded_plugins->erase( config );
            }
            else
            {
                 std::cerr << "...plugin not loaded...\n";
            }
}


//--------------------------------------------------------------------------
void cci_daemon_kernel::perform()
{
            std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
}

