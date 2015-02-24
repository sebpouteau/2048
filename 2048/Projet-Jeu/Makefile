LIBS= src test-fonction ncurses sdl
all:
	for i in $(LIBS);do\
		$(MAKE) -C $$i;\
	done

check: 

	$(MAKE) -C test-fonction

clean:
	for i in $(LIBS);do\
		$(MAKE) -C $$i clean;\
	done


run:
	$(MAKE) -C  ncurses run


run_sdl:
	$(MAKE) -C sdl run_sdl
