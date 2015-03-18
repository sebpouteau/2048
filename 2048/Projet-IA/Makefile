CC = gcc
CFLAGS =-std=c99 -g -Wall 
LDLIBS = -lgrid -lm -L ../Projet-Jeu/lib/
CPPFLAGS= 

all: verif-libgrid test-IA deplacement # test-IAYorda test-IAJimmy test-IAJimmy-1 test-IAJimmy-2 test-IAJimmy-3-TRICHE 

verif-libgrid:
	if [ ! -f "../Projet-Jeu/lib/libgrid.a" ];then $(MAKE) -C ../Projet-Jeu; fi

deplacement:
	if [ ! -d "bin" ];then mkdir bin; fi
	mv test* bin/
test-IA: main-IA.o 
	$(CC) -o test-IA main-IA.o   $(LDLIBS)

test-IAYordan: main-IAYordan.o  
	$(CC) -o test-IAYordan main-IAYordan.o  $(LDLIBS)

test-IAJimmy: main-IAJimmy.o  
	$(CC) -o test-IAJimmy main-IAJimmy.o $(LDLIBS)
test-IAJimmy-1: main-IAJimmy-1.o
	$(CC) -o test-IAJimmy-1 main-IAJimmy-1.o $(LDLIBS)
test-IAJimmy-2: main-IAJimmy-2.o
	$(CC) -o test-IAJimmy-2 main-IAJimmy-2.o  $(LDLIBS)
test-IAJimmy-3-TRICHE: main-IAJimmy-3-TRICHE.o  
	$(CC) -o test-IAJimmy-3-TRICHE main-IAJimmy-3-TRICHE.o  $(LDLIBS)


include dep

.PHONY: clean
clean:
	rm -f dep *.o *~ 
	if [ -d "bin" ];then rm -rf bin/; fi
dep:
	$(CC) $(CPPFLAGS) -MM *.c > dep


