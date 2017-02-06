//proc_thread_pool.cpp  william k. johnson 2017

#include <proc_ace_acceptor.h>

using namespace proc_ace;



//suppress from adaptive communication environment
#pragma clang diagnostic ignored "-Wself-assign"
#pragma clang diagnostic ignored "-Wconstant-logical-operand"


using namespace proc_ace;
using std::ostringstream;

//----------------------------------------------------------------------------------------------------------------------------
proc_thread_pool::proc_thread_pool (void)
  : active_threads_ ( 0L )
{
	//
}

//----------------------------------------------------------------------------------------------------------------------------
int proc_thread_pool::start ( int pool_size )
{
        return this->activate ( THR_NEW_LWP | THR_DETACHED , pool_size );
}

//----------------------------------------------------------------------------------------------------------------------------
int proc_thread_pool::stop ( void )
{

      int counter = active_threads_.value ();

      while ( counter-- )  { this->enqueue ( 0L ); }

      while ( active_threads_.value () )
      {
          ACE_Time_Value timeout;
          timeout.msec ( 250 );
          ACE_OS::sleep( timeout );
      }


    return( 0L );
}

//----------------------------------------------------------------------------------------------------------------------------
int proc_thread_pool::enqueue (ACE_Event_Handler *handler)
{

      void *v_data = (void *) handler;
      char *c_data = (char *) v_data;

      ACE_Message_Block *mb;
      ACE_NEW_RETURN (mb,
                      ACE_Message_Block ( c_data ),
                      -1);


      if ( this->putq ( mb ) == -1 )
      {
          mb->release ();
          return -1;
      }

      return ( 0L );
}

//----------------------------------------------------------------------------------------------------------------------------
int proc_thread_pool::svc ( void )
{


      ACE_Message_Block *mb;

      if ( ACE_Thread_Manager::instance ()->testcancel (ACE_Thread::self () ) != 0)
      {
          ACE_DEBUG ((LM_DEBUG,
              "(%t) has been cancelled" ));

          return ( 0L );
      }

      proc_counter_guard counter_guard ( active_threads_ );

      while (this->getq ( mb) != -1 )
      {

          proc_message_block_guard message_block_guard ( mb );
          char *c_data = mb->base ();

          if ( c_data )
          {
              void *v_data = (void *) c_data;
              ACE_Event_Handler *handler = (ACE_Event_Handler *) v_data;
              if ( handler->handle_input ( ACE_INVALID_HANDLE ) == -1)
              { handler->handle_close ( handler->get_handle () , 0 ); }
          }
          else
          { return ( 0L ); }//ok , shutdown request
      }

      return ( 0L );
}


