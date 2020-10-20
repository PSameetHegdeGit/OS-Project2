.DEFAULT_GOAL := all

CC = gcc
CFLAGS = -g -w

all:
	$(CC) $(CFLAGS) -pthread -o create_thread create_thread.c -L../../ -lmypthread
	$(CC) $(CFLAGS) -pthread -o yield_exit_join_thread yield_exit_join_thread.c -L../../ -lmypthread
	$(CC) $(CFLAGS) -pthread -o yield_exit_join_thread_2 yield_exit_join_thread_2.c -L../../ -lmypthread
	$(CC) $(CFLAGS) -pthread -o yield_exit_join_thread_3 yield_exit_join_thread_3.c -L../../ -lmypthread
	$(CC) $(CFLAGS) -pthread -o mutex mutex.c -L../../ -lmypthread

clean:
	rm -rf create_thread yield_exit_join_thread yield_exit_join_thread_2 yield_exit_join_thread_3 mutex *.o peda* .gdb_history
