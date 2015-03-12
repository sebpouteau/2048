CC = gcc
CFLAGS = -std=c99 -g -Wall 
LDLIBS = -lgridSDL -lm -lSDL -lSDL_image -lSDL_ttf -lgrid -L ../lib/
CPPFLAGS= 

include dep

all:jeu_sdl
 
jeu_sdl: main-sdl.o 
	$(CC) -o jeu_sdl main-sdl.o $(LDLIBS)




.PHONY: clean
clean:
	rm -f dep *.o *~ jeu main-sdl-test 

dep:
	$(CC) $(CPPFLAGS) -MM *.c >dep


