CC = gcc
CFLAGS = -std=c99 -g -Wall -fPIC -I../../include
LDLIBS = -lA2_Emery_Gouraud_Kirov_Pouteau_fast -lgrid  -lm -lncurses  -L ../../lib/ 
CPPFLAGS= 

PATH_INCLUDE=../../include
PATH_LIB= ../../lib
PATH_BIN= ../../bin

all: $(PATH_LIB)/libA2_Emery_Gouraud_Kirov_Pouteau_fast.so $(PATH_LIB)/libA2_Emery_Gouraud_Kirov_Pouteau_fast.a verif-libgrid  $(PATH_BIN)/IA-graphique-fast $(PATH_BIN)/IA-repetition-fast 




verif-libgrid:



$(PATH_BIN)/IA-graphique-fast: main-graphique.o $(PATH_LIB)/*
	$(CC) -o $(PATH_BIN)/IA-graphique-fast main-graphique.o   $(LDLIBS)

$(PATH_BIN)/IA-repetition-fast: main-repetition.o $(PATH_LIB)/*
	$(CC) -o $(PATH_BIN)/IA-repetition-fast main-repetition.o   $(LDLIBS)

$(PATH_BIN)/IA-graphique-low: main-graphique.o $(PATH_LIB)/*
	$(CC) -o $(PATH_BIN)/IA-graphique-low main-graphique.o    $(LDLIBS)

$(PATH_BIN)/IA-repetition-low: main-graphique.o $(PATH_LIB)/*
	$(CC) -o $(PATH_BIN)/IA-repetition-low  main-repetition.o   $(LDLIBS)


install:$(PATH_LIB)/libA2_Emery_Gouraud_Kirov_Pouteau_fast.a

$(PATH_LIB)/libA2_Emery_Gouraud_Kirov_Pouteau_fast.a:A2_Emery_Gouraud_Kirov_Pouteau_fast.o
	ar -cr $(PATH_LIB)/libA2_Emery_Gouraud_Kirov_Pouteau_fast.a A2_Emery_Gouraud_Kirov_Pouteau_fast.o

$(PATH_LIB)/libA2_Emery_Gouraud_Kirov_Pouteau_fast.so:A2_Emery_Gouraud_Kirov_Pouteau_fast.o
	gcc -shared -fPIC A2_Emery_Gouraud_Kirov_Pouteau_fast.o -o $(PATH_LIB)/libA2_Emery_Gouraud_Kirov_Pouteau_fast.so

include dep


.PHONY: clean
clean:
	rm -f dep *.o *~ 

dep:
	$(CC) $(CPPFLAGS) -MM *.c > dep


