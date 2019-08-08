CC      = gcc
CFLAGS  = -Wall -g -std=c99 -pedantic

.SUFFIXES: .c .o

all: arnoldc

arnoldc: arnoldc.o structure_functions.o
	$(CC) $(CFLAGS) structure_functions.o arnoldc.c -o arnoldc

structure_functions.o: structure_functions.c
	$(CC) $(CFLAGS) -c structure_functions.c


clean:
	rm -rf *.o *.dSYM

.PHONY: clean all




