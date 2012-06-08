# -*- coding: utf-8 -*-
#__author__ = 'medcl'
#modified superpig119
#site 'http://log.medcl.net'
#email 'm@medcl.net'
 
from glob import glob
import string
import urllib
import urllib2
import thread 
from time import strftime, localtime,sleep,ctime
from datetime import timedelta, date
import os
import threading
import sys, getopt
 
 
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
    -x      specify the number of the sub-folder,calculated by userid % num_of_subfolder
    -k      skip_count
    -h      show help.
    -s      [on] [off] shutdown after download finished.
    '''
 
 
def download(thread_n,thread_all,input_file,output_folder,concurrent_thread,skip_count):
    #save log file
    log_file = open(input_file,'r')
 
    count=0;
    for line in log_file:
        count+=1;
        if(skip_count>0):
            if(count<skip_count):
                continue;        
        if(count%thread_all==thread_n):        
            #print line
            line=string.strip(line)
            try:
                url= ('%s%s'% ("http://v.t.sina.com.cn/widget/getusermblog.php?uid=",line))
 
                mon = strftime("%m",localtime())
                day = strftime("%d",localtime())
 
                date_str="%s-%s" % (mon,day)                
                #sub_folder="%s/%s/%s" % (output_folder,date_str,(int(line) % int(num_of_subfolder)));
 
                #if(not os.path.exists(sub_folder)):         
                 #   try:
                  #      os.makedirs(sub_folder);
                   # except Exception,e:
                    #    print e
                     #   pass
                file_name=string.strip("%s/%s%s"% (output_folder,line,".json"))
				
 
                file = urllib2.urlopen(url)

                output = open(file_name,'wb')
                msg=file.read()
                msg=msg.decode('unicode_escape')
                #print msg;
                output.write(msg.encode('utf-8'))
                output.close()        
                print  "thread:%s,count:%s,file:%s" % (thread_n,count,file_name);        
 
            except ValueError:
                  print 'error'
                  pass
    log_file.close()
 
#download(0,2);
#download(1,2);
 
opts, args = getopt.getopt(sys.argv[1:], "hi:o:c:x:k:s:")
input_file="sina_userId"

mon = strftime("%m",localtime())
day = strftime("%d",localtime())
date_str="%s-%s" % (mon,day) 
if(not os.path.exists(date_str)):         
	try:
		os.makedirs(date_str);
	except Exception,e:
		print e
		pass

output_folder = date_str

concurrent_thread=15;
#sub_folder=20;
skip_count=0;
shutdown_after_finish='off';
 
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
    #elif op== "-x":
     #    sub_folder=int(value)
    elif op == "-h":
         usage()
         sys.exit()
 
print 'starting:',ctime()       
threads=[]
#set thread_count here
thread_count=concurrent_thread;
for i in range(0,thread_count):
      print i
      t=threading.Thread(target=download,args=(i,thread_count,input_file,output_folder,concurrent_thread,skip_count))              
      threads.append(t)
for i in range(0,thread_count):
      threads[i].start()
for i in range(0,thread_count):
      threads[i].join()     
 
print 'finished:',ctime()
#shutdown after downloads finished
 
if(shutdown_after_finish=='on'):
    os.system('shutdown /s /t 300')
