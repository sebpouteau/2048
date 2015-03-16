LIBS=  test-fonction ncurses sdl 

all: install
	if [ ! -d "bin" ];then mkdir bin; fi
	for i in $(LIBS);do\
		$(MAKE) -C $$i all;\
	done
	mv ./ncurses/jeu_ncurses ./bin
	mv ./sdl/jeu_sdl ./bin

install:
	$(MAKE) -C src
	$(MAKE) -C sdl gridSDL.o
	if [ ! -d "lib" ];then mkdir lib; fi
	if [ ! -f "lib/libgrid.a" ];then ar -cr ./lib/libgrid.a ./src/grid.o; fi
	if [ ! -f "lib/libgridSDl.a" ];then ar -cr ./lib/libgridSDL.a ./sdl/gridSDL.o; fi



check: 
	$(MAKE) -C test-fonction check


clean:
	for i in $(LIBS);do\
		$(MAKE) -C $$i clean;\
	done
		$(MAKE) -C src clean;\
	rm *~
	if [ -d "lib" ];then rm -rf lib; fi
	if [ -d "bin" ];then rm -rf bin; fi
