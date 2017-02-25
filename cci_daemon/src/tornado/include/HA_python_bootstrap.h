//HA_python_bootstrap.h   chromatic universe william k. johnson 2017
#pragma once


#include <cci_bootstrap.h>
#include "HAPYTHONBOOTSTRAP_export.h"

static std::string nametag { "auth_mgr_acceptor" };

class HAPYTHONBOOTSTRAP_Export HA_python_bootstrap : public ACE_Service_Object
{
          public:

                explicit HA_python_bootstrap()
                {}

                //upcalls
                virtual int init ( int argc, ACE_TCHAR *argv[] );
                virtual int fini ();
                virtual int info ( ACE_TCHAR **str , size_t len ) const;

          private:

                //attributes


          public :

                //accessors-inspectors



};

ACE_FACTORY_DEFINE(HAPYTHONBOOTSTRAP , HA_python_bootstrap)

