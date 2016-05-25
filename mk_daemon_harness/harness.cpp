#include <scy/pluga/pluga.h>
#include <scy/sharedlibrary.h>
#include <mk_daemon_plugins.h>
#include <iostream>
#include <cassert>

using namespace scy;

int main(int argc, char** argv)
{
    // Set the plugin shared library location
    std::string path( "/dev_src/dev_src_git/mk_daemon_plugins/release/libmkdaemon-default" ); //;SCY_INSTALL_PREFIX);
    //path += "/dev_src/dev_src_git/mk_daemon_plugins/release/libmkdaemon-default";
#if WIN32
# ifdef _DEBUG
    path += "d.dll";
# else
    path += ".dll";
# endif
#else
    path += ".so";
#endif

    try {
        // Load the shared library
        std::cout << "Loading: " << path << std::endl;
        SharedLibrary lib;
        lib.open(path);

        // Get plugin descriptor and exports
        pluga::PluginDetails* info;
        lib.sym("exports", reinterpret_cast<void**>(&info));
        std::cout << "Plugin Info: "
            << "\n\tAPI Version: " << info->apiVersion
            << "\n\tFile Name: " << info->fileName
            << "\n\tClass Name: " << info->className
            << "\n\tPlugin Name: " << info->pluginName
            << "\n\tPlugin Version: " << info->pluginVersion
            << std::endl;

        // API Version checking
        if (info->apiVersion != SCY_PLUGIN_API_VERSION)
            throw std::runtime_error(
                util::format("Plugin ABI version mismatch. Expected %s, got %s.",
                    SCY_PLUGIN_API_VERSION, info->apiVersion));

        // Instantiate the plugin
        auto plugin = reinterpret_cast<pluga::IPlugin*>(info->initializeFunc());

        // Call plugin methods
        assert(plugin->on_command("some:command", "random:data", 11));

        // Close the plugin and free memory
        std::cout << "Closing" << std::endl;
        lib.close();
    }
    catch (std::exception& exc) {
        std::cerr << "Error: " << exc.what() << std::endl;
        assert(0);
    }

    return 0;
}
