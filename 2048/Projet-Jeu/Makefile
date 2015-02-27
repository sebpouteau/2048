LIBS= src test-fonction ncurses sdl

all:
	for i in $(LIBS);do\
		$(MAKE) -C $$i;\
	done

check: 
	$(MAKE) -C test-fonction check

clean:
	for i in $(LIBS);do\
		$(MAKE) -C $$i clean;\
	done


run_curses:
	$(MAKE) -C  ncurses run_ncurses


run_sdl:
	$(MAKE) -C sdl run_sdl
