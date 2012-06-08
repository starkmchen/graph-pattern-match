#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<sys/time.h>

#include<protocol/TBinaryProtocol.h>
#include<transport/TSocket.h>
#include <transport/TTransportUtils.h>

#include<vector>
#include<string>
#include<Hbase.h>

using namespace std;
using namespace apache::hadoop::hbase::thrift;
using namespace boost;

using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;
using namespace apache::thrift;



int main()
{
  shared_ptr<TTransport> socket(new TSocket("192.168.1.163", 9090));
  shared_ptr<TTransport> transport(new TBufferedTransport(socket));
  shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
  HbaseClient client(protocol);

  try
    {
      transport->open();
      vector<string>tbname;
      Mutation muta;
      vector<Mutation>vmuta;

      try
	{
	  muta.column="f1:no";
	  muta.value="12312";
	  muta.isDelete=0;
	  vmuta.push_back(muta);
	  client.mutateRow("t1", "chenminisme", vmuta);
	}
      catch(IOError &io)
	{
	  cout << io.message << endl;
	}
      transport->close();
    }
  catch(TException &tx)
    {
      printf("Error %s\n", tx.what());
    }
}
