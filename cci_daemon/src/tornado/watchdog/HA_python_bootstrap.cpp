//HA_python_bootstrap.cpp   chromatic universe william k. johnson 2017

#include <HA_python_bootstrap.h>
#include <Python.h>



static std::string conf_path { "/etc/chromatic-universe/ha_python_bootstrap.ini" };

std::string HA_python_bootstrap::cci_trinity_app_moniker = "cci_trinity.py";
std::string HA_python_bootstrap::cci_trinity_async_moniker = "cci_trinity_async.py";


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

    	            ACE_Trace _( ACE_TEXT( "proc_signal_handler..HA_python_bootstrap.." ) , __LINE__ );

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
int HA_python_bootstrap::init ( int argc , ACE_TCHAR *argv[] )
{

               ACE_Trace _( ACE_TEXT( "HA_python_bootstrap::init" ) , __LINE__ );


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
                                       ACE_TEXT ( "HA_python_bootstrap" ),
                                       0,
                                       dispatcher_section) == -1)
                ACE_ERROR_RETURN ((LM_ERROR,
                                   ACE_TEXT ("%p\n"),
                                   ACE_TEXT ( "can't open HA_python_bootstrap section"  ) ) ,
                                  -1 );
              //listen port
              u_int dispatcher_port;
              if (config.get_integer_value ( dispatcher_section,
                                             ACE_TEXT ( "listen_port" ) ,
                                             dispatcher_port ) == -1 )
                ACE_ERROR_RETURN ((LM_ERROR,
                                   ACE_TEXT ("HA_python_bootstrap listen_port ")
                                   ACE_TEXT (" does not exist\n") ) ,
                                  -1 );
              port( dispatcher_port );

              //cci private
              ACE_TString cci_private;
              if (config.get_string_value (  dispatcher_section,
                                             ACE_TEXT ( "cci_private" ) ,
                                             cci_private ) == -1 )
              ACE_ERROR_RETURN ((LM_ERROR,
                                   ACE_TEXT ("HA_python_bootstrap cci private")
                                   ACE_TEXT (" does not exist\n") ) ,
                                  -1 );
              prv( cci_private.c_str() );
              //cci private_python
              if (config.get_string_value (  dispatcher_section,
                                             ACE_TEXT ( "cci_private_python" ) ,
                                             cci_private ) == -1 )
              ACE_ERROR_RETURN ((LM_ERROR,
                                   ACE_TEXT ("HA_python_bootstrap cci private_python")
                                   ACE_TEXT (" does not exist\n") ) ,
                                  -1 );
              if( chdir( cci_private.c_str() ) == -1 )
              {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                   ACE_TEXT ("...change directory failed...")
                                   ACE_TEXT (" python bootstrap\n") ) ,
                                  -1 );

              }

              set_py_env();

              //the program name , not important
              wchar_t w_name[] = { L"cci-python" };
              Py_SetProgramName( w_name );

              try
              {
                    Py_Initialize();
              }
              catch( ... )
              {
                       ACE_ERROR_RETURN ((LM_ERROR,
                                   ACE_TEXT ("...Py_Initialize...")
                                   ACE_TEXT (" fatal exception\n" ) ) ,
                                  -1 );

              }

              int py = PyRun_SimpleString( "import sys, logging , os , posix\n" );
              if( py != -1 )
              {
                     ACE_DEBUG
                  ((LM_DEBUG, ACE_TEXT ("(%t) ..PyRun_SimpleString...ok...import sys , posix modules...\n")));

              }



              /*if( ACE_Thread_Manager::instance()->spawn( ACE_THR_FUNC (python_bootstrap_func) ,
                                                     (void*) this ,
                                                     THR_NEW_LWP  ,
                                                     &m_thread_id ) )
              {
                  ACE_DEBUG
                  ((LM_DEBUG, ACE_TEXT ("(%t) ..spawned python_bootstrap thread..\n")));

              }
              else
              {
                   ACE_DEBUG
                  ((LM_DEBUG, ACE_TEXT ("(%t) ..spawning of python bootstrap failed..\n")));

              }*/


              if( validate_entry_point( HA_python_bootstrap::cci_trinity_app_moniker ) == -1 )
              {

                       ACE_ERROR_RETURN ((LM_ERROR,
                                   ACE_TEXT ("...open entry point...")
                                   ACE_TEXT (" fatal exception\n" ) ) ,
                                  -1 );

              }


              py =  open_entry_point( HA_python_bootstrap::cci_trinity_app_moniker );
              if( py != -1 )
              {
                  ACE_DEBUG
                  ( ( LM_DEBUG, ACE_TEXT ("(%t) ..python entry point bootstrapped...\n" ) ) );
              }


              return 0;
}


//------------------------------------------------------------------------------------------------
void HA_python_bootstrap::set_py_env()
{
            ACE_Trace _( ACE_TEXT( "HA_python_bootstrap::set_py_env" ) , __LINE__ );
            std::ostringstream ostr;

            ostr << prv()
                 << "/lib/python3.6/lib-dynload/:"
                 << prv()
                 << "/lib/python3.6/:"
                 << prv()
                 << "/lib/python3.6/site-packages/:"
                 <<prv()
                 << "/bin/:"
                 << home();

                 setenv( "PYTHONPATH" , ostr.str().c_str() , 1 );
                 setenv( "CCI_ENTRYPOINT" ,
                         HA_python_bootstrap::cci_trinity_app_moniker.c_str() ,
                         1 );

}


//------------------------------------------------------------------------------------------------
int  HA_python_bootstrap::validate_entry_point( const std::string& entry )
{
            ACE_Trace _( ACE_TEXT( ":HA_python_bootstrap::validate_entry_point" ) , __LINE__ );

            std::string::size_type idx = entry.find( "." );
            if( idx == std::string::npos ) { return  -1; }
            //file type
            std::string str( entry.substr( ++idx , std::string::npos )  );
            //pyo
            if( str.compare( "pyo" ) != 0 )
            //try py
            { if( str.compare( "py" ) != 0 ) { return -1; }  }

            //lazy man stat
            return file_exists( entry );

}


//------------------------------------------------------------------------------------------------
int  HA_python_bootstrap::file_exists( const std::string& filename )
{
              FILE *file;
              file = fopen( filename.c_str() , "r" );
              if( file )
              {
                fclose( file );

                return 1;
              }

              return 0;
}


//------------------------------------------------------------------------------------------------
int  HA_python_bootstrap::open_entry_point( const std::string&  entry )
{

              FILE *fd;

              fd = fopen( entry.c_str() , "r" );
              if ( fd == NULL )
              {
                 return -1;
              }

              int ret = PyRun_SimpleFile( fd , entry.c_str() );
              if ( PyErr_Occurred() != NULL )
              {
                ret = 1;
                // this exits with the right code if sys exit
                PyErr_Print();
                PyObject *f = PySys_GetObject( "stderr" );
                if ( PyFile_WriteString( "\n", f ) ) { PyErr_Clear(); }
              }


              return ret;

}


//------------------------------------------------------------------------------------------------
int  HA_python_bootstrap::fini()
{

            ACE_Trace _( ACE_TEXT( "HA_python_bootstrap::fini" ) , __LINE__ );


            int t = ACE_Thread_Manager::instance()->cancel( m_thread_id );
            t = ACE_Thread_Manager::instance()->testcancel( m_thread_id );

            ACE_DEBUG((LM_NOTICE , "%D(%t) python bootstrap thread test cancel=%d\n",
                                            t ) );



            return 0;
}

//------------------------------------------------------------------------------------------------
int HA_python_bootstrap::info( ACE_TCHAR **str , size_t len ) const
{

            ACE_Trace _( ACE_TEXT( "HA_python_bootstrap::info" ) , __LINE__ );


            ACE_TCHAR buf[BUFSIZ];
            ACE_OS::sprintf ( buf,
                              ACE_TEXT ("HA_python_bootstrap listening on port %lu\n"),
                              port() );
            if ( *str == 0 )
            { *str = ACE::strnew (buf); }
            else
            {  ACE_OS::strncpy (*str, buf, len); }

            return static_cast<int>( ACE_OS::strlen (*str) );
}

//--------------------------------------------------------------------------------------
void python_bootstrap_func( void* ptr_instance )
{
               ACE_Trace _( ACE_TEXT( "HA_python_bootstrap func" ) , __LINE__ );


               ACE_DEBUG
                  ((LM_DEBUG, ACE_TEXT ("(%D %t) ..python bootstrap thread..\n")));

               HA_python_bootstrap*  boot = static_cast<HA_python_bootstrap*> ( ptr_instance );

               ACE_Process_Options options;
               std::ostringstream ostr;
               ostr << boot->prv().c_str()
                    << "/cci/dev_t//bin/cci-bootstrap "
                    << "-e &";
               options.command_line( ostr.str().c_str() );
               ACE_Process process;
               pid_t pid = process.spawn( options );

               if( pid != -1 )
               {
                ACE_DEBUG
                  ((LM_DEBUG, ACE_TEXT ("%D (%t) ..python bootstrap succeeded..\n")));
               }

               for( ;; )
               {
                   std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
               }


}



