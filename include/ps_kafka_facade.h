//ps_kafka_facade.h      william k. johnson 2017
//


#pragma once


//c++ standard
#include <string>
#include <stdexcept>
#include <exception>
#include <sstream>
#include <cassert>
//cci
#include <cci_daemon_utils.h>
#include <cci_daemon_kernel.h>
#include <cci_daemon_generic.h>

namespace cdi = cci_daemon_impl;

namespace  ps_kafka
{



          //aliases
          using pas_consumer = cdi::publish_and_subscribe_server::publish_and_subscribe_consumer;
          using pas_server =  cdi::publish_and_subscribe_server;

          ///kafka impl
          //--------------------------------------------------------------------------
          class ps_kafka_impl : public cdi::publish_and_subscribe_intf
          {

                public :

                    //repr
                    friend std::ostream&  operator <<( std::ostream& ostr , const ps_kafka_impl& pki );

                    explicit ps_kafka_impl() ;
                    virtual ~ps_kafka_impl() = default;


                protected :

                    //

                private   :

                    std::string     m_moniker;

                public :

                   //accessors-inspectors
                   std::string moniker() const noexcept { return m_moniker; }

                   //services
                   bool connect( const std::string& connect_string );

          };

          //kafka plugin
          //---------------------------------------------------------------------------
          class  ps_kafka_facade : public pas_consumer ,
                                   public std::enable_shared_from_this<ps_kafka_facade>
          {

            public :

                //repr
                friend std::ostream&  operator <<( std::ostream& ostr , const ps_kafka_facade&& pkf );

                //ctor
                explicit ps_kafka_facade() = default;
                //dtor
                virtual ~ps_kafka_facade() = default;

                //services
                bool can_open_broker ( const std::string& config );
                std::unique_ptr<cdi::publish_and_subscribe_intf> open_broker( const std::string& config );

          };

          //dynamic exports
          extern "C" int get_engine_version() { return 1; }
          extern "C" void register_plugin( cdi::cci_daemon_kernel& kernel );
          extern "C" void clear_context( cdi::cci_daemon_kernel& kernel );


}

