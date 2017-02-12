//cci_daemon_generic.h    william k. johnson 2017

#pragma once
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-function"

#include <string>

namespace cci_daemon_impl
{

        //-----------------------------------------------------------------------
        class publish_and_subscribe_intf
        {
            public :
                virtual bool connect( const std::string& connect_string ) = 0;
                virtual std::string moniker() const noexcept = 0;
        };



}
