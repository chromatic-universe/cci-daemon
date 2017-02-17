//HA_kernel_dynamic.h   chromatic universe william k. johnson 2017
#pragma once



#include <cci_daemon_kernel.h>
#include <proc_ace.h>
#include "HACCIFS_export.h"

static std::string nametag { "ccifs" };

//------------------------------------------------------------------------------------
class ccifs_base_handler : public ACE_Event_Handler
{
           public:

                int handle_timeout( const ACE_Time_Value &current_time,
                                    const void * = 0 )
                {
                    time_t epoch = ( (timespec_t) current_time).tv_sec;
                    ACE_DEBUG( ( LM_INFO ,  "%D ccifs base handler...handle_timeout...\n" ) );

                    return 0;
                };
};


//-------------------------------------------------------------------------------------
class HACCIFS_Export HA_ccifs : public ACE_Service_Object
{
          public:

                explicit HA_ccifs() : m_timer_id { 0 }
                {

                }

                //upcalls
                virtual int init ( int argc, ACE_TCHAR *argv[] );
                virtual int fini ();
                virtual int info ( ACE_TCHAR **str , size_t len ) const;

          private:

                //attributes
                long   m_timer_id;


          public :

                //accessors-inspectors

};

ACE_FACTORY_DEFINE(HACCIFS , HA_ccifs)

