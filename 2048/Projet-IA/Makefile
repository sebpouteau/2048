CC = gcc
CFLAGS =-std=c99 -g -Wall 
LDLIBS = -lgrid -lm -lncurses -L ../Projet-Jeu/lib/
CPPFLAGS= 


all: verif-libgrid test-yordan test-seb2 test-jimmy deplacement 



verif-libgrid:
	if [ ! -f "../Projet-Jeu/lib/libgrid.a" ];then $(MAKE) -C ../Projet-Jeu; fi

deplacement:
	if [ ! -d "bin" ];then mkdir bin; fi
	mv test* bin/

#test-seb: strategy_seb.o
#	$(CC) -o test-seb strategy_seb.o   $(LDLIBS)

test-seb2: strategy_seb2.o
	$(CC) -o test-seb2 strategy_seb2.o   $(LDLIBS)

test-jimmy: strategy_jimmy.o
	$(CC) -o test-jimmy strategy_jimmy.o   $(LDLIBS)

test-yordan: strategy_yordan.o
	$(CC) -o test-yordan strategy_yordan.o   $(LDLIBS)

test-ysa: strategy_ysa.o
	$(CC) -o test-ysa strategy_ysa.o   $(LDLIBS)


include dep

.PHONY: clean
clean:
	rm -f dep *.o *~ 
	if [ -d "bin" ];then rm -rf bin/; fi
dep:
	$(CC) $(CPPFLAGS) -MM *.c > dep


