
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
proc_handler::proc_handler ( const ACE_Time_Value& max_inactivity ) :   m_current_command{ proc_command::proc_no_command } ,
                                                                        m_proc_packet_error{ proc_packet_error::peNoPacketError } ,
                                                                        m_bSilent{ false } ,
                                                                        m_proc_acceptor{ nullptr } ,
                                                                        creator_ { ACE_Thread::self() } ,
                                                                        m_maximumTimeToWait{ max_inactivity } ,
                                                                        m_dwTimerToken{ 0 }
{

          ACE_Trace _( ACE_TEXT( "proc_handler::proc_handler" ) , __LINE__ );

          try
          {
              char hostname[1024];
              gethostname( hostname , 1023 );
              m_strComputerName = hostname;
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

