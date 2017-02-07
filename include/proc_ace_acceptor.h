//proc_ace_acceptor.h chromatic universe 2017 william k. johnson

//suppress from adaptive communication environment
#pragma clang diagnostic ignored "-Wself-assign"
#pragma clang diagnostic ignored "-Wconstant-logical-operand"


#include <proc.h>

namespace proc_ace
{
            ///forward
            class proc_handler;
            class proc_thread_pool;
            class proc_acceptor;
            class proc_protocol_data;


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
            enum class concurrency_t : unsigned long
            {
                single_threaded_,
                thread_per_connection_,
                thread_pool_
            };

            enum class proc_command : unsigned long
            {
                proc_login = 0 ,
                proc_logout ,
                proc_authenticate ,
                proc_register ,
                proc_unregiser ,
                proc_no_command ,
                proc_error
            };

            enum class proc_packet_error : unsigned long
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

            enum class proc_set_param : unsigned long
            {
                ospPrompt = 0
            };

            enum class proc_packet_part : unsigned long
            {
                ppToken = 0 ,
                ppCommand ,
                ppPayload ,
                ppUid
            };

            enum class proc_state : unsigned long
            {
                oesNonAuthenticated = 0 ,
                oesConnected ,
                oesAuthenticated ,
                oesSelected ,
                oesAdmin ,
                oesWait ,
                oesLogout
            };

            enum class proc_set_op : unsigned long
            {
                osopAdd ,
                osopDelete ,
                osopClear
            };


            //aliases
            using proc_acceptor_base = ACE_Acceptor <proc_handler, ACE_SOCK_ACCEPTOR>;
            using counter_t = ACE_Atomic_Op<ACE_Mutex, int>;
            using protocol_data_ptr = proc_protocol_data*;
            using proc_thread_pool_ptr = proc_thread_pool*;
            using proc_acceptor_ptr = proc_acceptor*;
            using set_map = std::map<proc_set_param , bool>;
            using byte_ptr = unsigned char*;
            using map_of_parts = std::multimap<proc_packet_part , std::string>;
            using current_state = std::set<proc_state>;


            //adt
            //----------------------------------------------------------------------------------------------
            //instance data
			class proc_protocol_data
			{
					public:

						//ctor
						proc_protocol_data( const std::string& nametag ) :  m_strNameTag { nametag } ,
																			m_dwPort { 8000L } ,
																			m_dwThreads { 125 }
						{}

						//dtor
						virtual ~proc_protocol_data() {}

					protected:

						std::string m_strNameTag;
						std::string m_strDefaultHost;
						std::string m_strDocumentPath;
						unsigned long m_dwPort;
						unsigned long m_dwThreads;
						std::string m_strDn;

					public:

						//accessors
						std::string tag() const noexcept { return ( m_strNameTag ); }
						std::string default_host() const noexcept { return ( m_strDefaultHost ); }
						std::string dn()  const noexcept { return ( m_strDn ); }
						std::string document_path() const noexcept { return ( m_strDocumentPath ); }
						unsigned long port()  const noexcept { return ( m_dwPort ); }
						unsigned long thread_pool_size()  const noexcept { return ( m_dwThreads ); }

						//mutators
						void port( unsigned long port ) { m_dwPort = port; }
						void default_host( std::string host ) { m_strDefaultHost = host; }
						void dn( std::string dn ) { m_strDn = dn; }
						void document_path( std::string document_path ) { m_strDocumentPath = document_path; }
						void thread_pool_size( unsigned long pool_size ) { m_dwThreads = pool_size; }

			};


            //-----------------------------------------------------------------------------------------------
            class proc_counter_guard
			{
                        public:

                          //ctor
                          proc_counter_guard ( counter_t &counter )
                            : counter_ ( counter ) { ++counter_; }

                          //dtor
                          ~proc_counter_guard ( void ) { --counter_; }

                        protected:

                          //attributes
                          counter_t &counter_;
			};

            //-----------------------------------------------------------------------------------------------
            class proc_message_block_guard
            {
                public:

                  //ctor
                  proc_message_block_guard ( ACE_Message_Block *&mb ): mb_ { mb } {}

                  //dtor
                  ~proc_message_block_guard (void)	  {	mb_->release (); }

                protected:

                  //attributes
                  ACE_Message_Block *&mb_;

            };

            //services
            //-----------------------------------------------------------------------------------------------
            class proc_thread_pool : public ACE_Task<ACE_MT_SYNCH>
            {

                        public:

                          typedef ACE_Task<ACE_MT_SYNCH> inherited;

                          //ctor
                          explicit proc_thread_pool( void );

                          enum size_t
                          {
                            default_pool_size_ = 25
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
            class proc_handler : public ACE_Svc_Handler <ACE_SOCK_STREAM, ACE_NULL_SYNCH>
            {

                        public:

                          //types
                          typedef ACE_Svc_Handler <ACE_SOCK_STREAM, ACE_NULL_SYNCH> inherited;
                          //function pointer prototype
                          typedef int ( proc_handler::*ptr_to_function )( const std::string& strParams );
                          typedef std::map<std::string , ptr_to_function> map_of_commands;

                          //ctor
                          proc_handler ( const ACE_Time_Value& max_inactivity = ACE_Time_Value ( max_client_timeout ) );

                          //actual destructor
                          void destroy ();

                          //open handler
                          int open ( void *accept );

                          //close handler
                          int close ( u_long flags = 0 );

                          virtual int handle_close (ACE_HANDLE handle = ACE_INVALID_HANDLE,
                                                    ACE_Reactor_Mask mask = ACE_Event_Handler::ALL_EVENTS_MASK);

                          int handle_timeout( const ACE_Time_Value& now , const void* act );


                        private:

                           proc_acceptor_ptr                m_proc_acceptor;
                           map_of_commands                  m_command_map;
                           std::ostringstream               m_ostr;
                           std::string                      m_strComputerName;
                           std::string                      m_strToken;
                           std::string                      m_strCommand;
                           proc_command                     m_current_command;
                           proc_packet_error                m_proc_packet_error;
                           long                             m_dwTimerToken;
                           const ACE_Time_Value             m_maximumTimeToWait;
                           set_map                          m_set_map;
                           bool                             m_bSilent;
                           ACE_Time_Value                   m_lastActivityTime;
                           current_state                    m_currentState;


                        protected:

                          proc_thread_pool* thread_pool();
                          ACE_thread_t creator_;
                          concurrency_t concurrency ();

                          //adaptive communication environment overrides
                          int svc ();
                          int handle_input (ACE_HANDLE handle);

                          //thread queue callback
                          int process ( char *rdbuf, int rdbuf_len );

                           //hidden destructor
					      ~proc_handler();

                          //helpers
                          void output_boiler_plate( std::string str_concurrency = "" );
                          std::string output_prompt( bool bToken = true );
                          void map_commands();
                          void perform( const std::string& str );
                          void flush_stream_to_connector();
                          void flush_binary_stream_to_connector( unsigned char* u_buffer , unsigned long dwLen );
                          void format_packet_error( proc_packet_error pe );
                          std::string client_addr();
                          void stream_out( const std::string& str );
					      void stream_out( const std::string& str , unsigned long len );
					      void stream_out( const byte_ptr& ptrBytes , unsigned long len );
                          void output_ok_response( const std::string& ack );
					      void output_no_response( const std::string& nack );
                          void flush_immediate( const std::string& atom );
                          void parse_command( const std::string& packet ,
										 map_of_parts& parts ,
										 unsigned short& usCommandParamCount );
                          bool check_connection_state( proc_state state );
                          void update_connection_state( proc_set_op op , proc_state state );




                        public :

                          //accessors-inspectors
                          proc_acceptor* acceptor() { return ( m_proc_acceptor ); }
                          std::string computer_name() const noexcept { return ( m_strComputerName ); }
                          std::ostringstream& stream() { return ( m_ostr ); }
                          std::string token() const noexcept  { return ( m_strToken ); }
                          map_of_commands& command_map() { return ( m_command_map ); }
                          bool silent() { return ( m_bSilent ); }
                          proc_command command() const noexcept  { return ( m_current_command ); }
                          proc_packet_error error_packet() const noexcept { return ( m_proc_packet_error ); }
                          std::string command_str() const noexcept { return ( ucase( m_strCommand ) ); }

                          //mutators
                          void silent( bool bSilent ) { m_bSilent = bSilent; }
                          void error_packet( proc_packet_error error ) { m_proc_packet_error = error; }
                          void acceptor( proc_acceptor_ptr acceptor ) { m_proc_acceptor = acceptor; }
                          void command( proc_command command ) { m_current_command = command; }
                          void command_str( const std::string& str_command ) { m_strCommand = str_command; }
                          void token( const std::string& str_token ) { m_strToken = str_token; }


                          //handlers
						  int on_login( const std::string& params );


                          //string utils
                          static std::string ucase( const std::string& str );
                          static std::string lcase( const std::string& str );
                          static void gnaw( std::string& str , const std::string& token );
                          static void rtrimlast( std::string& str , const std::string& token );
                          static void chomp( std::string& str , const std::string& token );
                          static std::string ltrim( const std::string &szToTrim ,
                                                    const std::string& szTrimChars );
                          static std::string rtrim( const std::string &szToTrim ,
                                                    const std::string& szTrimChars );
                          static std::string extract_quoted_string( const std::string& str );






            };

            //--------------------------------------------------------------------------------------------------
            class proc_acceptor : public proc_acceptor_base
            {
                public:

                  typedef proc_acceptor_base inherited;

                  proc_acceptor ( concurrency_t concurrency = concurrency_t::thread_pool_);
                  proc_acceptor ( proc_thread_pool &thread_pool);
                  ~proc_acceptor (void);

                  int open ( const ACE_INET_Addr &addr,
                             ACE_Reactor *reactor,
                             protocol_data_ptr instance_data ,
                             int pool_size = proc_thread_pool::default_pool_size_ );

                  int close (void);

                protected:

                  //attributes
                  concurrency_t         concurrency_;
                  proc_thread_pool      private_thread_pool_;
                  proc_thread_pool&     the_thread_pool_;
                  protocol_data_ptr     m_instance_data;

                public:

                  //accessors
                  concurrency_t concurrency (void) noexcept { return this->concurrency_; }
                  proc_thread_pool_ptr thread_pool (void) { return &this->the_thread_pool_; }
                  int thread_pool_is_private (void) {return &the_thread_pool_ == &private_thread_pool_; }
                  protocol_data_ptr  data() { return ( m_instance_data ); }


                  //mutators
                  void thread_pool( ACE_Thread_Manager* thrd_mgr ) { ACE_Task<ACE_MT_SYNCH> ( thr_mgr ); }
                  void data( protocol_data_ptr data ) { m_instance_data = data; }

            };


}



