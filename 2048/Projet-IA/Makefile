CC = gcc
CFLAGS = -std=c99 -g -Wall -fPIC
LDLIBS = -lgrid  -lm -lncurses  -L ../Projet-Jeu/lib/ -L.
CPPFLAGS= 



all: verif-libgrid  test-graphique-fast test-repetition-fast deplacement 




verif-libgrid:
	if [ ! -f "../Projet-Jeu/lib/libgrid.a" ];then $(MAKE) -C ../Projet-Jeu; fi

deplacement:
	if [ ! -d "bin" ];then mkdir bin; fi
	mv test* bin/

#test-seb: strategy_seb.o
#	$(CC) -o test-seb strategy_seb.o   $(LDLIBS)

test-graphique-fast: main-graphique.o A2_Emery_Gouraud_Kirov_Pouteau_fast.o
	$(CC) -o test-graphique-fast main-graphique.o A2_Emery_Gouraud_Kirov_Pouteau_fast.o  $(LDLIBS)

test-repetition-fast: main-repetition.o A2_Emery_Gouraud_Kirov_Pouteau_fast.o
	$(CC) -o test-repetition-fast main-repetition.o A2_Emery_Gouraud_Kirov_Pouteau_fast.o  $(LDLIBS)

test-graphique-low: main-graphique.o 
	$(CC) -o test-graphique-low main-graphique.o  A2_Emery_Gouraud_Kirov_Pouteau_fast.o  $(LDLIBS)

test-repetition-low: main-graphique.o
	$(CC) -o test-repetition-low  main-repetition.o   $(LDLIBS)


include dep

.PHONY: clean
clean:
	rm -f dep *.o *~ 
	if [ -d "bin" ];then rm -rf bin/; fi
dep:
	$(CC) $(CPPFLAGS) -MM *.c > dep


