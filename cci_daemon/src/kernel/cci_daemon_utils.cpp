//cci_daemon_utils.cpp      william k. johnson 2017

#include <cci_daemon_utils.h>

using namespace cci_daemon_utils;


//static void blocking_read( int cp_fd[] , std::ostream& ostr );


//-------------------- )--------------------------------------------------------------
//c binding -
void buffer_stream_from_open( const std::string& params ,
                              const std::string& file_name ,
                              std::ostream& ostr ,
                              reading r ,
                              int& pid_ )
{

           std::string out_str { "error_init"  };
           pid_t     pid;
           int       parent_to_child[2];
           int       child_to_parent[2];

           assert( pipe( parent_to_child ) == 0 );
           assert( pipe( child_to_parent ) == 0 );

           switch( pid = fork()  )
           {
               case -1 :

                  out_str = "fork failed";
                  break;

                case 0 :

                  //child
                  assert( dup2( parent_to_child[ read_fd  ], STDIN_FILENO  ) != -1 );
                  assert( dup2( child_to_parent[ write_fd ], STDOUT_FILENO ) != -1 );
                  assert( dup2( child_to_parent[ write_fd ], STDERR_FILENO ) != -1);
                  assert( close( parent_to_child[ write_fd ] ) == 0 );
                  assert( close( child_to_parent [ read_fd ] ) == 0 );

                  //child creation failed if we're here
                  out_str = "error in child process creation";
                  break;

                default :

                  //parent
                  assert( close( parent_to_child[ read_fd ] ) == 0 );
                  assert( close( child_to_parent [ write_fd ] ) == 0 );




           }


}

//-------------------------------------------------------------------------------------
/*void blocking_read( int cp_fd[] ,  std::ostream& ostr )
{

          std::string    data_read_from_child;
          char           buffer[ buffer_size + 1 ];
          ssize_t        read_result;


          while ( true )
          {
            switch ( read_result = read( cp_fd[ read_fd ] ,
                                         buffer ,
                                         buffer_size ) )
            {

                  case 0: //End-of-File, or non-blocking read.
                    cout << "End of file reached..."         << endl
                         << "Data received was ("
                         << dataReadFromChild.size() << "):" << endl
                         << dataReadFromChild                << endl;

                    ASSERT_IS( pid, waitpid( pid, & status, 0 ) );

                    cout << endl
                         << "Child exit staus is:  " << WEXITSTATUS(status) << endl
                         << endl;

                    exit(0);


                  case -1:
                    if ( (errno == EINTR) || (errno == EAGAIN) )
                    {
                      errno = 0;
                      break;
                    }
                    else
                    {
                      FAIL( "read() failed" );
                      exit(-1);
                    }

                  default:
                    //dataReadFromChild . append( buffer, readResult );
                    break;

            }
          }



}*/


