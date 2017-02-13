//manager_authentication.h    william k. johnson 2017

#pragma once
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-function"

#include <proc_ace.h>

namespace auth_ace_manager
{

                //forward
                class auth_mgr_handler;
                class auth_mgr_thread_pool;
                class auth_mgr_acceptor;
                class auth_mgr_protocol_data;


                //immutable
                //seconds
                static unsigned long max_client_timeout { 1800 };
                static const char crlf[] { "\015\012" };
                static const char cr[] { "\015" };
                static const char lf[] = { "\012" };
                static const unsigned short timer_id { 0 };
                static const unsigned long cmd_buffer_len { 65536L };
                static const unsigned long constCommandBufferLength { 1024 } ;
                static const unsigned short constMaxTokenLength { 10 };



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
                        peUnrecognizedCommand ,
                        peSelectState ,
                        peFailure ,
                        peInvalidObject ,
                        peInvalidState ,
                        peInvalidSpecification ,
                        peNoSpecification ,
                        peAttributeNotSupported ,
                        peNotAuthenticated ,
                        peAlreadyAuthenticated ,
                        peNotEnoughPrivileges ,
                        peNoPacketError
                };

                enum class auth_packet_part : unsigned long
                {
                    ppToken = 0 ,
                    ppCommand ,
                    ppPayload ,
                    ppUid
                };


                enum class concurrency_t : unsigned long
                {
                    single_threaded_,
                    thread_per_connection_,
                    thread_pool_
                };

                enum class auth_set_param : unsigned long
                {
                    ospPrompt = 0
                };


                 enum class auth_command : unsigned long
                {
                    auth_login = 0 ,
                    auth_logout ,
                    auth_authenticate ,
                    auth_no_command ,
                    auth_error
                };



                //aliases
                using auth_mgr_acceptor_base = ACE_Acceptor <auth_mgr_handler, ACE_SOCK_ACCEPTOR>;
                using counter_t = ACE_Atomic_Op<ACE_Mutex, int>;
                using auth_mgr_thread_pool_ptr = auth_mgr_thread_pool*;
                using auth_mgr_acceptor_ptr = auth_mgr_acceptor*;
                using protocol_data_ptr = auth_mgr_protocol_data*;
                using map_of_parts = std::multimap<auth_packet_part , std::string>;
                using byte_ptr = unsigned char*;



               //adt
               //-----------------------------------------------------------------------------------------------
               class auth_mgr_counter_guard
               {
                            public:

                              //ctor
                              auth_mgr_counter_guard ( counter_t &counter )
                                : counter_ ( counter ) { ++counter_; }

                              //dtor
                              ~auth_mgr_counter_guard ( void ) { --counter_; }

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
                          ~auth_mgr_message_block_guard (void)	  {	mb_->release (); }

                        protected :

                          //attributes
                          ACE_Message_Block *&mb_;

                };

                //----------------------------------------------------------------------------------------------
                //instance data
                class auth_mgr_protocol_data
                {
                        public :

                            //ctor
                            auth_mgr_protocol_data( const std::string& nametag ) :  m_dwPort { 7082 } ,
                                                                                    m_dw_threads { 10 } ,
                                                                                    m_allow_remote { false }
                            {}

                            //dtor
                            virtual ~auth_mgr_protocol_data() = default;


                        protected :

                            unsigned long m_dwPort;
                            unsigned long m_dw_threads;
                            bool          m_allow_remote;

                        public :

                            //accessors
                            unsigned long port()  const noexcept { return ( m_dwPort ); }
                            unsigned long thread_pool_size()  const noexcept { return ( m_dw_threads ); }
                            bool remote() const noexcept { return m_allow_remote; }

                            //mutators
                            void port( unsigned long port ) { m_dwPort = port; }
                            void thread_pool_size( unsigned long pool_size ) { m_dw_threads = pool_size; }
                            void remote( const bool rm ) { m_allow_remote = rm; }

                };

                //services
                //-----------------------------------------------------------------------------------------------
                class auth_mgr_thread_pool : public ACE_Task<ACE_MT_SYNCH>
                {

                            public:

                              typedef ACE_Task<ACE_MT_SYNCH> inherited;

                              //ctor
                              explicit auth_mgr_thread_pool( void );

                              enum size_t
                              {
                                default_pool_size_ = 10
                              };

                            protected:

                              //upcall
                              int svc ( void );

                              //attributes
                              counter_t active_threads_;

                            public:

                              //servicestao developer
                              int start ( int pool_size = default_pool_size_ );
                              virtual int stop ( void );
                              int enqueue ( ACE_Event_Handler *handler ) ;

                };



                //---------------------------------------------------------------------------------------------
                class auth_mgr_handler : public ACE_Svc_Handler <ACE_SOCK_STREAM, ACE_NULL_SYNCH>
                {

                         public :

                              //types
                              typedef ACE_Svc_Handler <ACE_SOCK_STREAM, ACE_NULL_SYNCH> inherited;
                              //function pointer prototype
                              typedef int ( auth_mgr_handler::*ptr_to_function )( const std::string& str_params );
                              typedef std::map<std::string , ptr_to_function> map_of_commands;


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
                               auth_packet_error                m_auth_packet_error;
                               long                             m_dw_timer_token;
                               const ACE_Time_Value             m_maximum_time_to_wait;
                               ACE_Time_Value                   m_last_activity_time;
                               std::string                      m_str_computer_name;
                               std::string                      m_str_command;
                               auth_command                     m_current_command;
                               std::string                      m_str_token;
                               std::ostringstream               m_ostr;
                               map_of_commands                  m_command_map;


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


                              //helpers
                              void output_boiler_plate( std::string str_concurrency = "" );
                              std::string output_prompt( bool bToken = true );
                              void map_commands();
                              void perform( const std::string& str );
                              void flush_stream_to_connector();
                              void flush_binary_stream_to_connector( unsigned char* u_buffer , unsigned long dwLen );
                              void format_packet_error( auth_packet_error pe );
                              std::string client_addr();
                              void stream_out( const std::string& str );
                              void stream_out( const std::string& str , unsigned long len );
                              void stream_out( const byte_ptr& ptrBytes , unsigned long len );
                              void output_ok_response( const std::string& ack );
                              void output_no_response( const std::string& nack );
                              void flush_immediate( const std::string& atom );
                              void parse_command( const  std::string& packet ,
                                                  map_of_parts& parts ,
                                                  unsigned short& uscommandParamCount );



                            public :

                              //accessors-inspectors
                              auth_mgr_acceptor_ptr acceptor() { return ( m_auth_mgr_acceptor ); }
                              auth_packet_error error_packet() const noexcept { return ( m_auth_packet_error ); }
                              std::string computer_name() const noexcept { return ( m_str_computer_name ); }
                              auth_command command() const noexcept  { return ( m_current_command ); }
                              std::string command_str() const noexcept { return ( m_str_command ); }
                              std::string token() const noexcept  { return ( m_str_token ); }
                              void token( const std::string& str_token ) { m_str_token = str_token; }
                              std::ostringstream& stream() { return ( m_ostr ); }
                              map_of_commands& command_map() { return ( m_command_map ); }


                              //mutators
                              void error_packet( auth_packet_error error ) { m_auth_packet_error = error; }
                              void acceptor( auth_mgr_acceptor_ptr acceptor ) { m_auth_mgr_acceptor = acceptor; }
                              void command( auth_command command ) { m_current_command = command; }
                              void command_str( const std::string& str_command ) { m_str_command = str_command; }




                              //handlers
                              int on_login( const std::string& params );

                              //string utils
                              static void chomp( std::string& str , const std::string& token );
                              static std::string lcase( const std::string& str );
                              static void rtrimlast( std::string& str , const std::string& token );





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
