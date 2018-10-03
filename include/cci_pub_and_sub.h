
//cci_pub_and_sub.h      william k. johnson 2018

#pragma once

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
                class publish_and_subscribe_duplexer
                {

                  public :

                      //ctor
                      publish_and_subscribe_duplexer() = default;
                      //dtor
                      virtual ~publish_and_subscribe_duplexer()
                      {}

                 private :

                      //attributes
                      std::string   m_str_moniker;

                  /// duplexer can open a broker
                  public :

                      //services
                      virtual bool can_open_broker( const std::string &config ) = 0;
                      virtual std::unique_ptr<publish_and_subscribe_intf> open_broker( const std::string &config ) = 0;

               };
               ///
               typedef std::list<publish_and_subscribe_duplexer*> duplexer_list;
               //ctor
               publish_and_subscribe_server() : m_duplexers( new duplexer_list )
               {}
               //dtor
               virtual ~publish_and_subscribe_server()
               {}


            private :

                      std::unique_ptr<duplexer_list> m_duplexers;

            public:

                //accessors-inspectors
                duplexer_list* duplexers() { return  m_duplexers.get(); }

                //services
                void add_publish_subscribe_duplexer( publish_and_subscribe_duplexer* duplexer )
                {
                    m_duplexers->push_back( duplexer );
                }

                std::unique_ptr<publish_and_subscribe_intf> open_broker( const std::string &config )
                {
                  for( auto& elem : *m_duplexers )
                  {
                    if(  elem->can_open_broker( config ) )
                    {   return elem->open_broker( config ); }
                  }
                  throw std::runtime_error( "invalid or unsupported duplexer type" );

               }



          };

}
