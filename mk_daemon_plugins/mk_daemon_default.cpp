#include <mk_daemon_plugins.h>
#include <iostream>


SCY_PLUGIN(mk_default_plugin, "mk_daemon_default", "0.1.1")

//--------------------------------------------------------------------------------------
mk_default_plugin::mk_default_plugin()
{
    std::cout << "mk_default_plugin: create" << std::endl;
}

//--------------------------------------------------------------------------------------
mk_default_plugin::~mk_default_plugin()
{
    std::cout << "mk_default_plugin: destroy" << std::endl;
}

//--------------------------------------------------------------------------------------
void mk_default_plugin::execute()
{
    std::cout << "mk_default_plugin: execute" << std::endl;
}

//--------------------------------------------------------------------------------------
bool mk_default_plugin::on_command( const char* node , const char* data , unsigned int size )
{
    std::cout << "mk_default_plugin: command: " << node << ": " << data <<  "size:" << size << std::endl;
    // Process commands as required
    return true;
}
