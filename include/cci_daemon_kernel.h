//cci_daaemon_kernel.h      chromatic universe william k. johnson 2019

#pragma once

#include <string>
#include <map>
#include <iostream>
#include <utility>
#include <chrono>
#include <thread>
#include <mutex>
//cci
#include <cci_daemon_generic.h>
#include <cci_daemon_plugin.h>
#include <cci_pub_and_sub.h>
#include <cci_kernel_directives.h>


namespace cci_daemon_impl
{

      //forward
	  class cci_daemon_base_kernel;
      class cci_daemon_kernel;

      //aliases
      using plugin_dictionary = std::map<std::string , cci_daemon_plugin>;
      using plugins_ptr = plugin_dictionary*;
      using supported_dictionary = std::map<std::string,std::string>;
      using cci_daemon_kernel_ptr = cci_daemon_kernel*;
      using cci_daemon_base_kernel_ptr = cci_daemon_base_kernel*;
      using param_map = std::map<std::string,std::string>;

      //kernel structure - home brew kernel process struct
	  extern "C"
	  {
		  typedef struct kernel_context
		  {
			//atttributes
			//
			//commmand line
			param_map   pm;


			//services
			//
			cci_daemon_impl::cci_daemon_kernel*   kernel_ref;
			cci_handle_t                          lib_ref;
			int ( *make_kernel ) ( kernel_context* context_ptr );
			int ( *unmake_kernel ) ( kernel_context* context_ptr );


		   } kernel_context;
	   }
           typedef kernel_context* kernel_context_ptr;
           //types
           typedef int call_kernel_function(  kernel_context_ptr context_ptr ) ;


	  //base
	  //
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
          class cci_daemon_base_kernel
          {


                public :

		            //ctor
                    cci_daemon_base_kernel();

                    //dtor
                    virtual ~cci_daemon_base_kernel();
                    //can't copy a kernel
                    cci_daemon_base_kernel( const cci_daemon_base_kernel& ) = delete;
                    //no assign
                    const cci_daemon_base_kernel& operator=( const cci_daemon_base_kernel&  ) = delete;


		        protected :

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


                    //helpers
                    //
                    //invalidate any pb entities
                    //existing after a plugin
                    //has been removed
                    void invalidate_progeny( const std::string& config )
                    {
                        //todo
                    }



                public :

                    //accessors-inspectors }
                    //
                    publish_and_subscribe_server&   get_pb_server() { return m_pb_server; }
                    virtual supported_dictionary supported() { return cci_daemon_base_kernel::m_dict_supported; }

                    //services
                    //
                    virtual bool supported_service( const std::string& key )
                    { return cci_daemon_base_kernel::m_dict_supported.find( key )
                        != cci_daemon_base_kernel::m_dict_supported.end(); }
                    virtual void load_plugin( const std::string &config );
                    virtual void unload_plugin( const std::string& config );
                    size_t plugin_count() { return m_loaded_plugins->size(); }
                    virtual bool registered( const std::string& config )
                    { return  m_loaded_plugins->find( config ) != m_loaded_plugins->end();   }

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


           };
	   //cci_daemon_kernel
	   class cci_daemon_kernel : public cci_daemon_base_kernel
	   {
		   public :

			    //instance - kernel singleton
			    static cci_daemon_kernel_ptr  instance()
			    {
				//double checked lock
				if( ! m_instance )
				{
				    std::lock_guard<std::mutex> lg( m_mutex );
				    if( ! m_instance )
				    {  m_instance = new cci_daemon_kernel(); }
				}

				return  m_instance;
			    }

			    //dispose instance
			    static void dispose_instance()
			    {
                    delete m_instance;
                    m_instance = nullptr;
			    }

			    //dtor
			    virtual ~cci_daemon_kernel()
			    {}

			    //can't copy a kernel
			    cci_daemon_kernel( const cci_daemon_kernel& ) = delete;
			    //no assign
			    const cci_daemon_kernel& operator=( const cci_daemon_kernel&  ) = delete;


		  private :

			   //ctor
			   cci_daemon_kernel()
			   {}

		  protected :

			   static cci_daemon_kernel_ptr       m_instance;
               static std::mutex                  m_mutex;



	   };



	   //facade
	   //
           //-----------------------------------------------------------------------------------
           extern "C" int make_kernel( kernel_context_ptr context_ptr );
           //------------------------------------------------------------------------------------
           extern "C" int unmake_kernel( kernel_context_ptr context_ptr );






}
