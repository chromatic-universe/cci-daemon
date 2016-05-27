//cci_daaemon_kernel.h     william k. johnson 2016

#pragma once

#include <string>
#include <map>

//cci
#include <cci_daemon_generic.h>
#include <cci_daemon_plugin.h>


namespace cci_daemon_impl
{

          //forward


          //aliases


          //enumerations

          /// <summary>The engine's core</summary>
          class cci_daemon_kernel
          {

            /// <summary>Map of plugins by their associated file names</summary>
            typedef std::map<std::string, Plugin> PluginMap;

            /// <summary>Accesses the storage server</summaryv
            public: MYENGINE_API StorageServer &getStorageServer() {
              return this->storageServer;
            }

            /// <summary>Accesses the graphics server</summary>
            public: MYENGINE_API GraphicsServer &getGraphicsServer() {
              return this->graphicsServer;
            }

            /// <summary>Loads a plugin</summary>
            /// <param name="filename">File the plugin will be loaded from</param>
            public: MYENGINE_API void loadPlugin(const std::string &filename) {
              if(this->loadedPlugins.find(filename) == this->loadedPlugins.end()) {
                this->loadedPlugins.insert(
                  PluginMap::value_type(filename, Plugin(filename))
                ).first->second.registerPlugin(*this);
              }
            }

            /// <summary>All plugins currently loaded</summary>
            private: PluginMap loadedPlugins;
            /// <summary>Manages storage-related tasks for the engine</summary>
            private: StorageServer storageServer;
            /// <summary>Manages graphics-related tasks for the engine</summary>
            private: GraphicsServer graphicsServer;

          };

}


