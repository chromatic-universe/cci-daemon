//HA_kernel_dynamic.h   chromatic universe william k. johnson 2017
#pragma once


#include <chrono>
#include <thread>
#include <proc_ace.h>
#include <sys/inotify.h>
#include <limits.h>
#include <cci_daemon_kernel.h>

#include "HACCIFS_export.h"

static std::string nametag { "ccifs" };

//--------------------------------------------------------------------------------------
static void ccifs_func( void* ptr_instance );
//--------------------------------------------------------------------------------------
static void display_inotify_event( struct inotify_event* ine );


//-------------------------------------------------------------------------------------
class HACCIFS_Export HA_ccifs : public ACE_Service_Object
{
          public:

                explicit HA_ccifs() : m_thread_id { 0 } ,
                                      m_b_running { true } ,
                                      m_str_tmpfs { "/var/ccifs/cache" }
                {
                                   }

                ~HA_ccifs()
                {
                    ACE_Trace _( ACE_TEXT( "HA_ccifs::destructor" ) , __LINE__ );
                }

                //upcalls
                virtual int init ( int argc, ACE_TCHAR *argv[] );
                virtual int fini ();
                virtual int info ( ACE_TCHAR **str , size_t len ) const;

          private:

                //attributes
                ACE_thread_t   m_thread_id;
                bool           m_b_running;
                std::string    m_str_tmpfs;



          public :

                //accessors-inspectors
                bool running() const noexcept { return m_b_running; }
                std::string fs() const noexcept { return m_str_tmpfs; }

                //mutators
                void fs( const std::string& target ) { m_str_tmpfs = target; }

                //services
                int ccifs_inotify();



};




ACE_FACTORY_DEFINE(HACCIFS , HA_ccifs)

