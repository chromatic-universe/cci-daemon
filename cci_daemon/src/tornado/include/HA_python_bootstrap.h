//HA_python_bootstrap.h   chromatic universe william k. johnson 2017
#pragma once

#include <chrono>
#include <thread>
#include <limits.h>
#include <proc_ace.h>
#include <HAPYTHONBOOTSTRAP_export.h>
#include <Python.h>

static std::string nametag { "python bootstrap" };

//--------------------------------------------------------------------------------------
static void python_bootstrap_func( void* ptr_instance );


class HAPYTHONBOOTSTRAP_Export HA_python_bootstrap : public ACE_Service_Object
{
          public :

                explicit HA_python_bootstrap() : m_pid { 0 } ,
                                                 m_thread_id { 0 } ,
                                                 m_port { 0 } ,
                                                 m_str_private { "" }
                {}

                ~HA_python_bootstrap()
                {
                    ACE_Trace _( ACE_TEXT( "HA_python_bootstrap destructor" ) , __LINE__ );
                }

                //upcalls
                virtual int init ( int argc, ACE_TCHAR *argv[] );
                virtual int fini ();
                virtual int info ( ACE_TCHAR **str , size_t len ) const;

          private :

                //attributes
                unsigned         m_pid;
                ACE_thread_t     m_thread_id;
                u_int            m_port;
                std::string      m_str_private;
                std::string      m_str_home;


          protected :

                //helpers
                void set_py_env();
                int validate_entry_point( const std::string& entry );
                int file_exists( const std::string& filename );
                int open_entry_point( const std::string&  entry );


          public :

                //accessors-inspectors
                unsigned pid() const noexcept { return m_pid; }
                ACE_thread_t tid() const noexcept { return m_thread_id; }
                u_int port() const noexcept { return m_port; }
                std::string prv() const noexcept { return m_str_private; }
                std::string home() const noexcept { return m_str_home; }

                //mutators
                void pid( const unsigned pd ) { m_pid = pd; }
                void tid( const ACE_thread_t id ) { m_thread_id = id; }
                void port( const u_int prt ) { m_port = prt; }
                void prv( const std::string& pv ) { m_str_private = pv; }
                void home( const std::string& hm ) { m_str_home = hm; }



                //immutable
                static std::string cci_trinity_app_moniker;
                static std::string cci_trinity_async_moniker;



};

ACE_FACTORY_DEFINE(HAPYTHONBOOTSTRAP , HA_python_bootstrap)

