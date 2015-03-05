CC = gcc
CFLAGS = -std=c99 -g -Wall 
LDLIBS = -lncurses -lm 
CPPFLAGS= 

all:jeu
 
jeu: main-jeu.o ../src/grid.o
	$(CC) -o jeu main-jeu.o ../src/grid.o  $(LDLIBS)

include dep

.PHONY: clean
clean:
	rm -f dep *.o *~ jeu

dep:
	$(CC) $(CPPFLAGS) -MM *.c >dep

