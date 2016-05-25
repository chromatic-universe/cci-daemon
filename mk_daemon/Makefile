# chromatic-daemon make william k. Johnson 2015

#
# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#

# define the c++ compiler to use
CXX = clang++

#define any compile-time flags
CPPFLAGS = -Wall -O0 -Qunused-arguments -g -std=c++11 -pg -D_FILE_OFFSET_BITS=64 
CPPFLAGS_RELEASE = -Wall -O3 -Qunused-arguments -std=c++11 -D_FILE_OFFSET_BITS=64 

# define any directories containing header files other than /usr/include
#
INCLUDES = -I../include -I/usr/include/c++/4.8 -I/usr/include/i386-linux-gnu/c++/4.8
	       

# define library paths in addition to /usr/lib
LFLAGS = -L$(DEV)/lib

# define any libraries to link into executable:
LIBS = -lstdc++ 

# define the C++ source files
SRCS = chromatic_daemon.cpp main.cpp

# define the C++ object files 
OBJS = chromatic_daemon.o main.o
OBJS_DBG = chromatic_daemondbg.o maindbg.o

# define the executable file 
MAIN = chromatic-daemon-dbg
RELEASE_MAIN = chromatic-daemon

.PHONY: depend clean

.DEFAULT: debug

debug:   $(MAIN)
        chromatic_daemondbg.o: chromatic_daemon.cpp
	$(CC) $(CPPFLAGS) $(INCLUDES) -c  chromatic_daemon.cpp -o chromatic_daemondbg.o

        maindbg.o: main.cpp

	$(CC) $(CPPFLAGS) $(INCLUDES) -c  main.cpp -o maindbg.o

	@echo  debug chromatic-daemon has been compiled
release:  $(RELEASE_MAIN)
        chromatic_daemon.o: chromatic_daemon.cpp
	$(CC) $(CPPFLAGS_RELEASE) $(INCLUDES) -c  chromatic_daemon.cpp -o chromatic_daemon.o

        main.o: main.cpp

	$(CC) $(CPPFLAGS_RELEASE) $(INCLUDES) -c  main.cpp -o main.o

	@echo  release chromatic-daemon has been compiled

$(MAIN): $(OBJS_DBG)
	 $(CC) $(CPPFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS_DBG) $(LFLAGS) $(LIBS)

$(RELEASE_MAIN): $(OBJS) 
	$(CC) $(CPPFLAGS_RELEASE) $(INCLUDES) -o $(RELEASE_MAIN) $(OBJS) $(LFLAGS) $(LIBS)
	

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file) 
# (see the gnu make manual section about automatic variables)
.c.o:
	$(CC) $(CPPFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) *.o *~ $(MAIN)
	$(RM) *.o *~ $(RELEASE_MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^ 

# DO NOT DELETE THIS LINE -- make depend needs it
