//cci_daemon_base_kernel.cpp    william k. johnson 2019-2021
//

#include <iostream>
#include <cci_daemon_kernel.h>


using namespace cci_daemon_impl;
using json = nlohmann::json;

//static initializations
supported_dictionary cci_daemon_base_kernel::m_dict_supported =
{
     { "publish_subscribe" ,  "{ \"moniker\" : \"publish_and_subscribe\"" }
};

cci_daemon_kernel_ptr cci_daemon_kernel::m_instance;
std::mutex cci_daemon_kernel::m_mutex;


publish_and_subscribe_intf::~publish_and_subscribe_intf()
{}

//------------------------------------------------------------------------
cci_daemon_base_kernel::cci_daemon_base_kernel() : m_loaded_plugins( std::make_unique<plugin_dictionary>() )
{
    //
}
//------------------------------------------------------------------------
cci_daemon_base_kernel::~cci_daemon_base_kernel()
{
            //
}

//------------------------------------------------------------------------
void cci_daemon_base_kernel::load_plugin( const std::string &config )
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
void cci_daemon_base_kernel::unload_plugin( const std::string& config )
{
            //deregister
            auto iter = m_loaded_plugins->find( config );
            if( iter != m_loaded_plugins->end() )
            {
                std::cerr << "...deregistering plugin..."
                          << config
                          << "\n";
                std::cerr << "...releasing local contexts..\n";
                iter->second.clear_context( *this );
                m_loaded_plugins->erase( config );
            }
            else
            {
                 std::cerr << "...plugin not loaded...\n";
            }
}



//----------------------------------------------------------------------------
extern "C" int make_kernel( kernel_context_ptr context_ptr )
{ context_ptr->kernel_ref =  context_ptr->kernel_ref->instance();
  return 0;
}
//-----------------------------------------------------------------------------
extern "C" int unmake_kernel( kernel_context_ptr context_ptr  )
{ if( context_ptr->kernel_ref )
  { context_ptr->kernel_ref->dispose_instance(); }
  return 0;
}
