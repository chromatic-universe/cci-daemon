
//cci_pub_and_sub.h      william k. johnson 2016

#include <list>
#include <string>
#include <memory>
#include <stdexcept>
#include <memory>

namespace cci_daemon_impl
{

          //forward declarations
          //placeholder
          class publish_and_subscribe{};

          ///publish-subscribe related domain
          class publish_and_subscribe_server
          {

             public:

                ///
                class publish_and_subscribe_consumer
                {

                  public:

                      //ctor
                      publish_and_subscribe_consumer() = default;
                      //dtor
                      virtual ~publish_and_subscribe_consumer()
                      {}

                  /// consumer can open a broker
                  public:

                      //services
                      virtual bool can_open_broker( const std::string &config ) = 0;
                      virtual std::unique_ptr<publish_and_subscribe> open_broker( const std::string &config ) = 0;

               };
               ///

            private :

                //attributes
               typedef std::list< std::unique_ptr<publish_and_subscribe_consumer>> consumer_list;
               consumer_list m_consumers;

            public:

                void add_publish_subscribe_consumer( std::unique_ptr<publish_and_subscribe_consumer> consumer )
                {
                    m_consumers.push_back( std::move( consumer ) );
                }

                std::unique_ptr<publish_and_subscribe> open_broker( const std::string &config )
                {
                  for( auto& elem : m_consumers )
                  {
                    if(  elem->can_open_broker( config ) )
                    {   return elem->open_broker( config ); }
                  }
                  throw std::runtime_error( "invalid or unsupported consumer type" );

                }

          };

}
