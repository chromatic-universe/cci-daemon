#
# Compiler flags
#
CXX     = clang++
CFLAGS = -Wall -Qunused-arguments -Werror -Wextra -DPLUGU_ENABLE_STL -std=c++11 -I../include \
		   		 																-I/dev_tools/libsourcey/src/pluga/include \
																			    -I/dev_tools/libsourcey/src/base/include \
																				-I/dev_tools/libsourcey/build \
																				-I/dev_tools/libsourcey/src/uv/include \
																				-I/dev_tools/libsourcey/deps/libuv/include
LDFLAGS = -L/usr/local/lib -L/dev_src/dev_src_git/lib -ldl -llibuv -lscy_base -lmkdaemon-default 
TARGET_BIN = harness 
#
# Project files
#
SRCS = harness.cpp 
OBJS = $(SRCS:.cpp=.o)

#
# Debug build settings
#
DBGDIR = debug
DBGBIN = $(DBGDIR)/$(TARGET_BIN)
DBGOBJS = $(addprefix $(DBGDIR)/, $(OBJS))
DBGCFLAGS = -g -O0 -DDEBUG -pg 

#
# Release build settings
#
RELDIR = release
RELBIN = $(RELDIR)/$(TARGET_BIN)
RELOBJS = $(addprefix $(RELDIR)/, $(OBJS))
RELCFLAGS = -O3 -DNDEBUG

.PHONY: all clean debug prep release remake

# Default build
all: prep release

#
# Debug rules
#
debug: $(DBGBIN)

$(DBGBIN): $(DBGOBJS)
	$(CXX) $(CFLAGS) $(DBGCFLAGS) $(LDFLAGS) -o $(DBGBIN) $^

$(DBGDIR)/%.o: %.cpp
	$(CXX) -c $(CFLAGS) $(DBGCFLAGS) -o $@ $<



#
# Release rules
#
release: $(RELBIN)

$(RELBIN): $(RELOBJS)
	$(CXX) $(CFLAGS) $(RELCFLAGS) $(LDFLAGS) -o $(RELBIN) $^

$(RELDIR)/%.o: %.cpp

	$(CXX) -c $(CFLAGS) $(RELCFLAGS) -o $@ $<

#
# Other rules
#
prep:
	@mkdir -p $(DBGDIR) $(RELDIR)

remake: clean all

clean:
	rm -f $(RELOBJS) $(DBGOBJS) $(DBGBIN) $(RELBIN)


