//proc_ace_acceptor.cpp

#include <proc_ace_acceptor.h>

using namespace proc_ace;
using std::ostringstream;


//----------------------------------------------------------------------------------------------------------------------------
 proc_acceptor::proc_acceptor ( concurrency_t concurrency ) : concurrency_ { concurrency } ,
																        the_thread_pool_ ( private_thread_pool_ )  ,
																		m_instance_data{ nullptr }
 {
	 	 ACE_Trace _( ACE_TEXT( "proc_acceptor::proc_acceptor ( concurrency_t concurrency )" ) , __LINE__ );
 }

 //----------------------------------------------------------------------------------------------------------------------------
  proc_acceptor::proc_acceptor ( proc_thread_pool &thread_pool ) : concurrency_ { concurrency_t::thread_pool_ },
		    																	  the_thread_pool_ ( thread_pool ) ,
		    																	  m_instance_data{ nullptr }
  {
	  	  ACE_Trace _( ACE_TEXT( "proc_acceptor::proc_acceptor ( proc_thread_pool &thread_pool)" ) , __LINE__ );
  }

  //----------------------------------------------------------------------------------------------------------------------------
   proc_acceptor::~proc_acceptor ( void )
   {
	   	   ACE_Trace _( ACE_TEXT( "proc_acceptor::~proc_acceptor ( void )" ) , __LINE__ );

		   if ( concurrency() == concurrency_t::thread_pool_ && thread_pool_is_private () )
		   {
			  thread_pool ()->close ();
		   }
   }

   //----------------------------------------------------------------------------------------------------------------------------
    int proc_acceptor::open (	const ACE_INET_Addr &addr,
									ACE_Reactor *reactor,
									protocol_data_ptr instance_data ,
									int pool_size )
    {
    	  ACE_Trace _( ACE_TEXT( "int proc_acceptor::open" ) , __LINE__ );

    	  data( instance_data );

    	  if ( concurrency() == concurrency_t::thread_pool_ && thread_pool_is_private() )
    	  {
    	    thread_pool()->start( pool_size );
    	  }

    	  return inherited::open ( addr , reactor );
    }

    //----------------------------------------------------------------------------------------------------------------------------
    int proc_acceptor::close ( void )
    {
    	ACE_Trace _( ACE_TEXT( "proc_acceptor::close" ) , __LINE__ );

    	if ( concurrency() == concurrency_t::thread_pool_ && thread_pool_is_private () )
    	  {
    	    thread_pool ()->stop();
    	  }

    	  return inherited::close();
    }







