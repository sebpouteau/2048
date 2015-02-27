CC = gcc
CFLAGS = -std=c99 -g -Wall 
LDLIBS = -lm -lSDL -lSDL_image -lSDL_ttf
CPPFLAGS= 

all:jeu main-sdl-test
 
jeu: main-sdl.o ../src/gridSDL.o ../src/grid.o 
	$(CC) -o jeu main-sdl.o ../src/gridSDL.o ../src/grid.o  $(LDLIBS)
main-sdl-test: main-sdl-test.o ../src/gridSDL.o ../src/grid.o
	$(CC) -o main-sdl-test main-sdl-test.o ../src/gridSDL.o ../src/grid.o  $(LDLIBS)


include dep

.PHONY: clean
clean:
	rm -f dep *.o *~ jeu main-sdl-test 

dep:
	$(CC) $(CPPFLAGS) -MM *.c >dep

run_sdl:
	./jeu

