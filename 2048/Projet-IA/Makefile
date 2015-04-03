CC = gcc
CFLAGS =-std=c99 -g -Wall -fPIC
LDLIBS = -lstrat -lgrid  -lm -lncurses  -L ../Projet-Jeu/lib/ -L.
CPPFLAGS= 



all: verif-libgrid  test-graphique-fast test-repetition-fast #deplacement 




verif-libgrid:
	if [ ! -f "../Projet-Jeu/lib/libgrid.a" ];then $(MAKE) -C ../Projet-Jeu; fi

deplacement:
	if [ ! -d "bin" ];then mkdir bin; fi
	mv test* bin/

#test-seb: strategy_seb.o
#	$(CC) -o test-seb strategy_seb.o   $(LDLIBS)

test-graphique-fast: main-graphique.o 
	$(CC) -o test-graphique-fast main-graphique.o   $(LDLIBS)

test-repetition-fast: main-graphique.o 
	$(CC) -o test-repetition-fast main-graphique.o   $(LDLIBS)

test-graphique-low: main-graphique.o 
	$(CC) -o test-graphique-low main-graphique.o    $(LDLIBS)

test-repetition-low: main-graphique.o
	$(CC) -o test-repetition-low  main-graphique.o   $(LDLIBS)


include dep

.PHONY: clean
clean:
	rm -f dep *.o *~ 
	if [ -d "bin" ];then rm -rf bin/; fi
dep:
	$(CC) $(CPPFLAGS) -MM *.c > dep


