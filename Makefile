CC=gcc
CFLAGS=-g -O1 -Wall
LDLIBS=-lpthread

all:  echoclient echoserver

echoclient: echoclient.c csapp.h csapp.c
echoserver: echoserver.c echo.c csapp.h csapp.c sbuf.h sbuf.c


clean:
	rm -f *.o *~ *.exe  echoclient echoserver csapp.o
