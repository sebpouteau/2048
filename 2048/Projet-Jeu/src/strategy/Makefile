CC = gcc
CFLAGS = -std=c99 -g -Wall -fPIC -I../../include
LDLIBS = -lA2_emery_gouraud_kirov_pouteau_fast -lA2_emery_gouraud_kirov_pouteau_low -lgridNcurses -lgrid -lgcov -lm -lncurses  -L ../../lib/ 
CPPFLAGS= 

PATH_INCLUDe=../../include
PATH_LIB= ../../lib
PATH_BIN= ../../bin

all: $(PATH_LIB)/libA2_emery_gouraud_kirov_pouteau_fast.a  $(PATH_LIB)/libA2_emery_gouraud_kirov_pouteau_low.a   $(PATH_BIN)/IA-graphique-fast $(PATH_BIN)/IA-repetition-fast $(PATH_BIN)/IA-graphique-low $(PATH_BIN)/IA-repetition-low 



$(PATH_BIN)/IA-graphique-fast: main-graphique-fast.o $(PATH_LIB)/*
	$(CC) -o $(PATH_BIN)/IA-graphique-fast main-graphique-fast.o   $(LDLIBS)

$(PATH_BIN)/IA-repetition-fast: main-repetition-fast.o $(PATH_LIB)/*
	$(CC) -o $(PATH_BIN)/IA-repetition-fast main-repetition-fast.o   $(LDLIBS)

$(PATH_BIN)/IA-graphique-low: main-graphique-low.o $(PATH_LIB)/*
	$(CC) -o $(PATH_BIN)/IA-graphique-low main-graphique-low.o    $(LDLIBS)

$(PATH_BIN)/IA-repetition-low: main-repetition-low.o $(PATH_LIB)/*
	$(CC) -o $(PATH_BIN)/IA-repetition-low  main-repetition-low.o   $(LDLIBS)


install:$(PATH_LIB)/libA2_emery_gouraud_kirov_pouteau_fast.a $(PATH_LIB)/libA2_emery_gouraud_kirov_pouteau_low.a

$(PATH_LIB)/libA2_emery_gouraud_kirov_pouteau_fast.a: A2_emery_gouraud_kirov_pouteau_fast.o
	ar -cr $(PATH_LIB)/libA2_emery_gouraud_kirov_pouteau_fast.a A2_emery_gouraud_kirov_pouteau_fast.o

$(PATH_LIB)/libA2_emery_gouraud_kirov_pouteau_low.a: A2_emery_gouraud_kirov_pouteau_low.o
	ar -cr $(PATH_LIB)/libA2_emery_gouraud_kirov_pouteau_low.a A2_emery_gouraud_kirov_pouteau_low.o


include dep


.PHONY: clean
clean:
	rm -f dep *.o *~ *.g*

dep:
	$(CC) $(CPPFLAGS) -MM *.c > dep


