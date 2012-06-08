
#include "MyHeaders.h"
#include "Hbase.h"
#include<transport/TSocket.h>
#include <transport/TTransportUtils.h>
#include <protocol/TBinaryProtocol.h>
#include <server/TSimpleServer.h>
#include <server/TThreadedServer.h>
#include <transport/TServerSocket.h>
#include <transport/TBufferTransports.h>


using namespace std;
using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace apache::hadoop::hbase::thrift;
using namespace boost;


#define HBASESERVER "192.168.1.163"
#define HBASEPORT 9090


int main()
{
  shared_ptr<TTransport> hbasesocket(new TSocket(HBASESERVER, HBASEPORT));
  shared_ptr<TTransport> hbasetransport(new TBufferedTransport(hbasesocket));
  shared_ptr<TProtocol> hbaseprotocol(new TBinaryProtocol(hbasetransport));
  HbaseClient hbaseclient(hbaseprotocol);
  
  hbasetransport->open();

  vector<string>v;
  vector<TRowResult>re;

  ScannerID id;

  v.push_back("content");

  id=hbaseclient.scannerOpen("weibo", "",v);
  
  int cnt=10;
  while(cnt--)
    {
      hbaseclient.scannerGet(re,id);
      cout << re[0].row << endl;
      cout << re[0].columns["content"].value << endl;
    }
  
  hbasetransport->close();
}
