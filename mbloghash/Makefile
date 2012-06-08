

BOOST_DIR = /usr/include/boost
THRIFT_DIR = /usr/local/include/thrift

DYLIBDIR = /home/mapred/dylib

GEN_SRC = mbhashframe/*.cpp
GEN_INCLUDE = mbhashframe



LINKOPT = -lthrift

CFLAGS = -Wl,-rpath,$(DYLIBDIR) -L$(DYLIBDIR)


all: HashServer client

client: testhash.cpp
	g++ -o $@ $< $(CFLAGS) $(GEN_SRC) -I$(GEN_INCLUDE) -I$(BOOST_DIR) -I$(THRIFT_DIR) $(LINKOPT)


HashServer: HashServer.cpp MyStr.h
	g++ -o $@ $< $(CFLAGS) $(GEN_SRC) -I$(GEN_INCLUDE) -I$(BOOST_DIR) -I$(THRIFT_DIR) -L$(DYLIBDIR) $(LINKOPT)


clean :
	rm HashServer client