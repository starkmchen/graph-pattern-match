#!/usr/bin/python
import signal
import sys
import threading
import time



class MyLog:
  def __init__(self, adt, itemmax=-1):
    self.itemmax=itemmax
    self.itemnum=0
    self.adt=adt

  def _createfilename(self):
    tm=time.localtime()
    time_str="%02d-%02d-%02d-%02d-%02d"%(tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec)
    fname = "%s/%s"%(self.logdir,self.logprefix) + time_str
    return fname

  def initFile(self, dir, prefix):
    self.logdir=dir
    self.logprefix=prefix
    self.f=open(self._createfilename(), 'w')
    if self.f == None:
      return -1
    return 0

  def flush(self):
    if self.f is not None:
      self.f.flush()

  def close(self):
    if self.f is not None:
      self.f.close()

  def write(self, cont):
    if self.f == None:
      return
    if self.adt == 1:
      tm=time.localtime()
      time_str="%02d-%02d-%02d-%02d-%02d"%(tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec)
      self.f.write(time_str+': ')
    self.f.write(cont)
    if self.itemmax > 0:
      self.itemnum += 1
      if self.itemnum == self.itemmax:
        self.f.close()
        self.initFile(self.logdir, self.logprefix)
        self.itemnum = 0




def sig_usr1(signo, stack):
    print signo



def main():
    log = MyLog(1, 10)
    log.initFile('/home/mapred/statelog', 'chenmin')
    for i in range(0, 20):
        log.write(str(i)+'\n')
        time.sleep(0.2)

    log.close()



if __name__ == '__main__':
    main()
