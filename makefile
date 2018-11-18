#Makefile for the Aho-Corasick-C program
#Coded by Martin Smith, Algorithm by Alfred Aho and Margaret Corasick 
PROJ=AC
OBJDIR=objects

SOURCES = AC.c Build.c IO.c SteQ.c State.c Output.c tests.c
OBJECTS = AC.o Build.o IO.o SteQ.o State.o Output.o tests.o
HEADERS = AC.h Build.h IO.h SteQ.h State.h Output.h tests.h
CC = gcc
CFLAGS = -g -Wall

$(PROJ): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

clean:
	rm -fR *.o ac
