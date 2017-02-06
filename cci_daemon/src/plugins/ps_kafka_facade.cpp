

#include <stdexcept>
#include <exception>
//cci
#include <cci_daemon_kernel.h>
#include <cci_daemon_generic.h>
#include <ps_kafka_facade.h>


using namespace ps_kafka;
using namespace cci_daemon_impl;

////kaka impl
//--------------------------------------------------------------------------
ps_kafka_impl::ps_kafka_impl() : m_moniker { "kafka" }
{

}

//-------------------------------------------------------------------------
bool ps_kafka_impl::connect( const std::string& connect_string )
{
       std::cerr << "...kafka...\n";

       return true;
}

//-------------------------------------------------------------------------
std::ostream& operator << ( std::ostream& ostr , const ps_kafka_impl& pkf )
{
        ostr << "publish/subscribe consumer : kafka";

        return ostr;
}

///kafka virtual consumer
//---------------------------------------------------------------------------
bool ps_kafka_facade::can_open_broker( const std::string& config )
{
          return true;
}

//---------------------------------------------------------------------------
std::unique_ptr<publish_and_subscribe_intf> ps_kafka_facade::open_broker( const std::string& config )
{
          if( !can_open_broker( config ))
          {  throw std::runtime_error ( "broker cannnot be opened" ); }

          std::unique_ptr<publish_and_subscribe_intf> d = std::unique_ptr<ps_kafka_impl>( new ps_kafka_impl() );

          return d;
}

//-----------------------------------------------------------------------------
extern "C" std::ostream& operator << ( std::ostream& ostr , const ps_kafka_facade& pkf )
{
          ostr << "plugin:ps_kafka_facade";

          return ostr;
}


//----------------------------------------------------------------------------
extern "C" void register_plugin( cci_daemon_kernel& kernel )
{
           //register plugin - do all heap allocations/initializations
           //here  for the plugin library
           pas_consumer* pas = new ps_kafka_facade();
           kernel.get_pb_server().add_publish_subscribe_consumer( pas );
           std::cerr << "...registered plugin...\n";

}

//--------------------------------------------------------------------------
extern "C" void clear_context( cci_daemon_kernel& kernel )
{
           std::cerr << "...clear context\n";

           //release any lib )memory allocations here
           pas_server::consumer_list* cl = kernel.get_pb_server().consumers();
           if( cl )
           {
               std::cerr << "...clearing publish and subscribe context\n";
               for( auto elem : *cl )
               {
                   if( elem )
                   {
                        //one of ours
                        if( ps_kafka_facade* pkf =
                                     dynamic_cast<ps_kafka_facade*>( elem ) )
                        {
                            std::cerr << "..,deleting..."
                                      << *pkf
                                      << "\n";
                            delete elem;
                        }
                   }
               }
               std::cerr << "...cleared context\n";
           }
}


