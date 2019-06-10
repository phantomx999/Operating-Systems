CC=gcc
CFLAGS=-std=c99
DBFLAGS=-g

make: btree.c
	$(CC) $(CFLAGS) -o comp.out btree.c

bebug: intro.c
	$(CC) $(CFLAGS) $(DBFLAGS) -o comp.out btree.c

clean:
	rm comp.out
