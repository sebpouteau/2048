CC = gcc
CFLAGS = -std=c99 -g -Wall 
LDLIBS = -lgridSDL -lm -lSDL -lSDL_image -lSDL_ttf -lgrid -L ../lib/
CPPFLAGS= 



all:jeu_sdl
 
jeu_sdl: main-sdl.o 
	$(CC) -o jeu_sdl main-sdl.o $(LDLIBS)

include dep


gridSDL.o:gridSDL.c
	$(CC) $(CFLAGS) -c gridSDL.c

.PHONY: clean
clean:
	rm -f dep *.o *~ jeu main-sdl-test 

dep:
	$(CC) $(CPPFLAGS) -MM *.c >dep


