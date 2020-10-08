.DEFAULT_GOAL := all

CC = gcc
CFLAGS = -g -w

all:
	$(CC) $(CFLAGS) -pthread -o create_thread create_thread.c -L../../ -lmypthread
	$(CC) $(CFLAGS) -pthread -o yield_thread yield_thread.c -L../../ -lmypthread
	$(CC) $(CFLAGS) -pthread -o join_exit join_exit_thread.c -L../../ -lmypthread

clean:
	rm -rf create_thread join_exit yield_thread *.o

