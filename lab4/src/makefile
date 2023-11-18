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

psum : libsum.a
	$(CC) -c parallel_sum.c -l sum -o psum -pthread $(CFLAGS)

sum.o : sumfunc.h
	$(CC) -o sum.o -c sumfunc.c -pthread $(CFLAGS)

clean :
	rm utils.o find_min_max.o parallel_min_max process_memory zombie psum sum.o psum.o libsum.a

libsum.a : sum.o
	ar rcs libsum.a sum.o


	