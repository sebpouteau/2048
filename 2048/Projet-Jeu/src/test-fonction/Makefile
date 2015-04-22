CC = gcc
CFLAGS = -std=c99 --coverage -g -Wall -I../../include
LDLIBS =  -lgrid -lgcov -lm  -L ../../lib/
CPPFLAGS = 
EXEC =test-fonction

PATH_LIB=../../lib
PATH_INCLUDE=../../include

all: test-fonction 

test-fonction: main-fonction-test.o ./fonction-test.o  $(PATH_LIB)/*
	${CC} ${CFLAGS} -o test-fonction main-fonction-test.o  ./fonction-test.o $(LDLIBS)

install:

include dep


dep:
	$(CC) $(CPPFLAGS) -MM *.c >dep

check: all 
	valgrind ./test-fonction
	gcov main-fonction-test.c
	gcov fonction-test.c
	gcov ../grid/grid.c

.PHONY: clean 
clean:
	rm -rf dep *~ *.o test-fonction *.out *.g*
