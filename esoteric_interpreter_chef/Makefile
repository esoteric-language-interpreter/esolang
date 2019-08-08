CC      = gcc
CFLAGS  = -Wall -g -std=c99 -pedantic

chef_interpreter: chef_interpreter.o chef_method_functions.o

chef_interpreter.o : chef_interpreter.c chef_method_functions.h chef_interpreter.h

chef_method_functions : chef_method_functions.o chef_method_functions.h chef_interpreter.h

chef_method_functions.o : chef_method_functions.c chef_method_functions.h