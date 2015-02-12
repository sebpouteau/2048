CC = gcc
CFLAGS =-std=c99 -g -Wall
CPPFLAGS = 
EXEC = test-chaine-1 test


all:test-grid test-temps-deplacement
 
test-grid: main.o grid.o
	${CC} ${LDFLAGS} -o test-grid main.o grid.o

test-temps-deplacement: main-test-temps.o grid.o
	${CC} ${LDFLAGS} -o test-temps-deplacement main-test-temps.o grid.o


include dep

.PHONY: clean
clean:
	rm -f dep *.o *~ test-grid test-temps-deplacement 
dep:
	$(CC) $(CPPFLAGS) -MM *.c >dep


