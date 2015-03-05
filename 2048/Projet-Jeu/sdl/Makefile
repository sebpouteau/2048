CC = gcc
CFLAGS = -std=c99 -g -Wall 
LDLIBS = -lm -lSDL -lSDL_image -lSDL_ttf
CPPFLAGS= 

all:jeu
 
jeu: main-sdl.o ../src/gridSDL.o ../src/grid.o 
	$(CC) -o jeu main-sdl.o ../src/gridSDL.o ../src/grid.o  $(LDLIBS)


include dep

.PHONY: clean
clean:
	rm -f dep *.o *~ jeu main-sdl-test 

dep:
	$(CC) $(CPPFLAGS) -MM *.c >dep


