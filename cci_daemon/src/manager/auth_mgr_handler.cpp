
//proc_ace_handler.h chromatic universe 2017 william k. johnson

#include <proc_ace_acceptor.h>

using namespace proc_ace;



//suppress from adaptive communication environment
#pragma clang diagnostic ignored "-Wself-assign"
#pragma clang diagnostic ignored "-Wconstant-logical-operand"


using namespace proc_ace;
using std::ostringstream;

using std::string;
using std::ends;
using std::ostringstream;
using std::wostringstream;
using std::wstring;
using std::make_pair;
using std::cout;
using std::endl;
using std::pair;
using std::set;
using std::ofstream;
using std::chrono::milliseconds;
using std::unique_ptr;


#define REGISTER_MASK ACE_Event_Handler::READ_MASK
#define REMOVE_MASK (ACE_Event_Handler::READ_MASK | ACE_Event_Handler::DONT_CALL)


//---------------------------------------------------------------------------------------
proc_handler::proc_handler ( const ACE_Time_Value& max_inactivity ) :   m_proc_acceptor{ nullptr } ,
                                                                        m_current_command{ proc_command::proc_no_command } ,
                                                                        m_proc_packet_error{ proc_packet_error::peNoPacketError } ,
                                                                        m_dwTimerToken{ 0 } ,
                                                                        m_maximumTimeToWait{ max_inactivity } ,
                                                                        m_bSilent{ false } ,
                                                                        creator_ { ACE_Thread::self() }
{

              ACE_Trace _( ACE_TEXT( "proc_handler::proc_handler" ) , __LINE__ );

              try
              {
                  char hostname[1024];
                  gethostname( hostname , 1023 );
                  m_strComputerName = hostname;
                  m_currentState.insert( proc_state::oesNonAuthenticated );
              }
              catch( ... )

              {
                 //
              }

              m_set_map.insert( make_pair( proc_set_param::ospPrompt , true ) );
}

//---------------------------------------------------------------------------------------
proc_handler::~proc_handler ()
{
            ACE_Trace _( ACE_TEXT( "proc_handler::~proc_handler" ) , __LINE__ );

            peer().close();
}

//---------------------------------------------------------------------------------------
concurrency_t proc_handler::concurrency()
{           return ( acceptor()->concurrency () );}

//---------------------------------------------------------------------------------------
proc_thread_pool_ptr proc_handler::thread_pool()
{           return ( acceptor()->thread_pool () ); }

//---------------------------------------------------------------------------------------
void proc_handler::destroy ()
{
           ACE_Trace _( ACE_TEXT( "proc_handler::destroy" ) , __LINE__ );
           reactor()->remove_handler ( this, REMOVE_MASK );


           delete this;
}


//---------------------------------------------------------------------------------------
void  proc_handler::output_boiler_plate( string str_concurrency )
{


              stream() << "* OK cci-dispatch daemon speaking.... "
                       << computer_name()
                       << " chromatic universe 2017 all rights reserved  ready";
              stream() << crlf;
              stream() << output_prompt( false );

              flush_stream_to_connector();

}

//---------------------------------------------------------------------------------------
string proc_handler::output_prompt( bool btoken )
{

              ostringstream ostr;

              if( btoken == true )
              {
                    ostr << token() ;
                    ostr << ends;
              }


              return ( ostr.str() );
}

//---------------------------------------------------------------------------------------
void proc_handler::map_commands()
{

            command_map().insert( std::pair<string , ptr_to_function> ( "login", &proc_handler::on_login ) );
            /*command_map().insert( std::pair<string , ptrToFunction> ( "logout", &chromatic_handler::on_logout ) );
            command_map().insert( std::pair<string , ptrToFunction> ( "create", &chromatic_handler::on_create ) );
            command_map().insert( std::pair<string , ptrToFunction> ( "select", &chromatic_handler::on_select ) );
            command_map().insert( std::pair<string , ptrToFunction> ( "examine", &chromatic_handler::on_examine ) );
            command_map().insert( std::pair<string , ptrToFunction> ( "list", &chromatic_handler::on_list ) );
            command_map().insert( std::pair<string , ptrToFunction> ( "delete", &chromatic_handler::on_delete ) );
            command_map().insert( std::pair<string , ptrToFunction> ( "rename", &chromatic_handler::on_rename ) );
            command_map().insert( std::pair<string , ptrToFunction> ( "lsub", &chromatic_handler::on_lsub ) );
            command_map().insert( std::pair<string , ptrToFunction> ( "subscribe", &chromatic_handler::on_subscribe ) );
            command_map().insert( std::pair<string , ptrToFunction> ( "unsubscribe", &chromatic_handler::on_unsubscribe ) );
            command_map().insert( std::pair<string , ptrToFunction> ( "noop", &chromatic_handler::on_noop ) );
            command_map().insert( std::pair<string , ptrToFunction> ( "check", &chromatic_handler::on_check ) );
            command_map().insert( std::pair<string , ptrToFunction> ( "capability", &chromatic_handler::on_capability ) );
            command_map().insert( std::pair<string , ptrToFunction> ( "search", &chromatic_handler::on_search ) );
            command_map().insert( std::pair<string , ptrToFunction> ( "copy", &chromatic_handler::on_copy ) );
            command_map().insert( std::pair<string , ptrToFunction> ( "fetch", &chromatic_handler::on_fetch ) );
            command_map().insert( std::pair<string , ptrToFunction> ( "store", &chromatic_handler::on_store ) );
            command_map().insert( std::pair<string , ptrToFunction> ( "uid", &chromatic_handler::on_uid ) );
            command_map().insert( std::pair<string , ptrToFunction> ( "close", &chromatic_handler::on_close ) );
            command_map().insert( std::pair<string , ptrToFunction> ( "expunge", &chromatic_handler::on_expunge ) );
            command_map().insert( std::pair<string , ptrToFunction> ( "append", &chromatic_handler::on_append ) );
            command_map().insert( std::pair<string , ptrToFunction> ( "starttls", &chromatic_handler::on_starttls ) );
            command_map().insert( std::pair<string , ptrToFunction> ( "status", &chromatic_handler::on_status ) );*/

}

//---------------------------------------------------------------------------------------
void  proc_handler::flush_stream_to_connector()
{
            ACE_Trace _( ACE_TEXT( "proc_handler::flush_stream_to_connector" ) , __LINE__ );

            if( silent() == true )
            {
                stream().str( "" );

                return;
            }

            ACE_Time_Value timeout( 2 );
            size_t bytes_sent;
            string strOutput ( stream().str() );
            peer().send_n( (char*) strOutput.c_str() , strOutput.size() , &timeout , &bytes_sent );

            stream().str( "" );
}

//---------------------------------------------------------------------------------------
void  proc_handler::flush_binary_stream_to_connector( unsigned char* u_buffer , unsigned long dwLen )
{
            if( silent() == true )
            {
                stream().str( "" );

                return;
            }

            ACE_Time_Value timeout( 5 );

            peer().send( (unsigned char*) u_buffer , dwLen , &timeout );
}

//---------------------------------------------------------------------------------------
string proc_handler::client_addr()
{
            ACE_INET_Addr addr;
            char addrstr[MAXHOSTNAMELEN + 1];

            peer().get_remote_addr( addr );
            addr.addr_to_string( addrstr , sizeof( addrstr ) );

            string strTemp( addrstr );

            return ( strTemp );
}


//---------------------------------------------------------------------------------------
int proc_handler::open ( void* accept )
{

              ACE_Trace _( ACE_TEXT( "proc_handler::open" ) , __LINE__ );
              output_boiler_plate();

              map_commands();
              silent( false );

              acceptor ( ( proc_acceptor_ptr) accept);

              string strConcurrency;

              //check for timeout every 30 seconds
              ACE_Time_Value reschedule( m_maximumTimeToWait.sec() / 60 );

              m_dwTimerToken = reactor()->schedule_timer( this ,
                                                          (void*) timer_id ,
                                                          m_maximumTimeToWait ,
                                                          reschedule );

              if ( concurrency () == concurrency_t::thread_per_connection_ )
              {
                return activate ( THR_DETACHED );
              }

              reactor ( acceptor()->reactor () );

              ACE_INET_Addr addr;
              if ( peer ().get_remote_addr (addr) == -1 ) { return ( -1 ); }

              if ( reactor ()->register_handler ( this,
                                                  REGISTER_MASK ) == -1 )
              {
                ACE_ERROR_RETURN ((LM_ERROR,
                                   "(%P|%t) can't register with reactor\n"),
                                  -1);
              }

              ACE_DEBUG ((LM_DEBUG,
                          "(%P|%t) connected with %s\n",
                          addr.get_host_name () ) );



              return ( 0L );

}

//---------------------------------------------------------------------------------------
int proc_handler::close(u_long flags)
{
              ACE_Trace _( ACE_TEXT( "proc_handler::closed" ) , __LINE__ );
              ACE_UNUSED_ARG( flags );


              destroy ();

              return ( 0L );
}

//----------------------------------------------------------------------------------------
int proc_handler::handle_input (ACE_HANDLE handle)
{
              ACE_Trace _( ACE_TEXT( "proc_handler::handle_input" ) , __LINE__ );

              ACE_UNUSED_ARG ( handle );

              m_lastActivityTime = reactor()->timer_queue()->gettimeofday();

              if ( concurrency () ==  concurrency_t::thread_pool_ )
              {
                 if ( ACE_OS::thr_equal (ACE_Thread::self(),
                                         creator_) )
                {
                      reactor ()->remove_handler ( this, REMOVE_MASK );

                      return ( thread_pool ()->enqueue ( this ) );
                }
              }

              char buf[cmd_buffer_len];
              int rval = process ( buf, sizeof (buf) );
              if ( concurrency () ==  concurrency_t::thread_pool_ )
              {
                  if ( rval != -1 )
                  {
                    reactor ()->register_handler ( this,
                                                   REGISTER_MASK );
                  }
              }

              return rval;
}

//----------------------------------------------------------------------------------------
int proc_handler::handle_close( ACE_HANDLE handle,
                                 ACE_Reactor_Mask mask )
{
              ACE_Trace _( ACE_TEXT( "proc_handler::handle_close" ) , __LINE__ );

              ACE_UNUSED_ARG (handle);
              ACE_UNUSED_ARG (mask);

              stream() << " * goodbye "
                       << client_addr()
                       << "......chromatic unvierse cci-daemon-dispatcher" << crlf;
              flush_stream_to_connector();

              this->destroy ();
              return 0;
}

//----------------------------------------------------------------------------------------
int proc_handler::handle_timeout( const ACE_Time_Value& now , const void* act )
{
                ACE_Trace _( ACE_TEXT( "proc_handler::handle_timeout" ) , __LINE__ );

                short unsigned int* sui = (short unsigned int*) act;
                if( *sui == (short unsigned int) timer_id )
                {
                    if( ( now - m_lastActivityTime ) >= m_maximumTimeToWait )
                    {
                        cout << client_addr() << " timed out" << endl;
                        stream() <<  " autologout; idle for too long" << crlf;
                        flush_stream_to_connector();
                        reactor()->remove_handler( this , ACE_Event_Handler::READ_MASK );

                        this->destroy ();
                    }
                }

                return ( 0 );
}

//---------------------------------------------------------------------------------------
int proc_handler::svc()
{
              ACE_Trace _( ACE_TEXT( "proc_handler::svc" ) , __LINE__ );

              char buf[BUFSIZ];

              //forever...
              while( 1 )
                {

                  if ( this->process(buf, sizeof (buf)) == -1 )
                  {
                    return -1;

                  }
                  milliseconds dura( 50 );
                  std::this_thread::sleep_for( dura );
                }

              return 0;
}

//--------------------------------------------------------------------------------------
int proc_handler::process ( char *rdbuf ,  int rdbuf_len )
{

              //process request

              ssize_t bytes_read;
              const  char ctrl_c = 3;
              const  char ctrl_z = 26;
              //memory stream - the stream buffer could be very large , put it on the heap
              auto ostr = std::make_unique<std::ostringstream>() ;
              //we'll only wait this
              ACE_Time_Value timeout( 50 );
              memset( (void*) rdbuf , '\0' , rdbuf_len );
              error_packet( proc_packet_error::peNoPacketError );

              //read buffer
              while ( ( bytes_read = peer().recv ( rdbuf, rdbuf_len , &timeout) ) > 0 )
              {
                   *ostr << rdbuf;

                   //zero read buufer
                   memset( (void*) rdbuf , '\0' , sizeof( rdbuf_len ) );
                   string::size_type st = ostr->str().find( ctrl_c );
                   string::size_type dt = ostr->str().find( ctrl_z );

                   //ctrl-c,ctrl-z
                   //trap for console termination
                   if( ( st != string::npos ) || ( dt != string::npos) )
                   {
                       return ( -1 );
                   }

                   //request terminator
                   st = ostr->str().find( "\n" );
                   if( st == string::npos )
                   {
                       if( ostr->str().size() <= constCommandBufferLength )
                       {
                            continue;
                       }
                   }


                   if( bytes_read > 0 )
                   {

                       //execute
                       perform( ostr->str() );

                       ACE_DEBUG ((LM_DEBUG, "\n\t\t------------------------------\n",
                                   (char*) ostr->str().c_str() ) );

                       //just a line feed or waiting - ignore
                       if( ( ostr->str().find ( "\n" ) != string::npos ) && ( ostr->str().size() == 2L ) )
                       {
                            stream() << output_prompt( false );
                            flush_stream_to_connector();

                            break;
                       }

                       //logout
                       if( command() != proc_command::proc_logout )
                       {
                            //fini
                            if( error_packet() != proc_packet_error::peNoPacketError )
                            {
                                format_packet_error( error_packet() );

                                break;

                            }

                        }
                        else
                        {
                            //snafu
                            flush_stream_to_connector();

                            return ( -1 );
                        }

                        flush_stream_to_connector();
                        ostr->str( "" );

                        break;
                   }

            }
            //set buffer to empty
            ostr->str( "" );
            ostr.release();

            return ( 0L );

}

//---------------------------------------------------------------------------------------
void proc_handler::perform( const string& str )
{

            if( str.empty() )
            {
                error_packet( proc_packet_error::peInvalidPayload );

                return;
            }

            unsigned short uscommands = 0;
            map_of_parts PartsMap;

            //parse the packet
            parse_command( str , PartsMap , uscommands );
            if( error_packet() != proc_packet_error::peNoPacketError )
            {
                return;
            }

            //call the required function
            map_of_parts::iterator pIter = PartsMap.find( proc_packet_part::ppCommand );
            if( pIter != PartsMap.end() )
            {
                string strcommand( pIter->second );
                strcommand = lcase( strcommand );

                map_of_commands::iterator c_iter = command_map().find( strcommand );
                if( c_iter  == command_map().end() )
                {
                    error_packet( proc_packet_error::peUnrecognizedCommand );

                    return;
                }

                proc_handler::ptr_to_function ptr_func = c_iter->second;
                if( ptr_func == NULL )
                {
                    stream() << output_prompt( false );
                    stream() << "NO no default action for this keyword" << crlf;
                    flush_stream_to_connector();

                    return;
                }

                //params
                pIter = PartsMap.find( proc_packet_part::ppPayload );
                string strPayload;

                pIter != PartsMap.end() ? strPayload = pIter->second : strPayload = "";
                int	lRetVal = ( this->*ptr_func ) ( strPayload );

                if( ( lRetVal != 0 ) || ( command() == proc_command::proc_error ) )
                {
                    return;
                }

            }
            else
            {
                return;
            }

            return;
}

//---------------------------------------------------------------------------------------
void proc_handler::format_packet_error( proc_packet_error pe )
{

            command( proc_command::proc_error );
            stream() << output_prompt();

            stream() << " NO ";

            if( pe == proc_packet_error::peNoToken ) stream() << "no token supplied - expected";
            else if( pe == proc_packet_error::peTokenTooLong) stream() << "token too long";
            else if( pe == proc_packet_error::peNoCommand ) stream() << "no command supplied - expected";
            else if( pe == proc_packet_error::peCommandTooLong ) stream() << "buffer to large";
            else if( pe == proc_packet_error::peTooManyParameters ) stream() << "too many parameters supplied";
            else if( pe == proc_packet_error::peInvalidPayload ) stream() << "invalid buffer";
            else if( pe == proc_packet_error::peInvalidSyntax ) stream() << "invalid syntax";
            else if( pe == proc_packet_error::peUnrecognizedCommand ) stream() << "command unrecognized";
            else if( pe == proc_packet_error::peSelectState ) stream() << " a target must be selected before a query";
            else if( pe == proc_packet_error::peTooFewParameters ) stream() << " too few parameters - more";
            else if( pe == proc_packet_error::peNoParametersExpectedGotSome ) stream() << " no parameters expected - got some";
            else if( pe == proc_packet_error::peParametersExpectedGotNone ) stream() << "parameters expected - got none";
            else if( pe == proc_packet_error::peFailure ) stream() << "a server operation within this context failed";
            else if( pe == proc_packet_error::peInvalidObject ) stream() << "object does not exist";
            else if( pe == proc_packet_error::peInvalidSpecification ) stream() << "invalid specification";
            else if( pe == proc_packet_error::peNoSpecification ) stream() << "no specification";
            else if( pe == proc_packet_error::peInvalidState ) stream() << "invalid state";
            else if( pe == proc_packet_error::peAttributeNotSupported ) stream() << "attribute not supported";
            else if( pe == proc_packet_error::peNotAuthenticated ) stream() << "please login";
            else if( pe == proc_packet_error::peAlreadyAuthenticated ) stream() << "already logged in";
            else if( pe == proc_packet_error::peNotEnoughPrivileges ) stream() << "not enough privileges";


            stream() << crlf;
            stream() << output_prompt( false );

            flush_stream_to_connector();
}

//---------------------------------------------------------------------------------------
void proc_handler::stream_out( const std::string& str )
{
	        stream() << str;
}

void proc_handler::stream_out( const byte_ptr& ptrBytes ,  unsigned long len )
{

	        //todo

}

//----------------------------------------------------------------------------------------
void proc_handler::stream_out( const std::string& str , unsigned long len )
{
	        stream() << str.substr( 0 , len );
}

//----------------------------------------------------------------------------------------
void proc_handler::flush_immediate( const string& atom )
{
            stream() << atom;

            flush_stream_to_connector();

}


//---------------------------------------------------------------------------------------
void proc_handler::parse_command( const  string& packet ,
								  map_of_parts& parts ,
								  unsigned short& uscommandParamCount )
{
            if( packet.empty() )
            {
                error_packet( proc_packet_error::peInvalidSyntax );

                return;
            }

            string space( " " );
            string str( packet );

            ACE_DEBUG ((LM_DEBUG,
                          "\n\t------------------------Client Packet------------------------\n" ) );
            ACE_DEBUG ((LM_DEBUG,
                          "(%P|%t) %s %s\n" , (char*) token().c_str() , (char*) str.c_str() ) );

            //get the token
            string::size_type idx = str.find_first_of( space );
            if( idx != string::npos )
            {
                string strtoken;
                strtoken = str.substr( 0 , idx );
                if( strtoken.size() > constMaxTokenLength )
                {
                    error_packet( proc_packet_error::peTokenTooLong );

                    return;
                }

                parts.insert( make_pair( proc_packet_part::ppToken , strtoken ) );
                token( strtoken );

                //truncate
                idx++;
                str = str.substr( idx );
                if( str.size() == 0 )
                {
                    error_packet( proc_packet_error::peInvalidSyntax );

                    return;
                }

                //get the command
                string strcommand;
                idx = str.find_first_of( space );
                if( idx == string::npos )
                {
                    //no params
                    uscommandParamCount = 0;
                    strcommand = str;
                    rtrimlast( strcommand , "\n" );
                    command_str( strcommand );

                    parts.insert( make_pair( proc_packet_part::ppCommand , strcommand ) );
                }
                else
                {
                    strcommand = str.substr( 0 , idx );


                    command_str( strcommand );

                    //truncate
                    idx++;
                    str = str.substr( idx );

                    parts.insert( make_pair( proc_packet_part::ppCommand , strcommand ) );
                    chomp( str , "\n" );
                    parts.insert( make_pair( proc_packet_part::ppPayload , str ) );
                }
            }
            else
            {
                error_packet( proc_packet_error::peNoCommand );

                return;
            }

            error_packet( proc_packet_error::peNoPacketError );

}

//---------------------------------------------------------------------------------------
void proc_handler::output_ok_response( const string& ack )
{
	        stream() << token() << " " << "OK " << ack << crlf;
}

//---------------------------------------------------------------------------------------
bool proc_handler::check_connection_state( proc_state state )
{
            bool bRet = false;

            current_state::iterator iter = m_currentState.find( state );
            iter == m_currentState.end() ? bRet = false  : bRet = true;

            return ( bRet );
}

//---------------------------------------------------------------------------------------
void proc_handler::update_connection_state( proc_set_op op , proc_state state )
{
            switch( op )
            {
                case proc_set_op::osopAdd:
                    m_currentState.insert( state );
                    break;
                case proc_set_op::osopDelete:
                    m_currentState.erase( state );
                    break;
                case proc_set_op::osopClear:
                    m_currentState.clear();
                    break;
            }
}


//---------------------------------------------------------------------------------------
void proc_handler::output_no_response( const string& nack )
{
	        stream() << token() << " " << "NO " << nack << crlf;
	        command_str( "" );
}

//---------------------------------------------------------------------------------------
void proc_handler::chomp( string& str , const string& token )
{
            string strTemp( str );

            if( str.empty() )
            {
                return;
            }

            string::size_type idx = str.find( token );
            if( idx != string::npos )
            {
                str = str.substr( 0 , --idx );
            }

}

//---------------------------------------------------------------------------------------
void proc_handler::gnaw( string& str , const string& token )
{
            string::size_type st;

            st = str.find( token , 0 );

            if( st != string::npos )
            {
                str.resize( st );
            }
}

//---------------------------------------------------------------------------------------
void proc_handler::rtrimlast( string& str , const string& token )
{
            if ( str.empty() )
            {
                return;
            }

            string::size_type idx = str.find_last_of( (char*) token.c_str() );

            //if not the last ignore
            if( idx != str.size() - 1 )
            {
                return;
            }

            if( idx != string::npos )
            {
                str = str.substr( 0 , --idx );
            }
}

//---------------------------------------------------------------------------------------
string proc_handler::ltrim( const string &szToTrim, const string& szTrimChars )
{
            string szToReturn = szToTrim;

            while( strchr( szTrimChars.c_str(), szToReturn[0] ) != NULL )
            {
                if( szToReturn.length() < 1 )
                {
                    return ( "" );
                }

                szToReturn.replace( 0, 1, "" );
            }

            return szToReturn;
}

//---------------------------------------------------------------------------------------
string proc_handler::rtrim( const string &szToTrim, const string& szTrimChars )
{
            string szToReturn = szToTrim;

            int nLastPos = szToReturn.length() - 1;

            if( nLastPos <= 0 )
            {
                return ( "" );
            }

            while( strchr( szTrimChars.c_str(), szToReturn[nLastPos] ) != NULL )
            {
                szToReturn.replace( nLastPos, 1, "" );

                nLastPos = szToReturn.length() - 1;

                if( nLastPos <= 0 )
                {
                    return ( "" );
                }
            }

            return szToReturn;
}

//---------------------------------------------------------------------------------------
string proc_handler::lcase( const string& str )
{
            string localstr( str );

            if( localstr.empty() )
            {
                return( "" );
            }

            string::iterator iter = localstr.begin();
            while ( iter != localstr.end() )
            {
                *iter = tolower( *iter );
                iter++;
            }

            return ( localstr );
}


//---------------------------------------------------------------------------------------
string proc_handler::ucase( const string& str )
{
            string localstr( str );

            if( localstr.empty() )
            {
                return( "" );
            }

            string::iterator iter = localstr.begin();
            while ( iter != localstr.end() )
            {
                *iter = toupper( *iter );
                iter++;
            }

            return ( localstr );
}

//---------------------------------------------------------------------------------------
string proc_handler::extract_quoted_string( const string& str )
{
           string s( str );
           if( s.empty() )
           {
               return ( s );
           }

           string::size_type idx = s.find_first_of( "\"" );
           if( idx != string::npos )
           {
                s.erase( idx , 1 );
                idx = s.find_last_of( "\"" );
                if( idx != string::npos )
                {
                    s.erase( idx , 1 );
                }
           }


           return ( s );

}



//demultiplex handlers
//---------------------------------------------------------------------------------------
int proc_handler::on_login( const string& params )
{
        ACE_Trace _( ACE_TEXT( "chromatic_handler::on_login" ) , __LINE__ );

        command( proc_command::proc_login );

        if( check_connection_state( proc_state::oesAuthenticated ) == true )
        {
            error_packet( proc_packet_error::peAlreadyAuthenticated );

            return ( 0 );
        }

        ostringstream ostr;

        //todo
        bool bRet = true;// context_ptr()->on_login( params , ostr.str() , stream() );
        if( bRet == true )
        {
            update_connection_state( proc_set_op::osopAdd , proc_state::oesAuthenticated );
            output_ok_response( "login completed" );
        }
        else
        {
            output_no_response( "login failed" );
        }

	return ( 0 );
}






