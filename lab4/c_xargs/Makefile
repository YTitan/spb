	 
CC = gcc

CFLAGS = -Wall -Wextra --std=gnu99 -pedantic -Werror

xargs: xargs.o
	$(CC) xargs.o -o xargs

xargs.o: xargs.c
	$(CC) -c $(CFLAGS) xargs.c
