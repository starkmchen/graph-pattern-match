#!/usr/bin/python
import sys,time
sys.path.append('Hbase-py')
sys.path.append('/home/mapred/thrift-0.8.0/lib/py/build/lib.linux-x86_64-2.7')

from hbase import Hbase
from thrift import Thrift
from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol

class TRowResult:
    row=''
    columns={}

idlist=set()
def main():
    hbasetransport = TSocket.TSocket("192.168.1.163", 9090)
    hbasetransport = TTransport.TBufferedTransport(hbasetransport)
    hbaseprotocol = TBinaryProtocol.TBinaryProtocol(hbasetransport)
    hbaseclient=Hbase.Client(hbaseprotocol)
    hbasetransport.open()

    id=hbaseclient.scannerOpen('userrelation', '', ['follower'])
    count=0
    while True:
        li=hbaseclient.scannerGet(id)
        if len(li) == 0:
            break
        for item in li:
            idlist.add(item.row[0:10])
            ids=item.columns['follower:'].value
            for i in ids.split(':'):
                idlist.add(i)
        count=count+1

    time_str=str(long(time.time()))
    fname='/home/mapred/sinaid/'+'id' + time_str
    f=open(fname, 'w')
    for i in idlist:
        f.write(i+'\n')
    f.close()
        

    hbaseclient.scannerClose(id)
    hbasetransport.close()
            

if __name__ == '__main__':
    main()
