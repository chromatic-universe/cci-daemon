//HA_kernel_dynamic.cpp   chromatic universe william k. johnson 2017

#include <HA_ccifs_dynamic.h>
#include <sstream>
#include <sys/mount.h>

static std::string conf_path { "/etc/chromatic-universe/ha_ccifs.ini" };
static std::string ccifs_user { "wiljoh" };
static std::string ccifs_prog { "ccifs" };
static std::string ccifs_pt { "/var/ccifs/cache" };


static int ccifs_inotify( const std::string& ccifs );
static void display_inotify_event( struct inotify_event* ine );
//
//

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
                                       ACE_TEXT ("HA_ccifs"),
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
                                             ACE_TEXT ( "ccifs_tmpfs_mount" ) ,
                                             ccifs ) == -1 )
              ACE_ERROR_RETURN ((LM_ERROR,
                                   ACE_TEXT ("HA_ccifs ccifs_mount")
                                   ACE_TEXT (" does not exist\n") ) ,
                                  -1 );

              if( ACE_Thread_Manager::instance()->spawn( ACE_THR_FUNC (ccifs_func) ,
                                                     (void*) this ,
                                                     THR_NEW_LWP  ,
                                                     &m_thread_id ) )
              {
                  ACE_DEBUG
                  ((LM_DEBUG, ACE_TEXT ("(%t) ..spawned ccifs notify thread..\n")));

              }
              else
              {
                   ACE_DEBUG
                  ((LM_DEBUG, ACE_TEXT ("(%t) ..spawning ccifs notify failed..\n")));

              }



              return 0;
}

//------------------------------------------------------------------------------------------------
int  HA_ccifs::fini()
{

            ACE_Trace _( ACE_TEXT( "HA_ccifs::fini" ) , __LINE__ );

            if( m_b_running ) { m_b_running = false; }
            //std::this_thread::sleep_for( std::chrono::milliseconds( 250 ) );
            int t = ACE_Thread_Manager::instance()->cancel( m_thread_id );
            t = ACE_Thread_Manager::instance()->testcancel( m_thread_id );

            ACE_DEBUG((LM_NOTICE , "%D(%t) ccifs=>notify thread test cancel=%d\n",
                                            t ) );


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


//--------------------------------------------------------------------------------------
void ccifs_func( void* ptr_instance )
{
               ACE_Trace _( ACE_TEXT( "HA_ccifs ccifs_func" ) , __LINE__ );


               ACE_DEBUG
                  ((LM_DEBUG, ACE_TEXT ("(%D %t) ..ccifs notify thread..\n")));

                HA_ccifs*  ccifs = static_cast<HA_ccifs*> ( ptr_instance );
                if( ccifs )
                {
                    ccifs->ccifs_inotify();
                }

                ACE_DEBUG
                  ((LM_DEBUG, ACE_TEXT ("%D (%t) ..ccifs notify thread exit..\n")));


}

//------------------------------------------------------------------------------------------------
int HA_ccifs::ccifs_inotify()
{
                ACE_Trace _( ACE_TEXT( "HA_ccifs::ccifs_inotify" ) , __LINE__ );


                static size_t BUF_LEN { 10 * (sizeof(struct inotify_event) + NAME_MAX + 1 ) };
                int inotify_fd, wd;
                char buf[BUF_LEN] __attribute__ ((aligned(8)));
                ssize_t num_read;
                char *p;
                struct inotify_event *event;


                //instantiate inotify interface
                inotify_fd = inotify_init();
                if( inotify_fd == -1 )
                {
                    ACE_ERROR_RETURN ((LM_ERROR,
                                       ACE_TEXT ( "HA_ccifs notify init")
                                       ACE_TEXT (" failed\n") ) ,
                                      -1 );
                }

                ACE_DEBUG
                  ((LM_NOTICE, ACE_TEXT ("%D (%t) ccifs:..initialized inotify interace..\n") ) );

                wd = inotify_add_watch( inotify_fd ,
                                        m_str_tmpfs.c_str() ,
                                        IN_ALL_EVENTS );
                if( wd == -1 )
                {
                      ACE_ERROR_RETURN ((LM_ERROR,
                                       ACE_TEXT ( "ccifs: add notifiy watch...")
                                       ACE_TEXT (" failed\n") ) ,
                                      -1 );

                }

                ACE_DEBUG((LM_NOTICE , "%D(%t) ccifs added notify watch=>%s\n",
                                            m_str_tmpfs.c_str() ) );


                while( running() )
                {

                      num_read = read( inotify_fd ,
                                       buf ,
                                       BUF_LEN );
                      if( num_read == 0 )
                      {
                             ACE_ERROR_RETURN ((LM_ERROR,
                                       ACE_TEXT ( "ccifs: read() from inotify fd returned 0...")
                                       ACE_TEXT (" failed\n") ) ,
                                      -1 );
                     }
                     if( num_read == -1 )
                     {

                            ACE_ERROR_RETURN ((LM_ERROR,
                                       ACE_TEXT ( "ccifs: read ")
                                       ACE_TEXT (" failed...\n") ) ,
                                      -1 );
                     }

                     //process elements in buffer
                     for ( p = buf; p < buf + num_read; )
                     {
                        event = (struct inotify_event*) p;
                        display_inotify_event( event );

                        p += sizeof(struct inotify_event) + event->len;
                     }

                     //std::this_thread::sleep_for( std::chrono::milliseconds( 250 ) );
                }




                return 0;
}

//------------------------------------------------------------------------------------------------
void display_inotify_event( struct inotify_event* ine )
{
                if( ine->mask & IN_CREATE )
                {
                    std::ostringstream ostr;
                    ostr << "<created> decriptor: "
                         << ine->wd
                         << " cookie:"
                         << ine->cookie
                         << " ";
                    if( ine->len > 0 )
                    {
                        ostr << "name="
                            << ine->name;
                    }

                    ACE_DEBUG((LM_NOTICE , "%D(%t) ccifs=>%s\n",
                                            ostr.str().c_str() ) );

                }

}


