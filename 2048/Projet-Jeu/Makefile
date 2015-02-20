CC = gcc
CFLAGS = -std=c99 -g -Wall 
LDLIBS = -lncurses -lm -lSDL -lSDL_image
CPPFLAGS= 

all:test-grid test-temps-deplacement jeu main-test-fonct main-sdl
 
jeu: main-jeu.o grid.o
	$(CC) -o jeu main-jeu.o grid.o  $(LDLIBS)

test-grid: main-test.o grid.o
	$(CC) $(LDFLAGS) -o test-grid main-test.o grid.o

test-temps-deplacement: main-test-temps.o grid.o
	$(CC) $(LDFLAGS) -o test-temps-deplacement main-test-temps.o grid.o

main-test-fonct: main-test-fonct.o grid.o
	$(CC) $(LDFLAGS) -o main-test-fonct main-test-fonct.o grid.o

main-sdl: main-sdl.o grid.o 
	$(CC) $(LDFLAGS) -o main-sdl main-sdl.o grid.o $(LDLIBS)

include dep

.PHONY: clean
clean:
	rm -f dep *.o *~ test-grid test-temps-deplacement jeu main-test-fonct main-sdl

dep:
	$(CC) $(CPPFLAGS) -MM *.c >dep


