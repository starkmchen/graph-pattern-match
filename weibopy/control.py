#!/usr/bin/python
import sys, os



def main():
    if sys.argv[1] == 'start':
        ret=os.fork()
        if ret == 0:
            os.execlp('./server.py', '')
        else:
            f=open('parserserverpid', 'w')
            f.write(str(ret))
            f.close()
    elif sys.argv[1] == 'stop':
        f=open('parserserverpid', 'r')
        pid=f.read()
        os.system('kill -10 '+str(pid))
        f.close()
    elif sys.argv[1] == 'flush':
        f=open('parserserverpid', 'r')
        pid=f.read()
        os.system('kill -12 '+str(pid))
        f.close()

if __name__ == '__main__':
    main()
