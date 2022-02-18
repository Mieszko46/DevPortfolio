CC=gcc
CFLAGS=-O3 -Wall
LIBS=-pthread

all: server.out
server.out: server.o
	$(CC) $(CFLAGS) $(LIBS) -o server.out server.o
server.o: server.c
	$(CC) $(CFLAGS) -c server.c
string.o: string.c
	$(CC) $(CFLAGS) -c string.c

.PHONY: clean
clean:
	rm -f *.o *.out