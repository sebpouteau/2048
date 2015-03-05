CC = gcc
CFLAGS =-std=c99 -g -Wall 
LDLIBS = -lncurses -lm 
CPPFLAGS= 

all: test-IAYordan #test-IA1  # test-IAJimmy test-IAJimmy-1 test-IAJimmy-2 test-IAJimmy-3-TRICHE
 
test-IA1: main-IA1.o ../Projet-Jeu/src/grid.o
	$(CC) -o test-IA1 main-IA1.o ../Projet-Jeu/src/grid.o  $(LDLIBS)

test-IAYordan: main-IAYordan.o ../Projet-Jeu/src/grid.o 
	$(CC) -o test-IAYordan main-IAYordan.o ../Projet-Jeu/src/grid.o $(LDLIBS)

test-IAJimmy: main-IAJimmy.o ../Projet-Jeu/src/grid.o ../Projet-Jeu/src/gridIA.o
	$(CC) -o test-IAJimmy main-IAJimmy.o ../Projet-Jeu/src/grid.o ../Projet-Jeu/src/gridIA.o $(LDLIBS)
test-IAJimmy-1: main-IAJimmy-1.o ../Projet-Jeu/src/grid.o ../Projet-Jeu/src/gridIA.o
	$(CC) -o test-IAJimmy-1 main-IAJimmy-1.o ../Projet-Jeu/src/grid.o ../Projet-Jeu/src/gridIA.o $(LDLIBS)
test-IAJimmy-2: main-IAJimmy-2.o ../Projet-Jeu/src/grid.o ../Projet-Jeu/src/gridIA.o
	$(CC) -o test-IAJimmy-2 main-IAJimmy-2.o ../Projet-Jeu/src/grid.o ../Projet-Jeu/src/gridIA.o $(LDLIBS)
test-IAJimmy-3-TRICHE: main-IAJimmy-3-TRICHE.o ../Projet-Jeu/src/grid.o ../Projet-Jeu/src/gridIA.o
	$(CC) -o test-IAJimmy-3-TRICHE main-IAJimmy-3-TRICHE.o ../Projet-Jeu/src/grid.o ../Projet-Jeu/src/gridIA.o $(LDLIBS)

include dep

.PHONY: clean
clean:
	rm -f dep *.o *~ test-IA1 test-IAYordan test-IAJimmy test-IAJimmy-1 test-IAJimmy-2 test-IAJimmy-3-TRICHE
dep:
	$(CC) $(CPPFLAGS) -MM *.c > dep


