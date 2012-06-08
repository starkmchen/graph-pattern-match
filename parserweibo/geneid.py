#!/usr/bin/python
import os,re
import string



logdir="/home/mapred/parserlog/"
iddir="/home/mapred/sinaid/"
idpriofile="sinaidprio"
logretno=15

class cmtsum:
    sum=0L
    cnt=0


userprio={}
weibocmt={}
idmap={}

def getprio(f):
    infotuples=[re.split(r'\s+', string.strip(item)) for item in f]
#    print infotuples
    for tuple in infotuples:
        if len(tuple) != 3:
            continue
        if weibocmt.has_key(tuple[0]):
            weibocmt[tuple[0]].cnt+=1
#            weibocmt[tuple[0]].sum+=long(tuple[1])+long(tuple[2])
        else :
            weibocmt[tuple[0]]=cmtsum()
            weibocmt[tuple[0]].cnt=1


def analyzelog():
    files=os.listdir(logdir)
    for i in files:
        fname=logdir+i
        f=open(fname, 'r')
        getprio(f)
        f.close()
    #       os.unlink(fname)

    for item in weibocmt.items():
        try:
            if item[1] > logretno :
                userprio[item[0]]=(item[1].cnt+logretno-1)/logretno
        except:
            pass


def getoldprio(f):
    for i in f:
        li=re.split(r'\s+', string.strip(i))
        if len(li[0]) == 0:
            continue
        userprio[li[0]]=1
    f.close()
#    print userprio



def analyzeidf():
    files=os.listdir(iddir)
    for i in files:
        fname=iddir+i
        f=open(fname, 'r')
        getoldprio(f)
        f.close()


def delfile():
    for f in os.listdir(iddir):
        if f != idpriofile:        
            os.unlink(iddir+f)


def geneidfile():
    for item in userprio.items():
        if idmap.has_key(item[1]):
            idmap[item[1]].append(item[0])
        else :
            idmap[item[1]]=[]
            idmap[item[1]].append(item[0])

    for item in idmap.items():
        fname = iddir+'id'+str(item[0])
        f=open(fname, 'w')
        for i in item[1]:
            f.write(i+'\n')
        f.close()


def main():
    analyzeidf()
    analyzelog()
    delfile()
    geneidfile()


if __name__ == '__main__':
    main()
