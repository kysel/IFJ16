CC=gcc
CFLAGS=-O2 -std=c11 -Wall -Wextra -pedantic -g

#source files
SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)

all: ifj16

ifj16: $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

tail: tail.o
	$(CC) $(CFLAGS) $^ -o $@

#test this
tail2: tail2.o
	$(CXX) $(CPPFLAGS) $^ -o $@

wordcount: wordcount.o io.o libhtable.a
	$(CC) $(CFLAGS) wordcount.o io.o -static libhtable.a -o $@

wordcount-dynamic: wordcount.o io.o libhtable.so
	$(CC) $(CFLAGS) wordcount.o io.o -L. libhtable.so -o $@

libhtable.a: $(LIBHTABOBJ)
	ar crs $@ $^

libhtable.so: $(LIBHTABSRC)
	$(CC) $(CFLAGS) -shared -fPIC $^ -o $@

tail2.o: tail2.cc
	mkdir -p .depend
	$(CXX) -c $(CPPFLAGS) $< -o $@

%.o: %.c
	mkdir -p .depend
	$(CC) -c $(CFLAGS) $< -o $@
	$(CC) $(CFLAGS) -MF".depend/$@.d" -MG -MM -MP -MT"$@" "$<"

clean:
	rm -rf *.o *.d .depend ifj16

-include .depend/*.d
