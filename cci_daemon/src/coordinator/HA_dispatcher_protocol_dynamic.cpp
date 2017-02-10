//HA_dispatcher_protocol_dynamic.cpp   chromatic universe william k. johnson 2017

#include <HA_dispatcher_protocol_dynamic.h>



static std::string conf_path { "/etc/chromatic-universe/ha_proc_ace_acceptor.ini" };
unsigned long g_dw_finished = 0L;


//signal handles
class proc_signal_handler : public ACE_Event_Handler
{
            public:

                proc_signal_handler() : ACE_Event_Handler()
                {}

                int handle_signal ( int signum ,
                                    siginfo_t * = 0 ,
                                    ucontext_t * = 0 )
                {

    	            ACE_Trace _( ACE_TEXT( "proc_signal_handler..HA_proc_ace_acceptor.." ) , __LINE__ );

                    switch( signum )
                    {
                        case SIGINT :

                            ACE_DEBUG(( LM_INFO , "...sigint received...\n" ) );
                            g_dw_finished = 1L;

                            break;
                    }

                    return 0;
                }
};

//---------------------------------------------------------------------------------------------
int HA_proc_ace_acceptor::init ( int argc , ACE_TCHAR *argv[] )
{

               ACE_Trace _( ACE_TEXT( "HA_proc_acceptor::init" ) , __LINE__ );

              //command line
              //-------------------------------------------------------------------------------
              static const ACE_TCHAR options[] = ACE_TEXT (":f:");
              ACE_Get_Opt cmd_opts (argc, argv, options, 0);
              if (cmd_opts.long_option
                  (ACE_TEXT ( "config" ), 'f', ACE_Get_Opt::ARG_REQUIRED) == -1)
              { return -1; }

              int option;
              ACE_TCHAR config_file[MAXPATHLEN];
              ACE_OS::strcpy ( config_file, ACE_TEXT ( conf_path.c_str() ) );
              while ( ( option = cmd_opts ()) != EOF)
              switch ( option )
              {
                  case 'f' :

                    ACE_OS::strncpy (config_file , cmd_opts.opt_arg () , MAXPATHLEN );
                    break;

                  case ':':

                    ACE_ERROR_RETURN ( ( LM_ERROR , ACE_TEXT ( "-%c requires an argument\n" )  ,
                                       cmd_opts.opt_opt ()) , -1 );
                  default:

                    ACE_ERROR_RETURN ( ( LM_ERROR , ACE_TEXT ( "parse error.\n" ) ) ,
                                      - 1);
              }

              //configuration file
              //-------------------------------------------------------------------------------
              ACE_Configuration_Heap config;
              config.open ();
              ACE_Registry_ImpExp config_importer (config);
              if ( config_importer.import_config (config_file) == -1 )
              { ACE_ERROR_RETURN ( ( LM_ERROR , ACE_TEXT ("%p\n") , config_file ) , -1 ); }


              ACE_Configuration_Section_Key dispatcher_section;
              if (config.open_section (config.root_section (),
                                       ACE_TEXT ( "HA_proc_ace_acceptor" ),
                                       0,
                                       dispatcher_section) == -1)
                ACE_ERROR_RETURN ((LM_ERROR,
                                   ACE_TEXT ("%p\n"),
                                   ACE_TEXT ( "can't open HA_proc_ace_acceptor section"  ) ) ,
                                  -1 );
              //listen port
              u_int dispatcher_port;
              if (config.get_integer_value ( dispatcher_section,
                                             ACE_TEXT ( "listen_port" ) ,
                                             dispatcher_port ) == -1 )
                ACE_ERROR_RETURN ((LM_ERROR,
                                   ACE_TEXT ("HA_proc_ace_acceptor listen_port ")
                                   ACE_TEXT (" does not exist\n") ) ,
                                  -1 );
              data_()->port( dispatcher_port );

              //register signals
              //proc_signal_handler* handler = new proc_signal_handler();
              //CE_Reactor::instance()->register_handler( SIGINT ,  handler );

              //initialize timer
              ACE_High_Res_Timer::global_scale_factor();
              ACE_Reactor::instance()->timer_queue()->gettimeofday( &ACE_High_Res_Timer::gettimeofday_hr );

              //thread pool
              acceptor_()->thread_pool( ACE_Thread_Manager::instance() );
              //open acceptor
              if ( acceptor_()->open ( ACE_INET_Addr( data_()->port() ) ,
                                                      ACE_Reactor::instance() ,
                                                      data_() ,
                                                      data_()->thread_pool_size() ) == -1 )
              {

                    ACE_ERROR_RETURN ( ( LM_ERROR,
                                         "%p\n" ,
                                         "open" ) ,
                                         -1 );
              }

              //unsigned long  wait_time = 1L;
              //auto tv =  ACE_Time_Value( wait_time );
              //while ( ! g_dw_finished )
              //{	ACE_Reactor::instance()->handle_events ( &tv ); }


              return 0;
}

//------------------------------------------------------------------------------------------------
int  HA_proc_ace_acceptor::fini()
{

            ACE_Trace _( ACE_TEXT( "HA_proc_acceptor::fini" ) , __LINE__ );


            this->m_acceptor->close();

            return 0;
}

//------------------------------------------------------------------------------------------------
int HA_proc_ace_acceptor::info( ACE_TCHAR **str , size_t len ) const
{

            ACE_Trace _( ACE_TEXT( "HA_proc_acceptor::info" ) , __LINE__ );


            ACE_TCHAR buf[BUFSIZ];
            ACE_OS::sprintf ( buf,
                              ACE_TEXT ("HA_proc_ace_accepotr listening on port %lu\n"),
                              this->m_data->port() );
            if ( *str == 0 )
            { *str = ACE::strnew (buf); }
            else
            {  ACE_OS::strncpy (*str, buf, len); }

            return static_cast<int>( ACE_OS::strlen (*str) );
}


