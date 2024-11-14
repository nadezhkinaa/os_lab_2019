CC=gcc
CFLAGS=-I.

all: parallel_min_max create_zombies  parallel_sum


parallel_min_max : utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o parallel_min_max utils.o find_min_max.o parallel_min_max.c $(CFLAGS)

create_zombies : 
	$(CC) -o create_zombies create_zombies.c $(CFLAGS)

process_memory : 
	$(CC) -o process_memory process_memory.c $(CFLAGS)

parallel_sum : utils.o sum.o
	$(CC) -pthread -o parallel_sum utils.o sum.o parallel_sum.c $(CFLAGS)

utils.o : utils.h
	$(CC) -o utils.o -c utils.c $(CFLAGS)

sum.o :
	$(CC) -o sum.o -c sum.c $(CFLAGS)

parallel_sum_lib : utils.o  libsum
	$(CC) -o parallel_sum utils.o parallel_sum.c -L/workspaces/os_lab_2019/lab4/src -lsum -pthread $(CFLAGS)

libsum :
	gcc -c sum.c -o sum.o
	ar rcs libsum.a sum.o
	rm sum.o

find_min_max.o : utils.h find_min_max.h
	$(CC) -o find_min_max.o -c find_min_max.c $(CFLAGS)

clean :
	rm utils.o find_min_max.o  parallel_min_max create_zombies process_memory parallel_sum libsum.a