CC = gcc
CFLAGS = -std=c99 -g -Wall -I../../include 
LDLIBS = -lgridNcurses -lncurses -lgrid -lgcov -lm -L ../../lib/
CPPFLAGS=
PATH_LIB=../../lib
PATH_BIN=../../bin
all:$(PATH_BIN)/jeu_ncurses

$(PATH_BIN)/jeu_ncurses: main-jeu.o $(PATH_LIB)/*
	$(CC) -o $(PATH_BIN)/jeu_ncurses main-jeu.o $(LDLIBS)

install: $(PATH_LIB)/libgridNcurses.a 

$(PATH_LIB)/libgridNcurses.a: gridNcurses.o
	ar -cr $(PATH_LIB)/libgridNcurses.a gridNcurses.o



include dep

.PHONY: clean
clean:
	rm -f dep *.o *~ jeu *.g*

dep:
	$(CC) $(CPPFLAGS) -MM *.c >dep

