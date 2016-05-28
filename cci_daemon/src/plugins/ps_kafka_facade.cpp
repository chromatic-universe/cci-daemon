"

#include <stdexcept>

//cci
#include <cci_daemon_kernel.h>


namespace  cci_daemon_impl
{
          //aliases
          using pas_server = publish_and_subscribe_server;
          using pas_consumer = publish_and_subscribe_consumer;

          ///kafka consumer
          class  ps_kafka_facade : public pas_server::pas_consumer
          {

            public :

                //ctor
                explicit ps_kafka_facade() = default;
                //tor
                virtual ~ps_kafka_facade() = default;

                //services

                bool canOpenArchive(const string &filename) {
              return filename.find(".zip") == (filename.length() - 4);
            }

            /// <summary>Opens an archive for reading</summary>
            /// <param name="filename">Archive that will be opened</param>
            public: ZIPPLUGIN_API auto_ptr<Archive> openArchive(
              const string &sFilename
            ) {
              if(!canOpenArchive(sFilename)) {
                throw runtime_error("No Zip archive");
              }

              return auto_ptr<Archive>(new Archive());
            }

          };

          /// <summary>Retrieves the engine version we're expecting</summary>
          /// <returns>The engine version the plugin was built against</returns>
          extern "C" ZIPPLUGIN_API int getEngineVersion() {
            return 1;
          }

          /// <summary>Registers the plugin to an engine kernel</summary>
          /// <param name="kernel">Kernel the plugin will be registered to</param>
          extern "C" ZIPPLUGIN_API void registerPlugin(Kernel &kernel) {
            kernel.getStorageServer().addArchiveReader(
              auto_ptr<StorageServer::ArchiveReader>(new ZipArchiveReader())
            );
          }

}
