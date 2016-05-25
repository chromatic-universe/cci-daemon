#
# Compiler flags
#
CXX     = clang++
CFLAGS = -fPIC -Wall -Qunused-arguments -Werror -Wextra -DPLUGU_ENABLE_STL 	-std=c++11 -I../include \
		   		 																	   -I/dev_tools/libsourcey/src/pluga/include \
																			           -I/dev_tools/libsourcey/src/base/include \
																					   -I/dev_tools/libsourcey/build \
																					   -I/dev_tools/libsourcey/src/uv/include \
																					   -I/dev_tools/libsourcey/deps/libuv/include
LDFLAGS = -shared  -lscy_pluga \
		           -lscy_util \
				   -llibuv \
				   -L/usr/local/lib# linking flags
BIN_LDFLAGS = -lmkdaemon-default
TARGET_LIB = libmkdaemon-default.so # target lib
TARGET_DEBUG_LIB =  libmkdaemon-default-dbg.so 
#
SRCS = mk_daemon_default.cpp 
OBJS = $(SRCS:.cpp=.o)

#
# Debug build settings
#
#
#
DBGDIR = debug
DBGLIB = $(DBGDIR)/$(TARGET_LIB)
DBGOBJS = $(addprefix $(DBGDIR)/, $(OBJS))
DBGCFLAGS = -g -O0 -DDEBUG -pg 

#
# Release build settings
#
RELDIR = release
RELLIB = $(RELDIR)/$(TARGET_LIB)
RELOBJS = $(addprefix $(RELDIR)/, $(OBJS))
RELCFLAGS = -O3 -DNDEBUG

.PHONY: all clean debug prep release r)make

# Default build
all: prep release

#
# Debug rules
#
debug: $(DBGLIB) 

$(DBGLIB): $(DBGOBJS)
	$(CXX) $(CFLAGS) $(DBGCFLAGS) $(LDFLAGS) -o $(DBGLIB) $^
	@cp $(DBGLIB) ../lib/$(TARGET_DEBUG_LIB)

$(DBGDIR)/%.o: %.cpp
	$(CXX) -c $(CFLAGS) $(DBGCFLAGS) -o $@ $<


#
# Release rules
#
release: $(RELLIB)

$(RELLIB): $(RELOBJS)
	$(CXX) $(CFLAGS) $(RELCFLAGS) $(LDFLAGS) -o $(RELLIB) $^
	@cp $(RELLIB) ../lib

$(RELDIR)/%.o: %.cpp

	$(CXX) -c $(CFLAGS) $(RELCFLAGS) -o $@ $<

#
# Other rules
#
prep:
	@mkdir -p $(DBGDIR) $(RELDIR)

remake: clean all

clean:
	rm -f $(RELEXE) $(RELOBJS) $(DBGEXE) $(DBGOBJS) $(DBGDIR)/$(TARGET_LIB) $(RELDIR)/$(TARGET_LIB)


