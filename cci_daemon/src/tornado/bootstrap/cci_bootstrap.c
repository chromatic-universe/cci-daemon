// cci_bootstrap.c     william k. johnson  chromatic universe 2016

// python-dev

#include <Python.h>

//c runtime
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <wchar.h>
#include <unistd.h>
#include <linux/limits.h>

static FILE* ostream_fd;
static int silent = 0 ;


//iinline
#define ENTRYPOINT_MAXLEN 128

char* default_sys_path = "/data/data/com.chromaticuniverse.cci_trinity/files/app";
char* default_python_home = "/data/data/com.chromaticuniverse.cci_trinity/files";
char* default_python_path = "/data/data/com.chromaticuniverse.cci_trinity/files/app/lib/python27.zip:"
                            "/data/data/com.chromaticuniverse.cci_trinity/files/app/python2.7/:"
                            "/data/data/com.chromaticuniverse.cci_trinity/lib/:"
                            "/data/data/com.chromaticuniverse.cci_trinity/files/app/lib/python2.7/lib-dynload/:"
                            "/data/data/com.chromaticuniverse.cci_trinity/files/app/lib/python2.7/site-packages/";
char* trinity = "cci_trinity.py";
char* vulture = "cci_trinity_async.py";
int cci = 0;


//helpers
//------------------------------------------------------------------------
static void stream_out_usage( const char* binary );
//------------------------------------------------------------------------
static int build_py_env();
//------------------------------------------------------------------------
static void cci_logger ( const char* msg  );
//------------------------------------------------------------------------
static void trace( char* atom );
//------------------------------------------------------------------------
static void chk( int py_chk );
//environment
//..----------------------------------------------------------------------
static void set_py_env();


/*static int  dir_exists(char *filename );
//------------------------------------------------------------------------
static int  file_exists( const char *filename );
//------------------------------------------------------------------------
static void trace( char* atom );
//------------------------------------------------------------------------
static void chk( int py_chk );
//------------------------------------------------------------------------
static int py_env_out();
// services
static int validate_entry_point( char* entry );
//------------------------------------------------------------------------
static int open_entry_point( char* entry );*/


// py api
int do_main( int argc , char** argv );
//-------------------------------------------------------------------------
int main( int argc , char** argv )
{
        return do_main( argc , argv );
}


// services
//-------------------------------------------------------------------------
static PyObject *cci_log( PyObject *self , PyObject *args )
{

          char *logstr = NULL;

          if ( !PyArg_ParseTuple( args , "s", &logstr) )
          {
            return NULL;
          }



          Py_RETURN_NONE;

}


//-------------------------------------------------------------------------------
static PyMethodDef cci_methods[] = {
                                               { "log" ,
                                                   cci_log ,
                                                   METH_VARARGS ,
                                                   "cci log"
                                                } ,
                                                { NULL ,
                                                  NULL ,
                                                  0 ,
                                                  NULL
                                                }
                                           };



//-------------------------------------------------------------------------------
#if PY_MAJOR_VERSION >= 3
    static struct PyModuleDef cciprivate = {
                                               PyModuleDef_HEAD_INIT ,
                                               "cciprivate" ,
                                               "" ,
                                               -1 ,
                                               cci_methods
                                              };
    //--------------------------------------------------------------------------
    PyMODINIT_FUNC init_cci_private( void )
    {
          return PyModule_Create( &cciprivate );
    }
#else
//---------------------------------------------------------------------------
    PyMODINIT_FUNC init_cci_private( void )
    {
          (void) Py_InitModule( "cciprivate" , cci_methods );
    }
#endif



//-------------------------------------------------------------------------
int do_main( int argc , char** argv )
{




              char *env_argument = NULL;
              char *env_entrypoint = NULL;
              char *env_logname = NULL;
              char entrypoint[ENTRYPOINT_MAXLEN];
              int ret = 0;
              FILE *fd;
              int use_default_env = 0;
              int opt = 0;




              while ( ( opt = getopt( argc ,
                                      argv ,
                                      "sevh" ) ) != -1 )
              {
                         switch( opt )
                         {

                            case 'e':
                                use_default_env = 0;
                                break;
                            case 's':
                                silent = 1;
                                break;
                            case 'v':
                                fprintf( stderr , "\033[22;32mcci-bootstrap version 0.8 william k. johnson 20176\n\033[0m" );
                                exit( 0 );
                            case 'h' :
                            default :
                            {
                                //executable moniker as arg
                                stream_out_usage( argv[0] );

                                exit( 0 );
                            }
                         }
              }

              ostream_fd = fopen( "cci-bootstrap.log" , "wr" );
              if( fd == NULL )
              {
                  trace( "...could not open log file for writing..." );

                  exit( 1 );
              }
              trace( "...opened log file for writing..." );


              set_py_env();


              trace( "initialize python...cci.." );
              env_argument = getenv( "CCI_PRIVATE_PYTHON" );
              env_entrypoint = getenv( "CCI_ENTRYPOINT" );

              trace( "changing directory to the one provided by CCI_PRIVATE_PYTHHON");
              ret = chdir( env_argument ) ;
              if( ret == -1 ) { trace( "...change directory failed..." ); }
              else{ trace( env_argument ); }


              Py_SetProgramName( "cci_python" );
              trace( "Py_SetProgramName...cci..." ) ;

              //instance will abort on failure
              trace( "...Py_Initialize..." );
              Py_Initialize();



#if PY_MAJOR_VERSION < 3
              PySys_SetArgv( argc, argv );
#endif

              trace( "initialized python" );

              // ensure threads will work
              trace( "...PyEval_InitThreads()..." );

              PyEval_InitThreads();
              trace( "...py threads ok..." );

#if PY_MAJOR_VERSION < 3
              trace( "... init_cci_private();..." );
              init_cci_private();
#endif
              /*
              int py = PyRun_SimpleString( "import androidembed\nandroidembed.log('testing python "
                                   "print redirection')" ) ;
              chk( py );
              trace( "... PyRun_SimpleString...ok...import androidembed.." );

              // inject our bootstrap code to redirect python stdin/stdout
              // replace sys.path with our path
              py = PyRun_SimpleString( "import sys, posix\n" );
              chk( py );

              LOGP( "setting up python from private environment..." );
              trace( "... PyRun_SimpleString...ok...import sys , posix modules.." );

              trace( "... build_py_env()..." );
              chk( build_py_env() );
              trace( "... py_env_out()..." );
              chk( py_env_out() );

              trace( "... validate_entry_point()..." );
              chk( validate_entry_point( env_entrypoint ) );


              trace( "... open_entry_point()...ok..." );


              trace( "... open_entry_point()..." );
              chk( open_entry_point( env_entrypoint ) );


              trace( "...fini..." );

              Py_Finalize();*/


              return 0;
}



//-------------------------------------------------------------------------
void cci_logger ( const char* msg  )
{
            struct timeval tv;
            gettimeofday( &tv, NULL );

            fprintf( ostream_fd ,
                     "\033[22;32m%u.%03u CCIPYTHONBOOTSTRAP-\033[0m: %s\n" ,
                     (int) tv.tv_sec ,
                     (int) ( tv.tv_usec / 1000 ) ,
                     msg );

}


//----------------------------------------------------------------------
int build_py_env()
{
             //LOGP( "setting up python from " );
             return PyRun_SimpleString(   "private = posix.environ['CCI_PRIVATE']\n"
                                           "argument = posix.environ['CCI_PRIVATE_PYTHON']\n"
                                           "sys.path[:] = [ \n"
                                           //"    private + '/lib/python36.zip', \n"
                                           "    private + '/lib/python/3.6', \n"
                                           "    private + '/lib/python3.6/lib-dynload/', \n"
                                            "    private + '/bin/', \n"
                                           "    private + '/lib/python3.6/site-packages/', \n"
                                           "    argument ]\n"
                                       );

}

//-------------------------------------------------------------------------
void chk( int py_chk )
{
             if( py_chk == 0 ) { trace( "...py stack call ok..." ); }
             else if( py_chk == -1 )
             {
                 trace( "....error in py stack...exiting..." );
                 exit( 1 );
             }
             else{ exit( 1 ); }
}


//------------------------------------------------------------------------
void stream_out_usage( const char* binary )
{
             //stream , small subset of options
             fprintf(   stderr ,
                        "\033[22;32m%s\nusage:\n\t\t-esvh\n" \
                        "binary: %s\n" \
                        "\n\033[0m" \
                        " options:\n" \
                        "  -e  use default settings , ignore environment\n" \
                        ") -s  silent\n" \
                        "  -v  version\n"\
                        "  -h  help\n\n" \
                        "\n" \
                        "\n" ,
                        "synopsis :\n"\
                            "\n\t\t bootstrapper for python - cci-daemon-dispatcher\n"\
                        "\t\t chromatic universe 2017\n" ,
                        binary
                    );
}

//----------------------------------------------------------------------
void trace( char* atom )
{

             if( silent ) { return; }

             // calendar time
             time_t ltime;
             //get current cal time
             ltime = time( NULL );

             fprintf( stderr ,
                     "trace=> %s  %s\n" ,
                     asctime( localtime( &ltime ) ) ,
                     atom );
             cci_logger( atom );
}

//--------------------------------------------------------------------------
void set_py_env()
{

             char path_buf[PATH_MAX];
             char path_entry[1024];

             char* p_path;
             p_path = getenv ( "CCI_PRIVATE" );
             if ( p_path != NULL )
             {
                trace( "..setting python path...." );
                sprintf( path_entry , "%s/lib/python3.6/lib-dynload/:" , p_path );
                strcat( path_buf , path_entry );
                memset( path_entry , '\0' , strlen( path_entry ) );
                sprintf( path_entry , "%s/lib/python3.6/lib/:" , p_path );
                strcat( path_buf , path_entry );
                memset( path_entry , '\0' , strlen( path_entry ) );
                sprintf( path_entry , "%s/lib/python3.6/site-packages/:" , p_path );
                strcat( path_buf , path_entry );
                memset( path_entry , '\0' , strlen( path_entry ) );
                sprintf( path_entry , "%s/bin/:" , p_path );
                strcat( path_buf , path_entry );

                p_path = getenv ( "CCI_PRIVATE_PYTHON" );
                if ( p_path != NULL )
                {
                    strcat( path_buf , p_path );
                }

                trace( "... setenv..PYTHONPATH" );
                chk( setenv( "PYTHONPATH"  , path_buf , 1 ) );
                trace( path_buf );


                trace( "... setenv..CCI_ENTRYPOINT" );
                chk( setenv( "CCI_ENTRYPOINT" , trinity , 1 ) );

             }


}


/*

//---------------------------------------------------------------------------
int dir_exists(char *filename)
{
      struct stat st;
      if ( stat( filename , &st ) == 0 )
      {
        if ( S_ISDIR( st.st_mode ) ) { return 1; }
      }

      return 0;
}

//---------------------------------------------------------------------------
int file_exists( const char *filename )
{
      FILE *file;
      if ( file = fopen( filename , "r" ) )
      {
        fclose( file );

        return 1;
      }

      return 0;
}





//---------------------------------------------------------------------------
int py_env_out()
{

      return PyRun_SimpleString(
          "class LogFile(object):\n"
          "    def __init__(self):\n"
          "        self.buffer = ''\n"
          "    def write(self, s):\n"
          "        s = self.buffer + s\n"
          "        lines = s.split(\"\\n\")\n"
          "        for l in lines[:-1]:\n"
          "            androidembed.log(l)\n"
          "        self.buffer = lines[-1]\n"
          "    def flush(self):\n"
          "        return\n"
          "sys.stdout = sys.stderr = LogFile()\n"
          "print('Android path', sys.path)\n"
          "import os\n"
          "print('os.environ is', os.environ)\n"
          "print('cci bootstrap done. __name__ is', __name__)");

}

//-----------------------------------------------------------------------------
int validate_entry_point( char* env_entrypoint )
{
      char entrypoint[ENTRYPOINT_MAXLEN];

      char *dot = strrchr( env_entrypoint , '.' );
      if ( dot <= 0 )
      {
        LOGP( "invalid entrypoint, abort." );
        return -1;
      }

      if ( strlen( env_entrypoint ) > ENTRYPOINT_MAXLEN - 2)
      {
          LOGP( "entrypoint path is too long, try increasing ENTRYPOINT_MAXLEN." );
          return -1;
      }

      if ( !strcmp (dot, ".pyo" ) )
      {
        if ( !file_exists( env_entrypoint ) )
        {
          //fallback on .py
          strcpy( entrypoint , env_entrypoint );
          entrypoint[strlen(env_entrypoint) - 1] = '\0';
          LOGP( entrypoint );
          if ( !file_exists( entrypoint ) )
          {
            LOGP( "Entrypoint not found (.pyo, fallback on .py), abort" );
            return -1;
          }
        }
        else
        {
          strcpy( entrypoint, env_entrypoint );
        }
      }
      else if (!strcmp(dot, ".py") )
      {
        //if .py is passed, check the pyo version first
        strcpy(entrypoint, env_entrypoint);
        entrypoint[strlen(env_entrypoint) + 1] = '\0';
        entrypoint[strlen(env_entrypoint)] = 'o';
        if (!file_exists( entrypoint ) )
        {
          // fallback on pure python version
          if ( !file_exists(env_entrypoint) )
          {
            LOGP( "Entrypoint not found (.py), abort.");
            return -1;
          }
          strcpy( entrypoint , env_entrypoint );
        }
      }
      else
      {
        LOGP( "Entrypoint have an invalid extension (must be .py or .pyo), abort.");
        return -1;
      }

      return 0;

}


//------------------------------------------------------------------------------------
int open_entry_point( char* entry )
{
      trace( entry );

      FILE *fd;

      fd = fopen( entry , "r" );
      if ( fd == NULL )
      {
        LOGP( "ppen the entrypoint failed" );
        LOGP( entry );

        return -1;
      }

      int ret = PyRun_SimpleFile( fd , entry );
      if ( PyErr_Occurred() != NULL )
      {
        ret = 1;
        // this exits with the right code if sys exit
        PyErr_Print();
        PyObject *f = PySys_GetObject( "stderr" );
        if ( PyFile_WriteString( "\n", f ) ) { PyErr_Clear(); }
      }

}*/


