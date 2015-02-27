CC = gcc
CFLAGS = -std=c99 -g -Wall
CPPFLAGS = 
EXEC =test-fonction


all: test-fonction 

test-fonction: main-test-fonct.o ../src/grid.o ../src/gridIA.o
	${CC} ${LDFLAGS} -o test-fonction main-test-fonct.o ../src/grid.o ../src/gridIA.o

include dep

.PHONY: clean
clean:
	rm -f dep  *.o test-fonction
dep:
	$(CC) $(CPPFLAGS) -MM *.c >dep

check:
	for i in $(EXEC); do\
		./$$i;\
	done

