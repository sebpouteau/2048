LIBS=  grid sdl strategy test-fonction ncurses 

all: install
	for i in $(LIBS);do\
		$(MAKE) -C $$i all;\
	done

install:
	for i in $(LIBS);do\
		$(MAKE) -C $$i install;\
	done

check: all
	$(MAKE) -C test-fonction check


clean:
	for i in $(LIBS);do\
		$(MAKE) -C $$i clean;\
	done





