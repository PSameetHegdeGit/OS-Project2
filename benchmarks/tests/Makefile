.DEFAULT_GOAL := all

CC = gcc
CFLAGS = -g -w

all:
	$(CC) $(CFLAGS) -pthread -o create_thread create_thread.c -L../../ -lmypthread

clean:
	rm -rf create_thread *.o

