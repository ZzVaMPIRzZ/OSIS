CC=gcc
CFLAGS=-I.

parallel_min_max : utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o parallel_min_max utils.o find_min_max.o parallel_min_max.c $(CFLAGS)

utils.o : utils.h
	$(CC) -o utils.o -c utils.c $(CFLAGS)

find_min_max.o : utils.h find_min_max.h
	$(CC) -o find_min_max.o -c find_min_max.c $(CFLAGS)

zombie :
	$(CC) -o zombie zombie_processes.c

process_memory :
	$(CC) -o process_memory process_memory.c

psum.o :
	$(CC) -c parallel_sum.c -o psum.o -pthread $(CFLAGS)

psum : libsum.a psum.o utils.o
	$(CC) psum.o utils.o -Lbin -lsum -o psum -pthread $(CFLAGS)

sum.o : sumfunc.h
	$(CC) -fPIC -o sum.o -c sumfunc.c -pthread $(CFLAGS)

clean :
	rm utils.o find_min_max.o parallel_min_max process_memory zombie psum sum.o psum.o

libsum.a : sum.o
	mkdir bin -p
	ar rcs bin/libsum.a sum.o

all : 
	$(MAKE) parallel_min_max
	$(MAKE) process_memory
	$(MAKE) zombie
	$(MAKE) psum
