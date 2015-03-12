CC = gcc
CFLAGS = -std=c99 -g -Wall 
LDLIBS = -lncurses -lm -lgrid -L ../lib/
CPPFLAGS= 

all:jeu_ncurses
 
jeu_ncurses: main-jeu.o 
	$(CC) -o jeu_ncurses main-jeu.o $(LDLIBS)

include dep

.PHONY: clean
clean:
	rm -f dep *.o *~ jeu

dep:
	$(CC) $(CPPFLAGS) -MM *.c >dep

