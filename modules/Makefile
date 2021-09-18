CC = gcc
HEADERS = include/encryption_manager.h

default:
	make ransom

encryption_manager.o: src/encryption_manager.c $(HEADERS)
	gcc -c src/encryption_manager.c

ransom: encryption_manager.o
	gcc -lm -o ransom encryption_manager.o

clean:
	-rm -f encryption_manager.o
	-rm -f ransom
