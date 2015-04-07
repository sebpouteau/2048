CC = gcc
CFLAGS = -std=c99 -g -Wall -fPIC -I../include
LDLIBS = -lA2_Emery_Gouraud_Kirov_Pouteau_fast -lgrid  -lm -lncurses  -L ../lib/  
CPPFLAGS= 



all: verif-libgrid  IA-graphique-fast IA-repetition-fast deplacement 




verif-libgrid:
	if [ ! -f "../lib/libgrid.a" ];then $(MAKE) -C ../; fi
	if [ ! -f "A2_Emery_Gouraud_Kirov_Pouteau_fast.o" ];then $(MAKE) -C . A2_Emery_Gouraud_Kirov_Pouteau_fast.o; fi
	if [ ! -f "../lib/libA2_Emery_Gouraud_Kirov_Pouteau_fast.so" ];then gcc -shared -fPIC A2_Emery_Gouraud_Kirov_Pouteau_fast.o -o libA2_Emery_Gouraud_Kirov_Pouteau_fast.so;mv libA2_Emery_Gouraud_Kirov_Pouteau_fast.so ../lib/; fi

deplacement:
	if [ ! -d "../bin" ];then mkdir ../bin; fi
	mv IA* ../bin/

IA-graphique-fast: main-graphique.o 
	$(CC) -o IA-graphique-fast main-graphique.o   $(LDLIBS)

IA-repetition-fast: main-repetition.o 
	$(CC) -o IA-repetition-fast main-repetition.o   $(LDLIBS)

IA-graphique-low: main-graphique.o 
	$(CC) -o IA-graphique-low main-graphique.o    $(LDLIBS)

IA-repetition-low: main-graphique.o
	$(CC) -o IA-repetition-low  main-repetition.o   $(LDLIBS)

include dep


.PHONY: clean
clean:
	rm -f dep *.o *~ 
	if [ -d "bin" ];then rm -rf bin/; fi
dep:
	$(CC) $(CPPFLAGS) -MM *.c > dep


