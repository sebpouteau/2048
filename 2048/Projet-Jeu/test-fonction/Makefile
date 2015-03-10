CC = gcc
CFLAGS = -std=c99 -g -Wall
LDLIBS =  -lgrid -lm  -L ../lib/
CPPFLAGS = 
EXEC =test-fonction


all: test-fonction 

test-fonction: main-fonction-test.o ./fonction-test.o
	${CC} ${LDFLAGS} -o test-fonction main-fonction-test.o ./fonction-test.o $(LDLIBS)

include dep

.PHONY: clean
clean:
	rm -f dep *~ *.o test-fonction
dep:
	$(CC) $(CPPFLAGS) -MM *.c >dep

check: 
	# for i in $(EXEC); do\
	# 	./$$i;\
	# done
	valgrind ./test-fonction
