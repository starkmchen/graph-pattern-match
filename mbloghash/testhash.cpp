#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<sys/time.h>

#include<protocol/TBinaryProtocol.h>
#include<transport/TSocket.h>
#include <transport/TTransportUtils.h>
#include "MBlogHash.h"
#include<vector>
#include<string>

using namespace std;
using namespace boost;

using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;
using namespace apache::thrift;

using namespace mbloghash;


int main()
{
  shared_ptr<TTransport> socket(new TSocket("192.168.1.163", 9084));
  shared_ptr<TTransport> transport(new TBufferedTransport(socket));
  shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
  MBlogHashClient client(protocol);

  transport->open();
  vector<string>in;
  vector<string>out;
  in.push_back("123");
  client.hashBlogId(out, in);
  for(int i=0; i<out.size(); i++)
    {
      cout << out[i] << endl;
    }
  transport->close();
}



