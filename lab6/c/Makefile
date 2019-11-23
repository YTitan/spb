
CC = gcc
CFLAGS = -pthread

all: server client

server: obj/server.o
	$(CC) $(CFLAGS) obj/server.o -o server

obj/server.o: src/server.c
	$(CC) $(CFLAGS) -c src/server.c -o obj/server.o

client: obj/client.o
	$(CC) obj/client.o -o client

obj/client.o: src/client.c
	$(CC) -c src/client.c -o obj/client.o
