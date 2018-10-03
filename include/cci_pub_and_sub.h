
//cci_pub_and_sub.h      william k. johnson 2018



#include <list>
#include <string>
#include <memory>
#include <stdexcept>
#include <memory>

namespace cci_daemon_impl
{


          ///publish-subscribe related domain
          class publish_and_subscribe_server
          {

             public:

                ///
                //chain of responsibility
                class publish_and_subscribe_consumer
                {

                  public :

                      //ctor
                      publish_and_subscribe_consumer() = default;
                      //dtor
                      virtual ~publish_and_subscribe_consumer()
                      {}

                 private :

                      //attributes
                      std::string   m_str_moniker;

                  /// consumer can open a broker
                  public :

                      //services
                      virtual bool can_open_broker( const std::string &config ) = 0;
                      virtual std::unique_ptr<publish_and_subscribe_intf> open_broker( const std::string &config ) = 0;

               };
               ///
               typedef std::list<publish_and_subscribe_consumer*> consumer_list;
               //ctor
               publish_and_subscribe_server() : m_consumers( new consumer_list )
               {}
               //dtor
               virtual ~publish_and_subscribe_server()
               {}


            private :

                      std::unique_ptr<consumer_list> m_consumers;

            public:

                //accessors-inspectors
                consumer_list* consumers() { return  m_consumers.get(); }

                //services
                void add_publish_subscribe_consumer( publish_and_subscribe_consumer* consumer )
                {
                    m_consumers->push_back( consumer );
                }

                std::unique_ptr<publish_and_subscribe_intf> open_broker( const std::string &config )
                {
                  for( auto& elem : *m_consumers )
                  {
                    if(  elem->can_open_broker( config ) )
                    {   return elem->open_broker( config ); }
                  }
                  throw std::runtime_error( "invalid or unsupported consumer type" );

               }



          };

}
