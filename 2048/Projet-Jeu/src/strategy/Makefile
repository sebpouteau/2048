CC = gcc
CFLAGS = -std=c99 -g -Wall -fPIC -I../../include
LDLIBS = -lA2_Emery_Gouraud_Kirov_Pouteau_fast -lA2_Emery_Gouraud_Kirov_Pouteau_low -lgrid  -lm -lncurses  -L ../../lib/ 
CPPFLAGS= 

PATH_INCLUDE=../../include
PATH_LIB= ../../lib
PATH_BIN= ../../bin

all:$(PATH_LIB)/libA2_Emery_Gouraud_Kirov_Pouteau_fast.so $(PATH_LIB)/libA2_Emery_Gouraud_Kirov_Pouteau_low.so  $(PATH_LIB)/libA2_Emery_Gouraud_Kirov_Pouteau_fast.a  $(PATH_LIB)/libA2_Emery_Gouraud_Kirov_Pouteau_low.a   $(PATH_BIN)/IA-graphique-fast $(PATH_BIN)/IA-repetition-fast $(PATH_BIN)/IA-graphique-low $(PATH_BIN)/IA-repetition-low 



$(PATH_BIN)/IA-graphique-fast: main-graphique-fast.o $(PATH_LIB)/*
	$(CC) -o $(PATH_BIN)/IA-graphique-fast main-graphique-fast.o   $(LDLIBS)

$(PATH_BIN)/IA-repetition-fast: main-repetition-fast.o $(PATH_LIB)/*
	$(CC) -o $(PATH_BIN)/IA-repetition-fast main-repetition-fast.o   $(LDLIBS)

$(PATH_BIN)/IA-graphique-low: main-graphique-low.o $(PATH_LIB)/*
	$(CC) -o $(PATH_BIN)/IA-graphique-low main-graphique-low.o    $(LDLIBS)

$(PATH_BIN)/IA-repetition-low: main-repetition-low.o $(PATH_LIB)/*
	$(CC) -o $(PATH_BIN)/IA-repetition-low  main-repetition-low.o   $(LDLIBS)


install:$(PATH_LIB)/libA2_Emery_Gouraud_Kirov_Pouteau_fast.a $(PATH_LIB)/libA2_Emery_Gouraud_Kirov_Pouteau_low.a

$(PATH_LIB)/libA2_Emery_Gouraud_Kirov_Pouteau_fast.a: A2_Emery_Gouraud_Kirov_Pouteau_fast.o
	ar -cr $(PATH_LIB)/libA2_Emery_Gouraud_Kirov_Pouteau_fast.a A2_Emery_Gouraud_Kirov_Pouteau_fast.o

$(PATH_LIB)/libA2_Emery_Gouraud_Kirov_Pouteau_fast.so: A2_Emery_Gouraud_Kirov_Pouteau_fast.o
	gcc -shared -fPIC A2_Emery_Gouraud_Kirov_Pouteau_fast.o -lm -o $(PATH_LIB)/../libA2_Emery_Gouraud_Kirov_Pouteau_fast.so

$(PATH_LIB)/libA2_Emery_Gouraud_Kirov_Pouteau_low.a: A2_Emery_Gouraud_Kirov_Pouteau_low.o
	ar -cr $(PATH_LIB)/libA2_Emery_Gouraud_Kirov_Pouteau_low.a A2_Emery_Gouraud_Kirov_Pouteau_low.o

$(PATH_LIB)/libA2_Emery_Gouraud_Kirov_Pouteau_low.so: A2_Emery_Gouraud_Kirov_Pouteau_low.o
	gcc -shared -fPIC A2_Emery_Gouraud_Kirov_Pouteau_low.o -lm -o $(PATH_LIB)/../libA2_Emery_Gouraud_Kirov_Pouteau_low.so

include dep


.PHONY: clean
clean:
	rm -f dep *.o *~ 

dep:
	$(CC) $(CPPFLAGS) -MM *.c > dep


