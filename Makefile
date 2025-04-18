CC=g++ -g -Wall -std=c++17 -Wno-deprecated-declarations

UNAME := $(shell uname -s)
ifeq ($(UNAME),Darwin)
    CC+=-D_XOPEN_SOURCE
    LIBFSCLIENT=libfs_client_macos.o
    LIBFSSERVER=libfs_server_macos.o
    BOOST_THREAD=boost_thread-mt
else
    LIBFSCLIENT=libfs_client.o
    LIBFSSERVER=libfs_server.o
    BOOST_THREAD=boost_thread
endif

# List of source files for your file server
FS_SOURCES=server.cpp

# Generate the names of the file server's object files
FS_OBJS=${FS_SOURCES:.cpp=.o}

TEST_SRCS = $(wildcard test*.cpp)
TESTS = $(patsubst %.cpp,%,$(TEST_SRCS))

all: fs $(TESTS)

# Compile the file server and tag this compilation
#
# Remember to set the CPLUS_INCLUDE_PATH, LIBRARY_PATH, and LD_LIBRARY_PATH
# environment variables to your Boost installation.
#
# Depending on how you installed Boost, you may need to change -lboost_thread
# to -lboost_thread-mt
fs: ${FS_OBJS} ${LIBFSSERVER}
	./autotag.sh push
	${CC} -o $@ $^ -l${BOOST_THREAD} -lboost_regex -lboost_system -pthread -ldl -lboost_chrono

# Compile each test program separately
%: %.cpp ${LIBFSCLIENT}
	$(CC) -o $@ $^

# Generic rules for compiling a source file to an object file
%.o: %.cpp
	${CC} -c $<
%.o: %.cc
	${CC} -c $<

clean:
	rm -rf ${FS_OBJS} fs $(TESTS) *.dSYM
