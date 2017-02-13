//HA_manager_protocddol_dynamic.h   chromatic universe william k. johnson 2017
#pragma once



#include <auth_ace_manager.h>
#include "HAAUTH_ACE_MANAGER_export.h"

static std::string nametag { "auth_mgr_acceptor" };

class HAAUTH_ACE_MANAGER_Export HA_auth_mgr_acceptor : public ACE_Service_Object
{
          public:

                explicit HA_auth_mgr_acceptor() : m_acceptor( new auth_ace_manager::auth_mgr_acceptor( auth_ace_manager:: concurrency_t::thread_per_connection_ ) ) ,
                                                  m_data( new auth_ace_manager::auth_mgr_protocol_data( nametag ) )
                {}

                //upcalls
                virtual int init ( int argc, ACE_TCHAR *argv[] );
                virtual int fini ();
                virtual int info ( ACE_TCHAR **str , size_t len ) const;

          private:

                //attributes
                std::unique_ptr<auth_ace_manager::auth_mgr_acceptor> m_acceptor;
                std::unique_ptr<auth_ace_manager::auth_mgr_protocol_data> m_data;


          public :

                //accessors-inspectors
                auth_ace_manager::auth_mgr_acceptor_ptr acceptor_() { return m_acceptor.get(); }
                auth_ace_manager::protocol_data_ptr data_() { return m_data.get(); }



};

ACE_FACTORY_DEFINE(HAAUTH_ACE_MANAGER , HA_auth_mgr_acceptor)

