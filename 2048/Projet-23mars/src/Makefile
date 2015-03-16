
CC = gcc
CFLAGS = -std=c99 -g -Wall 
LDLIBS = 
CPPFLAGS= 



include dep

.PHONY: clean
clean:
	rm -f dep *.o *~ 

dep:
	$(CC) $(CPPFLAGS) -MM *.c >dep


