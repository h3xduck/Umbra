CC = gcc
HEADERS = lib/RawTCP.h
EXTRA_CFLAGS= -I$(PWD)/lib

default: client

client.o: client.c $(HEADERS)
	gcc -c client.c lib/libRawTCP_Lib.a -o client.o

client: client.o
	gcc client.o lib/libRawTCP_Lib.a -o client

clean:
	-rm -f client.o
	-rm -f client
