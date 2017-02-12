//proc_ace_acceptor.cpp

#include <auth_ace_manager.h>

using namespace auth_ace_manager;
using std::ostringstream;


//----------------------------------------------------------------------------------------------------------------------------
 auth_mgr_acceptor::auth_mgr_acceptor ( concurrency_t concurrency ) : concurrency_ { concurrency } ,
																        the_thread_pool_ ( private_thread_pool_ )  ,
																		m_instance_data{ nullptr }
 {
	 	 ACE_Trace _( ACE_TEXT( "auth_mgr_acceptor::auth_mgr_acceptor ( concurrency_t concurrency )" ) , __LINE__ );
 }

 //----------------------------------------------------------------------------------------------------------------------------
 auth_mgr_acceptor::auth_mgr_acceptor ( auth_mgr_thread_pool &thread_pool ) : concurrency_ { concurrency_t::thread_pool_ },
		    																  the_thread_pool_ ( thread_pool ) ,
		    																  m_instance_data{ nullptr }
  {
	  	  ACE_Trace _( ACE_TEXT( "auth_mgr_acceptor::auth_mgr_acceptor ( proc_thread_pool &thread_pool)" ) , __LINE__ );
  }

//----------------------------------------------------------------------------------------------------------------------------
auth_mgr_acceptor::~auth_mgr_acceptor ( void )
{
	   	   ACE_Trace _( ACE_TEXT( "auth_mgr_acceptor::~auth_mgr_acceptor ( void )" ) , __LINE__ );

		   if ( concurrency() == concurrency_t::thread_pool_ && thread_pool_is_private () )
		   {
			  thread_pool ()->close ();
		   }
}

//----------------------------------------------------------------------------------------------------------------------------
int auth_mgr_acceptor::open (	const ACE_INET_Addr &addr,
								ACE_Reactor *reactor,
								protocol_data_ptr instance_data ,
								int pool_size )
{
    	  ACE_Trace _( ACE_TEXT( "int auth_mgr_acceptor::open" ) , __LINE__ );

    	  data( instance_data );

    	  if ( concurrency() == concurrency_t::thread_pool_ && thread_pool_is_private() )
    	  {
    	    thread_pool()->start( pool_size );
    	  }

    	  return inherited::open ( addr , reactor );
}

//----------------------------------------------------------------------------------------------------------------------------
int auth_mgr_acceptor::close ( void )
{
    	ACE_Trace _( ACE_TEXT( "auth_mgr_acceptor::close" ) , __LINE__ );

    	if ( concurrency() == concurrency_t::thread_pool_ && thread_pool_is_private () )
    	  {
    	    thread_pool ()->stop();
    	  }

    	  return inherited::close();
}







