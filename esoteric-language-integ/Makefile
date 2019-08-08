CC      = gcc
CFLAGS  = -Wall -g -std=c99 -pedantic

.SUFFIXES: .c .o

all: integ

integ: integ.c integ-functions.o integ-functions.h
	$(CC) $(CFLAGS) integ-functions.o integ.c -o integ

integ-functions.o: integ-functions.c integ-functions.h
	$(CC) $(CFLAGS) -c integ-functions.c

clean:
	rm -rf *.o *.dSYM

.PHONY: clean all





