//cci_daemon.h      chromatic universe  william k, johnson  2017
#pragma once
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-function"

//c runtime
#include <execinfo.h>
#include <cstdlib>
#include <cstdio>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <syslog.h>
#include <sys/stat.h>

#include <unistd.h>
//c++ standard
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <memory>
#include <exception>
#include <algorithm>

//cci
#include <cci_stack_trace.h>
#include <cci_shared_lib.h>
#include <cci_kernel_directives.h>
#include <cci_daemon_generic.h>
#include <cci_daemonize.h>


namespace cci_daemon_impl
{
            //forward
            class  cci_daemon_facade;
            struct kernel_context;


            //aliases
            using  cci_daemon_facade_ptr = cci_daemon_facade*;
            using  file_ptr = FILE*;
            using  kernel_context_ptr = kernel_context*;


           
            enum class service_proc : unsigned long
            {
                sp_default_service = 0 ,
                sp_default_coordinator ,
                sp_custom_coordinator
            };


            //interfaces
            class cci_daemon_intf
            {
                public :

                    virtual void map_kernel() = 0;
                    virtual void bootstrap_coordinator() = 0;
                    virtual int daemonize() = 0;
                    virtual int daemon_default_exec( const std::string& str_params ,
                                                     const unsigned long dw_flags ) = 0;
                    virtual ~cci_daemon_intf() = 0;

            };

            //services
            class cci_daemon_facade : public cci_daemon_intf
            {
                public :

                    //types
                    //function pointer prototype
                    typedef int (cci_daemon_facade::*ptr_to_proc)
                                (  const std::string& str_params , const unsigned long dw_flags  );

                    //ctors
                    explicit cci_daemon_facade( int argc , char* argv[] ) ;

                    //dtor
                    virtual ~cci_daemon_facade();

                    //no copy
                    cci_daemon_facade( const cci_daemon_facade& cdf ) = delete;
                    //no assing
                    cci_daemon_facade& operator= ( const cci_daemon_facade& cdf ) = delete;


                protected :

                    //support
                    virtual bool open_log();
                    virtual bool log_message( const std::string& msg );
                    virtual void close_log();
                    virtual bool read_config_file( const std::string& config_path );
                    virtual bool write_pid( const std::string& pid_file ,
                                            int flags  );
                    virtual bool remove_pid( const std::string& pid_file );
                    virtual int lock_region( int fd ,
                                             int type ,
                                             int whence ,
                                             int start ,
                                             int len );
                    void map_kernel();
                    void  _bt() { if ( backtrace() ) {  print_stacktrace( m_log_fp ); } }
                    void  _btw() { if ( backtrace() ) {  print_walk_backtrace( m_log_fp ); } }
                    lib_handle_t load_lib( const std::string& lib );

                    //accessors-inspectors
                    kernel_context_ptr context() { return m_ptr_kernel_context.get(); }
                    void k_( call_kernel_function* ckf ) { if( ! ckf )
                    { throw std::runtime_error( ".....could not retrieve function addres=>..." );} }

                private :

                    //attributes
                    unsigned long                   m_dw_flags;
                    file_ptr                        m_log_fp;
                    bool                            m_b_opened;
                    service_proc                    m_service_proc;
                    std::string                     m_str_conf;
                    std::string                     m_str_pid_path;
                    bool                            m_backtrace;
                    int                             m_argc;
                    char**                          m_argv;
                    std::string                     m_chroot_dir;
                    std::unique_ptr<kernel_context> m_ptr_kernel_context;

                    //prototypes
                    typedef int bootstrap_function( int argc , char* argv[] , void* ptr );

                    //helpers
                    bootstrap_function*      bootstrap_function_address;


                public  :

                    //accessors-inspectors
                    constexpr unsigned long flags() const noexcept { return m_dw_flags; }
                    constexpr unsigned long backtrace() const noexcept { return m_backtrace; }
                    service_proc proc() const noexcept { return m_service_proc; }
                    std::string config_path() const noexcept { return m_str_conf; }
                    std::string pid_path() const noexcept { return m_str_pid_path; }
                    std::string chroot_dir() const noexcept { return m_chroot_dir; }

                    //kernel functoroids
                    int bootstrap( int argc , char* argv[] , void* ptr  )
                    { return bootstrap_function_address( argc , argv , ptr ); }

                    //mutators
                    void flags( const unsigned long dw_flags ) noexcept { m_dw_flags = dw_flags; }
                    void backtrace( const unsigned long trace ) noexcept { m_backtrace = trace; }
                    void config_path( const std::string& path ) { m_str_conf = path; }
                    void pid_path( const std::string& path ) { m_str_pid_path = path; }
                    void proc( service_proc sp ) { m_service_proc = sp; }
                    void chroot_dir( const std::string& chr ) { m_chroot_dir = chr; }

                    //stream
                    friend std::ostream& operator<< ( std::ostream& o , const cci_daemon_facade& df );

                    //daemon execute procs
                    virtual int daemon_default_exec( const std::string& str_params , const unsigned long dw_flags );
                    virtual void bootstrap_coordinator();

                    //services
                    virtual int daemonize();

                    //immutable
                    //
                    //
                    static const std::string log_path;
                    ///buffer isze
                    static const unsigned buffer_size;
                    ///default config
                    static const std::string path_config;
                    ///pid
                    static const std::string path_pid;
                    ///close exec
                    static const unsigned cpf_cloexec;

                    

            };
            inline std::ostream& operator<< ( std::ostream& ostr, const cci_daemon_facade& df )
            {
                //todo
                return ostr << "daemon facade "
                            << "log=>" << cci_daemon_impl::cci_daemon_facade::log_path;
            }

            //immutable

            static void chromatic_terminate() noexcept
            {
                //core dump if environment set , else use exit
                char* s = getenv( "EF_DUMPCORE" );
                if( s != nullptr && *s != '\0' )
                {
                    abort();
                }
                else
                {
                    exit( EXIT_FAILURE );
                }
            }
            static volatile sig_atomic_t hup_received = 0; //set non-zero on receipt of SIGHUP
            static void sighup_handler( int signal )
            {
                hup_received = 1;
            }
            static void sigterm_handler( int signal )
            {
                openlog( "cci-daemon" , 0  , LOG_USER );
                syslog ( LOG_USER | LOG_INFO , "%s", "cci-daemon deactivated by sigterm...." );
                closelog();

                exit( EXIT_SUCCESS );
            }
            //todo
            static void sigsev_handler( int signal )
            {
               void *array[10];
               size_t size;
               file_ptr log_p { nullptr };
               int fn( 0L );

              // void*'s for all entries on the stack
              size = backtrace( array, 10 );

              // print out all the frames to stderr
              if( log_p )
              {
                  fprintf( log_p , "error: signal %d:\n" , signal );
                  backtrace_symbols_fd( array, size, fn );

                  exit( 1 );
              }
            }

}
namespace cdi = cci_daemon_impl;
