CC = gcc
CFLAGS = -std=c99 -g -Wall 
LDLIBS = -lm -lSDL -lSDL_image
CPPFLAGS= 

all:jeu
 
jeu: main-sdl.o ../src/grid.o
	$(CC) -o jeu main-sdl.o ../src/grid.o  $(LDLIBS)

include dep

.PHONY: clean
clean:
	rm -f dep *.o *~ jeu

dep:
	$(CC) $(CPPFLAGS) -MM *.c >dep

run_sdl:
	./jeu

