CC=gcc
CFLAGS=

SRCDIR=src
OBJDIR=obj

all: server client

server: $(OBJDIR)/server.o
	$(CC) $(OBJDIR)/server.o -o server

client: $(OBJDIR)/client.o
	$(CC) $(OBJDIR)/client.o -o client

$(OBJDIR)/server.o: $(SRCDIR)/server.c $(SRCDIR)/serverData.h
	$(CC) -c $(SRCDIR)/server.c -o $(OBJDIR)/server.o

$(OBJDIR)/client.o: $(SRCDIR)/client.c $(SRCDIR)/serverData.h
	$(CC) -c $(SRCDIR)/client.c -o $(OBJDIR)/client.o
