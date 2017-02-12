//manager_authentication.h    william k. johnson 2017

#pragma once
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-function"

#include <auth_mgr_ace.h>

namespace auth_manager
{

                //forward
                class auth_mgr_handler;
                class auth_mgr_thread_pool;
                class auth_mgr_acceptor;
                class auth_mgr_protocol_data;



                //enumerations
                enum class auth_packet_error : unsigned long
                {
                    peNoToken = 0 ,
                    peTokenTooLong ,
                    peNoCommand ,
                    peCommandTooLong ,
                    peParametersExpectedGotNone ,
                    peNoParametersExpectedGotSome ,
                    peTooManyParameters ,
                    peTooFewParameters ,
                    peInvalidPayload ,
                    peInvalidSyntax ,
                    peAttributeNotSupported ,
                    peNotAuthenticated ,
                    peAlreadyAuthenticated ,
                    peNotEnoughPrivileges ,
                    peNoPacketError
                };

                //enumerations
                enum class concurrency_t : unsigned long
                {
                    single_threaded_,
                    thread_per_connection_,
                    thread_pool_
                };



                //aliases
                using auth_mgr_acceptor_base = ACE_Acceptor <auth_mgr_handler, ACE_SOCK_ACCEPTOR>;
                using counter_t = ACE_Atomic_Op<ACE_Mutex, int>;
                using auth_mgr_thread_pool_ptr = auth_mgr_thread_pool*;
                using auth_mgr_acceptor_ptr = auth_mgr_acceptor*;
                using protocol_data_ptr = auth_mgr_protocol_data*;



               //-----------------------------------------------------------------------------------------------
               class auth_mgr__counter_guard
               {
                            public:

                              //ctor
                              auth_mgr_counter_guard ( counter_t &counter )
                                : counter_ ( counter ) { ++counter_; }

                              //dtor
                              ~auth_mgr__counter_guard ( void ) { --counter_; }

                            protected:

                              //attributes
                              counter_t &counter_;
                };

                //-----------------------------------------------------------------------------------------------
                class auth_mgr_message_block_guard
                {
                        public :

                          //ctor
                          auth_mgr_message_block_guard ( ACE_Message_Block *&mb ): mb_ { mb } {}

                          //dtor
                          ~auth_mgr__message_block_guard (void)	  {	mb_->release (); }

                        protected :

                          //attributes
                          ACE_Message_Block *&mb_;

                };

                //adt
                //----------------------------------------------------------------------------------------------
                //instance data
                class auth_mgr_protocol_data
                {
                        public :

                            //ctor
                            auth_mgr_protocol_data( const std::string& nametag ) :  m_dwPort { 7082 } ,
                                                                                    m_dwThreads { 10 } ,
                                                                                    m_allow_remote { false }
                            {}

                            //dtor
                            virtual ~auth_mgr_protocol_data() = default;
                            {}

                        protected :

                            unsigned long m_dwPort;
                            unsigned long m_dw_threads;
                            bool          m_allow_remote;

                        public :

                            //accessors
                            unsigned long port()  const noexcept { return ( m_dwPort ); }
                            unsigned long thread_pool_size()  const noexcept { return ( m_dw_threads ); }
                            bool remote() const noexpr { return m_allow_remote; }

                            //mutators
                            void port( unsigned long port ) { m_dwPort = port; }
                            void thread_pool_size( unsigned long pool_size ) { m_dwThreads = pool_size; }
                            void remote( const bool rm ) { m_allow_remote = rm };

                };

                //---------------------------------------------------------------------------------------------
                class auth_mgr_handler : public ACE_Svc_Handler <ACE_SOCK_STREAM, ACE_NULL_SYNCH>
                {

                         public :

                              //types
                              typedef ACE_Svc_Handler <ACE_SOCK_STREAM, ACE_NULL_SYNCH> inherited;

                              //ctor
                              auth_mgr_handler ( const ACE_Time_Value& max_inactivity = ACE_Time_Value ( max_client_timeout ) );

                              //actual destructor
                              void destroy ();

                              //open handler
                              int open ( void *accept );

                              //close handler
                              int close ( u_long flags = 0 );

                              virtual int handle_close (ACE_HANDLE handle = ACE_INVALID_HANDLE,
                                                        ACE_Reactor_Mask mask = ACE_Event_Handler::ALL_EVENTS_MASK);

                              int handle_timeout( const ACE_Time_Value& now , const void* act );


                            private :

                               auth_mgr_acceptor_ptr            m_auth_mgr_acceptor;
                               auth_mgr_packet_error            m_auth_mgr_packet_error;
                               long                             m_dw_timer_token;
                               const ACE_Time_Value             m_maximum_time_to_wait;
                               ACE_Time_Value                   m_last_activity_time;


                            protected :

                              auth_mgr_thread_pool* thread_pool();
                              ACE_thread_t creator_;
                              concurrency_t concurrency ();

                              //adaptive communication environment overrides
                              int svc ();
                              int handle_input (ACE_HANDLE handle);

                              //thread queue callback
                              int process ( char *rdbuf, int rdbuf_len );

                               //hidden destructor
                              ~auth_mgr_handler();


                            public :

                              //accessors-inspectors
                              auth_mgr_acceptor_ptr acceptor() { return ( m_auth_mgr_acceptor ); }
                              auth_mgr_packet_error error_packet() const noexcept { return ( m_auth_mgr_packet_error ); }

                              //mutators
                              void error_packet( auth_mgr_packet_error error ) { m_auth_mgr_packet_error = error; }
                              void acceptor( auth_mgr_acceptor_ptr acceptor ) { m_auth_mgr_acceptor = acceptor; }


               };

               //--------------------------------------------------------------------------------------------------
               class auth_mgr_acceptor : public auth_mgr_acceptor_base
               {
                            public :

                              typedef auth_mgr_acceptor_base inherited;

                              auth_mgr_acceptor ( concurrency_t concurrency = concurrency_t::thread_pool_);
                              auth_mgr_acceptor ( auth_mgr_thread_pool &thread_pool);
                          ~auth_mgr_acceptor (void);

                          int open ( const ACE_INET_Addr &addr,
                                     ACE_Reactor *reactor,
                                     protocol_data_ptr instance_data ,
                                     int pool_size = auth_mgr_thread_pool::default_pool_size_ );

                          int close (void);

                        protected :

                          //attributes
                          concurrency_t             concurrency_;
                          auth_mgr_thread_pool      private_thread_pool_;
                          auth_mgr_thread_pool&     the_thread_pool_;
                          protocol_data_ptr         m_instance_data;

                        public :

                          //accessors
                          concurrency_t concurrency (void) noexcept { return this->concurrency_; }
                          auth_mgr_thread_pool_ptr thread_pool (void) { return &this->the_thread_pool_; }
                          int thread_pool_is_private (void) {return &the_thread_pool_ == &private_thread_pool_; }
                          protocol_data_ptr  data() { return ( m_instance_data ); }


                          //mutators
                          void thread_pool( ACE_Thread_Manager* thrd_mgr ) { ACE_Task<ACE_MT_SYNCH> ( thr_mgr ); }
                          void data( protocol_data_ptr data ) { m_instance_data = data; }

                };





}
