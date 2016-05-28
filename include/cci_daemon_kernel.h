//cci_daaemon_kernel.h     william k. johnson 2016

#pragma once

#include <string>
#include <map>

//cci
#include <cci_daemon_generic.h>
#include <cci_daemon_plugin.h>
#include <cci_pub_and_sub.h>

namespace cci_daemon_impl
{

          //forward


          //aliases


          //enumerations

          ///core
          class cci_daemon_kernel
          {

                typedef std::map<std::string , cci_daemon_plugin> plugin_dictionary;

                public :

                    //ctor
                    explicit cci_daemon_kernel() = default;
                    //dtor
                    virtual ~cci_daemon_kernel() = default;

                private :

                    //attributes
                    plugin_dictionary               m_loaded_plugins;
                    publish_and_subscribe_server    m_pb_server;

                public :

                    //accessors-inspectors
                    publish_and_subscribe_server&   get_pb_server() { return m_pb_server; }

                    //services
                    void load_plugin( const std::string &config )
                    {
                      if( m_loaded_plugins.find( config ) == m_loaded_plugins.end() )
                      {
                           m_loaded_plugins.insert(  plugin_dictionary::value_type( config ,
                                                                                    cci_daemon_plugin( config ) )
                        ).first->second.register_plugin( *this );
                      }
                   }
              };

}


