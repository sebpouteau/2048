CC = gcc
CFLAGS = -std=c99 -g -Wall 
LDLIBS = -lm -lSDL -lSDL_image -lSDL_ttf
CPPFLAGS= 

all:jeu test-main-sdl
 
jeu: main-sdl.o ../src/grid.o 
	$(CC) -o jeu main-sdl.o ../src/grid.o  $(LDLIBS)
test-main-sdl: test-main-sdl.o ../src/grid.o
	$(CC) -o test-main-sdl test-main-sdl.o ../src/grid.o  $(LDLIBS)

include dep

.PHONY: clean
clean:
	rm -f dep *.o *~ jeu test-main-sdl

dep:
	$(CC) $(CPPFLAGS) -MM *.c >dep

run_sdl:
	./jeu

