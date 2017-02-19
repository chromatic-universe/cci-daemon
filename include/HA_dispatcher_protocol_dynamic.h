
#pragma once

//HA_dispatcher_protocol_dynamic.h   chromatic universe william k. johnson 2017


#include <proc_ace_acceptor.h>
#include <cci_kernel_directives.h>
#include "HAPROC_ACE_ACCEPTOR_export.h"

static std::string proc_nametag { "proc_ace_acceptor" };

class HAPROCACEACCEPTOR_Export HA_proc_ace_acceptor : public ACE_Service_Object
{
          public:

                explicit HA_proc_ace_acceptor() : m_acceptor( new proc_ace::proc_acceptor( proc_ace:: concurrency_t::thread_per_connection_ ) ) ,
                                                  m_data( new proc_ace::proc_protocol_data( proc_nametag ) )
                {}

                //upcalls
                virtual int init ( int argc, ACE_TCHAR *argv[] );
                virtual int fini ();
                virtual int info ( ACE_TCHAR **str , size_t len ) const;

          private:

                //attributes
                std::unique_ptr<proc_ace::proc_acceptor> m_acceptor;
                std::unique_ptr<proc_ace::proc_protocol_data> m_data;


          public :

                //accessors-inspectors
                proc_ace::proc_acceptor_ptr acceptor_() { return m_acceptor.get(); }
                proc_ace::protocol_data_ptr data_() { return m_data.get(); }



};

ACE_FACTORY_DEFINE(HAPROCACEACCEPTOR , HA_proc_ace_acceptor)

