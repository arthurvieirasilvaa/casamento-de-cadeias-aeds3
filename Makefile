CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c99

all: tp2

tp2: tp.o main.o
	$(CC) $(CFLAGS) -o $@ $^

tp2.o: tp.c tp.h
	$(CC) $(CFLAGS) -c -o $@ $<

main.o: main.c tp.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o tp2