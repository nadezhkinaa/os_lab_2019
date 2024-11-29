CC=gcc

all: client server

client: client.c libmultmodulo.so
	$(CC) client.c multmodulo.o -L -lmultmodulo -pthread -o client

server: server.c libmultmodulo.so
	$(CC) server.c multmodulo.o -L -lmultmodulo -pthread -o server

libmultmodulo.so: multmodulo.o
	$(CC) -shared multmodulo.o -o libmultmodulo.so

multmodulo.o : multmodulo.c
	$(CC) -c -fPIC multmodulo.c -o multmodulo.o

clean :
	rm client server libmultmodulo.so multmodulo.o