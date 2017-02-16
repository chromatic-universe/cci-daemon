// proc.h    william k. johnson chromatic unvierse 2017
#include "ace/svc_export.h"

#pragma once
//c++ standard
#include <tuple>
#include <stdexcept>
#include <exception>
#include <system_error>
#include <new>
#include <ios>
#include <future>
#include <typeinfo>
#include <iostream>
#include <utility>
#include <string>
#include <memory>
#include <type_traits>
#include <limits>
#include <algorithm>
#include <chrono>
#include <ratio>
#include <list>
#include <vector>
#include <map>
#include <set>
#include <functional>
#include <sstream>
#include <thread>

//c runtime std namepsace
#include <cstdio>
#include <cerrno>
#include <cctype>
#include <csignal>
#include <cstdlib>
#include <cstddef>
#include <cstring>
#include <ctime>
#include <climits>
#include <cassert>
//c runtime
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

//adaptive communication environment
#include <ace/Task.h>
#include <ace/Atomic_Op.h>
#include <ace/Acceptor.h>
#include <ace/Svc_Handler.h>
#include <ace/Date_Time.h>
#include <ace/Mutex.h>
#include <ace/OS.h>
#include <ace/Get_Opt.h>
#include <ace/streams.h>
#include <ace/OS_NS_errno.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/Trace.h>
#include <ace/Timer_Queue_T.h>
#include <ace/Signal.h>
#include <ace/Service_Config.h>
#include <ace/Service_Object.h>
#include <ace/OS_NS_stdio.h>
#include <ace/OS_NS_string.h>
#include <ace/Configuration.h>
#include <ace/Configuration_Import_Export.h>
#include <ace/Log_Msg.h>
#include <ace/Log_Msg_Callback.h>
#include <ace/Log_Record.h>
#include "ace/OS_NS_time.h"
#include "ace/Reactor.h"
#include "ace/Event_Handler.h"
#include "ace/Asynch_IO.h"
#include "ace/Proactor.h"


