SRC = main.cpp jsongrabtest.cpp

jsongrab: $(SRC)
	g++  -g -o jsongrab ${SRC}   -lmysqlclient -L /usr/lib -ljson

clean:
	rm -f jsongrab $(obj)
