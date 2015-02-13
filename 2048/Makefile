CC = gcc
CFLAGS =-std=c99 -g -Wall 
LDLIBS = -lncurses -lm 
CPPFLAGS= 

all:test-grid test-temps-deplacement jeu
 
jeu: main-jeu.o grid.o
	$(CC) -o jeu main-jeu.o grid.o  $(LDLIBS)

test-grid: main.o grid.o
	$(CC) $(LDFLAGS) -o test-grid main.o grid.o

test-temps-deplacement: main-test-temps.o grid.o
	$(CC) $(LDFLAGS) -o test-temps-deplacement main-test-temps.o grid.o

 

include dep

.PHONY: clean
clean:
	rm -f dep *.o *~ test-grid test-temps-deplacement jeu 
dep:
	$(CC) $(CPPFLAGS) -MM *.c >dep


