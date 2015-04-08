CC = gcc
CFLAGS = -std=c99 -g -Wall -I../../include 
LDLIBS = -lncurses -lm -lgrid -L ../../lib/
CPPFLAGS=
PATH_LIB=../../lib
PATH_BIN=../../bin
all:$(PATH_BIN)/jeu_ncurses
install:

$(PATH_BIN)/jeu_ncurses: main-jeu.o $(PATH_LIB)/*
	$(CC) -o $(PATH_BIN)/jeu_ncurses main-jeu.o $(LDLIBS)

include dep

.PHONY: clean
clean:
	rm -f dep *.o *~ jeu

dep:
	$(CC) $(CPPFLAGS) -MM *.c >dep

