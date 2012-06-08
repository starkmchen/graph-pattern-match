
BOOST_DIR = /usr/local/boost/include/boost-1_33_1/
THRIFT_DIR = /usr/local/include/thrift

GENINCLUDEDIR = SendJson-gen-cpp
GENINCLUDEDIR1 = mbhashframe
GENINCLUDEDIR2 = Hbasegen-cpp
JSONGRABDIR = jsongrab
DYLIBDIR = /home/mapred/dylib

GEN_SRC = $(GENINCLUDEDIR)/SendJson.cpp $(GENINCLUDEDIR)/sendjson_constants.cpp $(GENINCLUDEDIR)/sendjson_types.cpp 

GEN_SRC1= $(GENINCLUDEDIR1)/MBlogHash.cpp $(GENINCLUDEDIR1)/mbloghash_constants.cpp $(GENINCLUDEDIR1)/mbloghash_types.cpp

GEN_SRC2 = $(GENINCLUDEDIR2)/Hbase.cpp $(GENINCLUDEDIR2)/Hbase_constants.cpp $(GENINCLUDEDIR2)/Hbase_types.cpp


JSON_SRC= $(JSONGRABDIR)/jsongrab5.5.cpp

CLIENTSRC = testclient.cpp
SERVERSRC = ParserServer.cpp

LINKOPT = -lthrift -ljson -lpthread -lmysqlclient 

CFLAGS = -g -Wl,-rpath,/home/mapred/dylib -L$(DYLIBDIR)

all : ParserServer client

getid: hbaseid.cpp
	g++ -o $@ $< $(GEN_SRC2) -I${THRIFT_DIR} -I${BOOST_DIR} -I$(GENINCLUDEDIR2) $(LINKOPT)


client: $(CLIENTSRC)
	g++ -o $@ $(CLIENTSRC) $(GEN_SRC) $(CFLAGS) -I${THRIFT_DIR} -I${BOOST_DIR} -I$(GENINCLUDEDIR) $(LINKOPT)

ParserServer: $(JSON_SRC) $(SERVERSRC) MyLog.h
	g++ -o $@ $(SERVERSRC) $(GEN_SRC)  $(GEN_SRC1) $(GEN_SRC2) $(JSON_SRC) $(CFLAGS) $(LINKOPT) -I${THRIFT_DIR} -I${BOOST_DIR} -I$(GENINCLUDEDIR) -I$(GENINCLUDEDIR1) -I$(GENINCLUDEDIR2) -I$(JSONGRABDIR) 

clean:
	rm ParserServer client