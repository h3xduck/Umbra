CC = gcc
HEADERS = lib/RawTCP.h
EXTRA_CFLAGS= -I$(PWD)/lib

default:
	make client

client.o: client.c $(HEADERS)
	gcc -c client.c

client: client.o lib/libRawTCP_Lib.a
	gcc -lm -o client client.o -L. lib/libRawTCP_Lib.a

clean:
	-rm -f client.o
	-rm -f client
