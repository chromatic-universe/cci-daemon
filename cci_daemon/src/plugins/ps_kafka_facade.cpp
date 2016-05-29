

#include <stdexcept>
#include <exception>
//cci
#include <cci_daemon_kernel.h>


namespace  cci_daemon_impl
{
          //aliases
          using pas_consumer = publish_and_subscribe_server::publish_and_subscribe_consumer;
          using pas_server =  publish_and_subscribe_server;

          ///kafka consumer
          class  ps_kafka_facade : public pas_consumer ,
                                   public std::enable_shared_from_this<ps_kafka_facade>
          {

            public :

                //ctor
                explicit ps_kafka_facade() = default;
                //dtor
                virtual ~ps_kafka_facade() = default;

                //services
                bool can_open_broker ( const std::string& config )
                {  return true; }

                std::unique_ptr<publish_and_subscribe> open_broker( const std::string& config )
                {
                  if( !can_open_broker( config ))
                  {  throw std::runtime_error ( "broker cannnot be opened" ); }

                  return std::make_unique<publish_and_subscribe>();
                }

          };

          //exports
          extern "C" int get_engine_version() { return 1; }
          extern "C" void register_plugin( cci_daemon_kernel& kernel )
          {
               pas_consumer* pas = new ps_kafka_facade();
               kernel.get_pb_server().add_publish_subscribe_consumer( pas );
               std::cerr << "registered plugin....\n";

          }
          extern "C" void clear_context( cci_daemon_kernel& kernel )
          {
               std::cerr << "clear context....\n";

               pas_server::consumer_list* cl = kernel.get_pb_server().consumers();
               if( cl ) { std::cerr << "clearing publish and subscribe context\n"; }
          }

}
