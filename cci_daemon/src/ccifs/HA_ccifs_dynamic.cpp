//HA_kernel_dynamic.cpp   chromatic universe william k. johnson 2017

#include <HA_ccifs_dynamic.h>
#include <cci_kernel_directives.h>


static std::string conf_path { "/etc/chromatic-universe/ha_ccifs.ini" };

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

    	            ACE_Trace _( ACE_TEXT( "proc_signal_handler..HA_ccifs..." ) , __LINE__ );

                    switch( signum )
                    {
                        case SIGINT :

                            ACE_DEBUG(( LM_INFO , "...sigint received...\n" ) );

                            break;
                    }

                    return 0;
                }
};

//---------------------------------------------------------------------------------------------
int HA_ccifs::init ( int argc , ACE_TCHAR *argv[] )
{

               ACE_Trace _( ACE_TEXT( "%D HA_ccifs::init" ) , __LINE__ );



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
                                   ACE_TEXT ( "HA_ccifs" ),
                                   0,
                                   dispatcher_section) == -1)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("%p\n"),
                               ACE_TEXT ( "can't open HA_ccifs section"  ) ) ,
                              -1 );
          //fifo
          u_int dispatcher_port;
          if (config.get_integer_value ( dispatcher_section,
                                         ACE_TEXT ( "fifo" ) ,
                                         dispatcher_port ) == -1 )
          ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("HA_ccifs fifo")
                               ACE_TEXT (" does not exist\n") ) ,
                              -1 );
          //ccifs
          ACE_TString ccifs;
          if (config.get_string_value (  dispatcher_section,
                                         ACE_TEXT ( "ccifs_mount" ) ,
                                         ccifs ) == -1 )
          ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("HA_ccifs ccifs_mount")
                               ACE_TEXT (" does not exist\n") ) ,
                              -1 );



          ccifs_base_handler* hdlr = new ccifs_base_handler();
          ACE_Time_Value initial_delay( 3 );
          ACE_Time_Value interval( 1 );
          m_timer_id = ACE_Reactor::instance()->schedule_timer( hdlr ,
                                                                0 ,
                                                                initial_delay ,
                                                                interval );


           return 0;
}

//------------------------------------------------------------------------------------------------
int  HA_ccifs::fini()
{

            ACE_Trace _( ACE_TEXT( "HA_ccifs::fini" ) , __LINE__ );

            ACE_Reactor::instance()->cancel_timer( m_timer_id );


            return 0;
}

//------------------------------------------------------------------------------------------------
int HA_ccifs::info( ACE_TCHAR **str , size_t len ) const
{

            ACE_Trace _( ACE_TEXT( "HA_ccifs::info" ) , __LINE__ );


            ACE_TCHAR buf[BUFSIZ];
            ACE_OS::sprintf ( buf,
                              ACE_TEXT ( "HA_ccifs initialized" ) );
            if ( *str == 0 )
            { *str = ACE::strnew (buf); }
            else
            {  ACE_OS::strncpy (*str, buf, len); }

            return static_cast<int>( ACE_OS::strlen (*str) );
}


