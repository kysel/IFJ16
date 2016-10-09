CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -pedantic -g

#source files
SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)

all: ifj16

ifj16: $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	mkdir -p .depend
	$(CC) -c $(CFLAGS) $< -o $@
	$(CC) $(CFLAGS) -MF".depend/$@.d" -MG -MM -MP -MT"$@" "$<"

clean:
	rm -rf *.o *.d .depend ifj16

-include .depend/*.d
