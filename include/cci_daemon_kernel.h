//cci_daaemon_kernel.h     william k. johnson 2017

#pragma once

#include <string>
#include <map>
#include <iostream>
#include <utility>
#include <chrono>
#include <thread>
//cci
#include <cci_daemon_generic.h>
#include <cci_daemon_plugin.h>
#include <cci_pub_and_sub.h>
#include <cci_kernel_directives.h>

namespace cci_daemon_impl
{

          //forward
          class cci_daemon_kernel;


          //aliases
          using plugin_dictionary = std::map<std::string , cci_daemon_plugin>;
          using plugins_ptr = plugin_dictionary*;
          using supported_dictionary = std::map<std::string,std::string>;
          using cci_daemon_kernel_ptr = cci_daemon_kernel*;

          //enumerations

          ///core
          //we call this a 'kernel' for the reason it acts like a
          //minimal one. By using function and address indirection
          //like a real kernel  , it operates at the top
          //level of our implementation. It can load and uload
          //our framework components at will-like a premptive
          //kernel. The plugins essentially have their own address
          //space in their library instantiations. Only the kernel
          //actually creates and destroys plugin objects by
          //mainpulating their contexts and it
          //also controls the containers of servers that downcall
          //into the typed shared library by composition.

          class cci_daemon_kernel
          {


                public :


                    //ctor
                    explicit cci_daemon_kernel()  : m_loaded_plugins( new plugin_dictionary )
                    {}
                    explicit cci_daemon_kernel( std::unique_ptr<plugin_dictionary> pd  ) : m_loaded_plugins( std::move( pd ) )
                    {}

                    //dtor
                    virtual ~cci_daemon_kernel() = default;

                    //can't copy a kernel
                    cci_daemon_kernel( const cci_daemon_kernel& ) = delete;
                    //no assign
                    const cci_daemon_kernel& operator=( const cci_daemon_kernel&  ) = delete;


                private :

                    //attributes
                    //
                    //map of plugins
                    std::unique_ptr<plugin_dictionary>      m_loaded_plugins;
                    //nested class that operates as
                    //container for publish/subscribe
                    ////consumers
                    publish_and_subscribe_server            m_pb_server;
                    //supported
                    static supported_dictionary             m_dict_supported;


                protected :

                    //helpers
                    //
                    //invalidate any pb entities
                    //existing after a plugin
                    //has been removed
                    void invalidate_progeny( const std::string& config )
                    {
                        //todo
                    }

                    //release local contexts
                    virtual void destroy_contexts()
                    {
                         std::cerr << "...destroying plugin contexts....\n";

                         //publish and subscribe context
                         for( auto& elem : *m_loaded_plugins )
                         {
                                //inform library components to clear theircontexts
                                elem.second.clear_context( *this ) ;
                         }
                    }


                public :

                    //accessors-inspectors
                    publish_and_subscribe_server&   get_pb_server() { return m_pb_server; }

                    //services
                    //
                    bool supported( const std::string& key )
                    { return m_dict_supported.find( key ) != m_dict_supported.end(); }
                    static std::string version() noexcept { return version_string();  }
                    virtual void load_plugin( const std::string &config );
                    virtual void unload_plugin( const std::string& config );
                    size_t plugin_count() { return m_loaded_plugins->size(); }
                    virtual bool registered( const std::string& config )
                    { return  m_loaded_plugins->find( config ) != m_loaded_plugins->end();   }



           };

}


