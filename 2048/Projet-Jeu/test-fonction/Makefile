
CC = gcc
CFLAGS = -std=c99 -g -Wall -I../include
LDLIBS =  -lgrid -lm  -L ../lib/
CPPFLAGS = 
EXEC =test-fonction


all: test-fonction 

test-fonction: main-fonction-test.o ./fonction-test.o
	${CC} ${LDFLAGS} -o test-fonction main-fonction-test.o ./fonction-test.o $(LDLIBS)

include dep




dep:
	$(CC) $(CPPFLAGS) -MM *.c >dep

check: all 
	valgrind ./test-fonction
.PHONY: clean 
clean:
	rm -rf dep *~ *.o test-fonction *.out
