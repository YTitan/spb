
CC = gcc

CFLAGS = -Wall -Wextra --std=gnu99 -pedantic -Werror

tee: tee.o
	$(CC) tee.o -o tee

tee.o: tee.c
	$(CC) -c $(CFLAGS) tee.c
