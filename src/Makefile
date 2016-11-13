CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -pedantic -g
SHELL := /bin/bash

#source files
TESTSOURCES=$(wildcard *test.c)
SOURCES=$(filter-out $(TESTSOURCES),$(wildcard *.c))
OBJECTS=$(SOURCES:.c=.o)

all: ifj16

test: binaryTree-test find-test ifj16-test

ifj16: $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

binaryTree-test: $(filter-out main.o,$(OBJECTS))
	$(CC) $(CFLAGS) $^ $@.c -o $@
	valgrind -q --leak-check=full ./$@
	
find-test: $(filter-out main.o,$(OBJECTS))
	$(CC) $(CFLAGS) $^ $@.c -o $@
	valgrind -q --leak-check=full ./$@	

%.o: %.c
	mkdir -p .depend
	$(CC) -c $(CFLAGS) $< -o $@
	$(CC) $(CFLAGS) -MF".depend/$@.d" -MG -MM -MP -MT"$@" "$<"

clean:
	rm -rf *.o *.d .depend ifj16 find-test binaryTree-test test/input/*.log

ifj16-test: ifj16
	cd test/; ./test; \

-include .depend/*.d
