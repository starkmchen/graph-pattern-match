
BOOST_DIR = /usr/local/boost/include/boost-1_33_1/
THRIFT_DIR = /usr/local/include/thrift

GEN_SRC = gen-cpp/Hbase.cpp gen-cpp/Hbase_constants.cpp gen-cpp/Hbase_types.cpp 
DYLIBDIR = /home/mapred/dylib

#CFLAGS = -O2

fillhbase: fillhbase.cpp
	g++ -o $@ $(CFLAGS) fillhbase.cpp $(GEN_SRC)  -I${THRIFT_DIR} -I${BOOST_DIR} -Igen-cpp/ $(DYLIBDIR)/libthrift.so


clean:
	rm fillhbase
