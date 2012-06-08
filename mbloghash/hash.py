#!/usr/bin/python
import sys, os





def main():
    if sys.argv[1] == 'start':
        ret=os.fork()
        if ret == 0:
            os.execlp('./HashServer', '')
        else:
            f=open('hashserverpid', 'w')
            f.write(str(ret))
            f.close()
    elif sys.argv[1] == 'stop':
        f=open('hashserverpid', 'r')
        pid=f.read()
        os.system('kill -10 '+str(pid))



if __name__ == '__main__':
    main()
