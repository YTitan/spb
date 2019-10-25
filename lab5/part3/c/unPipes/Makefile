CC=gcc

SRCDIR=src
OBJDIR=obj

app: $(OBJDIR)/app.o
	$(CC) $(OBJDIR)/app.o -o app

$(OBJDIR)/app.o: $(SRCDIR)/app.c
	$(CC) -c $(SRCDIR)/app.c -o $(OBJDIR)/app.o
