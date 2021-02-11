#Makefile for the Aho-Corasick-C program
#Coded by Martin Smith, Algorithm by Alfred Aho and Margaret Corasick 
PROJ=ac

SOURCES = AC.c AC_Trie.c IO.c SteQ.c State.c tests.c
OBJECTS = AC.o AC_Trie.o IO.o SteQ.o State.o tests.o
HEADERS = AC.h AC_Trie.h IO.h SteQ.h State.h tests.h
CC = gcc
CFLAGS = -g -Wall

$(PROJ): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

clean:
	rm -fR *.o ac
