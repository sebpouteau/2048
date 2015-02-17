CC = gcc
CFLAGS =-std=c99 -g -Wall 
LDLIBS = -lncurses -lm 
CPPFLAGS= 

all:test-IA1
 
test-IA1: main-IA1.o ../Projet-Jeu/grid.o
	$(CC) -o test-IA1 main-IA1.o ../Projet-Jeu/grid.o  $(LDLIBS)

 

include dep

.PHONY: clean
clean:
	rm -f dep *.o *~ test-IA1 
dep:
	$(CC) $(CPPFLAGS) -MM *.c >dep


