CC=gcc
CFLAGS=-pthread
SRCDIR=src
OBJDIR=obj

app: $(OBJDIR)/app.o
	$(CC) $(CFLAGS) $(OBJDIR)/app.o -o app

$(OBJDIR)/app.o: $(SRCDIR)/app.c
	$(CC) -c $(CFLAGS) $(SRCDIR)/app.c -o $(OBJDIR)/app.o
