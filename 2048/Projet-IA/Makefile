CC = gcc
CFLAGS =-std=c99 -g -Wall 
LDLIBS = -lncurses -lm 
CPPFLAGS= 

all: test-IAYordan test-IA1 test-IAJimmy-1 test-IAJimmy-2 test-IAJimmy-3
 
test-IA1: main-IA1.o ../Projet-Jeu/grid.o
	$(CC) -o test-IA1 main-IA1.o ../Projet-Jeu/grid.o  $(LDLIBS)
test-IAYordan: main-IAYordan.o ../Projet-Jeu/grid.o
	$(CC) -o test-IAYordan main-IAYordan.o ../Projet-Jeu/grid.o  $(LDLIBS)
test-IAJimmy-1: main-IAJimmy-1.o ../Projet-Jeu/grid.o ../Projet-Jeu/gridIA.o
	$(CC) -o test-IAJimmy-1 main-IAJimmy-1.o ../Projet-Jeu/grid.o ../Projet-Jeu/gridIA.o $(LDLIBS)
test-IAJimmy-2: main-IAJimmy-2.o ../Projet-Jeu/grid.o ../Projet-Jeu/gridIA.o
	$(CC) -o test-IAJimmy-2 main-IAJimmy-2.o ../Projet-Jeu/grid.o ../Projet-Jeu/gridIA.o $(LDLIBS)
test-IAJimmy-3: main-IAJimmy-3.o ../Projet-Jeu/grid.o ../Projet-Jeu/gridIA.o
	$(CC) -o test-IAJimmy-3 main-IAJimmy-3.o ../Projet-Jeu/grid.o ../Projet-Jeu/gridIA.o $(LDLIBS)

include dep

.PHONY: clean
clean:
	rm -f dep *.o *~ all
dep:
	$(CC) $(CPPFLAGS) -MM *.c > dep


