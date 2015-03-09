CC = gcc
CFLAGS = -std=c99 -g -Wall 
LDLIBS = -lncurses -lm -lgrid -L ../lib/
CPPFLAGS= 

all:jeu
 
jeu: main-jeu.o 
	$(CC) -o jeu main-jeu.o $(LDLIBS)

include dep

.PHONY: clean
clean:
	rm -f dep *.o *~ jeu

dep:
	$(CC) $(CPPFLAGS) -MM *.c >dep

