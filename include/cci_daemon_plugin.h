//cci_daemon_plugin.h    william k. johnson  2016
#pragma once

#include  <string>
#include  <map>

//cci
#include <cci_shared_lib.h>


namespace cci_daemon_impl
{

          //forward decalrations
          class cci_daemon_kernel;

          //aliases
          using size_ptr_t = size_t*;

          //representation of a plugin
          class cci_daemon_plugin
          {

              public :

                  //ctor
                  explicit cci_daemon_plugin( const std::string& filename );
                  //dtor
                  virtual ~cci_daemon_plugin();
                  //copy
                  cci_daemon_plugin( const cci_daemon_plugin& cdp );
                  //assign
                  cci_daemon_plugin& operator= ( const cci_daemon_plugin& cdp );

              private :

                  //attributes
                  cci_handle_t      m_shared_lib;
                  size_ptr_t        m_ref_count;
                  std::string       m_str_filename;

                  //prototypes
                  typedef int   get_engine_version_function();
                  typedef void  register_plugin_function( cci_daemon_kernel& cdk );
                  //helpers
                  get_engine_version_function* get_engine_version_address;
                  register_plugin_function*    register_plugin_address;

             public :

                  //accessors-inspectors
                  int get_engine_version() const noexcept
                  { return get_engine_version_address(); }
                  //services
                  void register_plugin( cci_daemon_kernel& kernel )
                  { register_plugin_address( kernel ); }



          };
}
