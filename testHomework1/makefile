CC=gcc
CFLAGS=-std=c99
DBFLAGS=-g

make: votecounter.c
	$(CC) $(CFLAGS) -o votecounter votecounter.c open_read.o

debug: votecounter.c
	$(CC) $(CFLAGS) $(DBFLAGS) -o votecounter votecounter.c open_read.o

clean:
	rm votecounter


