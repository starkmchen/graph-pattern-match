# -*- coding: utf-8 -*-
__author__ = 'medcl'
#site 'http://log.medcl.net'
#email 'm@medcl.net'
 
from glob import glob
import string
import urllib
import urllib2
import thread 
from time import strftime, localtime,sleep,ctime,time
from datetime import timedelta, date
import os
import threading
import sys, getopt,re

sys.path.append('/home/mapred/dylib')

from sendjson import SendJson
from sendjson.ttypes import *
from thrift import Thrift
from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol

 
 
def usage():
    print '''
    downloading mircoblogs from weibo.com,written by @medcl,url:http://log.medcl.net;
    -------------------------------------------------------
    usage:
        rss_downloader.py [-i file_of_sina_user_id] [-o output_folder] [-c num_of_threads] [-x num_of_sub_folder]
 
    example:
        rss_downloader.py -i sina_userId -o data -c 15 -x 20  -k 100  -s on
 
    -i      the location of input file which containing the sina userid
    -o      the folder where the downloading files will be stored
    -c      number of the working threads,these threads will working concurrently
    -k      skip_count
    -h      show help.
    -s      [on] [off] shutdown after download finished.
    '''
 
 
def download(thread_n,thread_all,input_file,output_folder,skip_count):
    global jsonclient
    log_file = open(input_file,'r')
    fileli=[]
    count=0;
    filecnt=0
    for line in log_file:
        if(count%thread_all==thread_n):        
            #print line
            line=string.strip(line)
            if len(line) == 0:
                break
            try:
                url= ('%s%s'% ("http://v.t.sina.com.cn/widget/getusermblog.php?uid=",line))
 
                time_str=str(long(time()))
                file_name="%s/%s%s%s%s" % (output_folder,time_str, str(thread_n), filecnt, ".json");
                filecnt+=1
                if(not os.path.exists(output_folder)):
                    try:
                        os.makedirs(output_folder);
                    except Exception,e:
                        print e
                        pass
 
                file = urllib2.urlopen(url)
                sleep(skip_count)
                output = open(file_name,'wb')
                msg=file.read()
                msg=msg.decode('unicode_escape')
                #print msg;
                msg=msg.encode('utf-8')
                output.write(msg)
                output.close()        

                fileli.append(file_name)
                if len(fileli) == 10:
                    jsonclient.SendJsonFileName(fileli)
                    fileli = []

                print  "thread:%s,count:%s,file:%s" % (thread_n,count,file_name);        
            except ValueError:
                  print 'error'
                  pass
        count+=1
    jsonclient.SendJsonFileName(fileli)
    log_file.close()
 
#download(0,2);
#download(1,2);


outfile=open('log.txt', 'w')
sys.stdout=outfile
sys.stderr=outfile

opts, args = getopt.getopt(sys.argv[1:], "hi::o::c::k::s::")
#input_file="sina_userid"
output_folder="/home/mapred/weibodata"
concurrent_thread=5;
skip_count=0.5
shutdown_after_finish='off';

jsontransport = TSocket.TSocket("localhost", 9085)
jsontransport = TTransport.TBufferedTransport(jsontransport)
jsonprotocol = TBinaryProtocol.TBinaryProtocol(jsontransport)
jsonclient = SendJson.Client(jsonprotocol)
jsontransport.open()

for op, value in opts:
    if op == "-i":
         input_file = value
    elif op == "-o":
         output_folder = value
    elif op== "-c":
         concurrent_thread=int(value)
    elif op== "-s":
         shutdown_after_finish=value
    elif op== "-k":
         skip_count=int(value)
    elif op == "-h":
         usage()
         sys.exit()


jsonclient.SendJsonFileName([output_folder+'/'+f for f in os.listdir(output_folder)])
 
print 'starting:',ctime()       
threads=[]
#set thread_count here
thread_count=concurrent_thread;
for i in range(0,thread_count):
      t=threading.Thread(target=download,args=(i,thread_count,input_file,output_folder,skip_count))              
      threads.append(t)
for i in range(0,thread_count):
      threads[i].start()
for i in range(0,thread_count):
      threads[i].join()     

jsontransport.close() 
print 'finished:',ctime()
#shutdown after downloads finished
 
if(shutdown_after_finish=='on'):
    os.system('shutdown /s /t 300')

outfile.close()
