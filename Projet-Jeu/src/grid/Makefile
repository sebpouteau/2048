CC = gcc
CFLAGS = -std=c99 -g -ftest-coverage -fprofile-arcs -Wall -I../../include
LDLIBS = -lm -lgcov -L ../../lib/
CPPFLAGS= 

PATH_INCLUDE=../../include
PATH_LIB=../../lib
PATH_BIN=../../bin

all: $(PATH_LIB)/libgrid.a 
 
install: $(PATH_LIB)/libgrid.a 

$(PATH_LIB)/libgrid.a: grid.o
	ar -cr $(PATH_LIB)/libgrid.a grid.o

include dep

.PHONY: clean
clean:
	rm -f dep *.o *~ *.g* 

dep:
	$(CC) $(CPPFLAGS) -MM *.c >dep


