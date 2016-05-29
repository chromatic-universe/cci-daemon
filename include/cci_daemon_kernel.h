//cci_daaemon_kernel.h     william k. johnson 2016

#pragma once

#include <string>
#include <map>
#include <iostream>
#include <utility>
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
                    //
                    void load_plugin( const std::string &config )
                    {

                          if( m_loaded_plugins.find( config ) == m_loaded_plugins.end() )
                          {
                               std::cerr << "loading plugin....\n";

                               cci_daemon_plugin cdp = cci_daemon_plugin( config );
                               auto iter = m_loaded_plugins.insert( std::make_pair( config , cdp ) );
                               iter.first->second.register_plugin( *this );
                               std::cerr << "version is "
                                         << iter.first->second.get_engine_version()
                                         << "\n";
                          }
                    }

                    void destroy_contexts()
                    {
                         std::cerr << "destroying plugin contexts\n";

                         //ublish and subscribe context
                         for( auto& elem : m_loaded_plugins )
                         {
                                std::cerr << elem.first << "\n";
                                elem.second.clear_context( *this ) ;
                         }
                    }
              };

}


