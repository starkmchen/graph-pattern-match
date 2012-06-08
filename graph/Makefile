
BOOST_DIR = /usr/local/boost/include/boost-1_33_1/
THRIFT_DIR = /usr/local/include/thrift

GEN_SRC = gen-cpp/Hbase.cpp gen-cpp/Hbase_constants.cpp gen-cpp/Hbase_types.cpp 

DYLIBDIR = /home/mapred/dylib
GRAPH =graph.cpp graph.h

#CFLAGS = -O2

match : match2.cpp $(GRAPH)
	g++ -g -o $@ $(CFLAGS) match2.cpp $(GRAPH) $(GEN_SRC)  -I${THRIFT_DIR} -I${BOOST_DIR} -Igen-cpp/ $(DYLIBDIR)/libthrift.so


clean:
	rm match
