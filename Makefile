CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c99

all: tp3

tp3: tp.o main.o
	$(CC) $(CFLAGS) -o $@ $^

tp3.o: tp.c tp.h
	$(CC) $(CFLAGS) -c -o $@ $<

main.o: main.c tp.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o tp3