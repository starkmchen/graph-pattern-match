#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<sys/time.h>

#include<protocol/TBinaryProtocol.h>
#include<transport/TSocket.h>
#include <transport/TTransportUtils.h>

#include<vector>
#include<string>
#include<SendJson.h>

using namespace std;
using namespace boost;

using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;
using namespace apache::thrift;

using namespace sendjson;


int main()
{
  shared_ptr<TTransport> socket(new TSocket("192.168.1.163", 9085));
  shared_ptr<TTransport> transport(new TBufferedTransport(socket));
  shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
  SendJsonClient client(protocol);

  transport->open();
  
  vector<string>v;
  int i;

  //v.push_back("/home/mapred/weibodata/133511479640.json");
  v.push_back("/home/mapred/weibodata/133537747110.json");
  client.SendJsonFileName(v);

  transport->close();
}
