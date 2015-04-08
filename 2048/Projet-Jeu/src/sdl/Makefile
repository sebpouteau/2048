CC = gcc
CFLAGS = -std=c99 -g -Wall -I../../include
LDLIBS = -lgridSDL -lm -lSDL -lSDL_image -lSDL_ttf -lgrid -L ../../lib/
CPPFLAGS= 

PATH_INCLUDE=../../include
PATH_LIB=../../lib
PATH_BIN=../../bin

all: $(PATH_LIB)/libgridSDL.a $(PATH_BIN)/jeu_sdl
 
$(PATH_BIN)/jeu_sdl: main-sdl.o $(PATH_LIB)/*
	$(CC) -o $(PATH_BIN)/jeu_sdl main-sdl.o $(LDLIBS)

install: $(PATH_LIB)/libgridSDL.a 

$(PATH_LIB)/libgridSDL.a: gridSDL.o
	ar -cr $(PATH_LIB)/libgridSDL.a gridSDL.o

include dep

.PHONY: clean
clean:
	rm -f dep *.o *~ jeu main-sdl-test 

dep:
	$(CC) $(CPPFLAGS) -MM *.c >dep


