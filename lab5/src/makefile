CC=gcc
CFLAGS=-I.

mutex : mutex.c
	$(CC) mutex.c -o mutex -pthread $(CFLAGS)

fact : factorial.c
	$(CC) factorial.c -o fact -pthread $(CFLAGS)

dead : deadlock.c
	$(CC) deadlock.c -o deadlock -pthread $(CFLAGS)

clean :
	rm mutex fact