LIBS=  test-fonction ncurses sdl strategy

all: install
	if [ ! -d "bin" ];then mkdir bin; fi
	for i in $(LIBS);do\
		$(MAKE) -C $$i all;\
	done
	mv ./ncurses/jeu_ncurses ./bin
	mv ./sdl/jeu_sdl ./bin

install:
	if [ ! -d "include" ];then mkdir include; fi
	if [ ! -f "include/grid.h" ];then mv src/grid.h ./include; fi
	if [ ! -f "include/strategy.h" ];then mv strategy/strategy.h ./include; fi
	if [ ! -f "include/gridSDL.h" ];then mv sdl/gridSDL.h ./include; fi
	if [ ! -f "include/fonction-test.h" ];then mv test-fonction/fonction-test.h ./include; fi
	$(MAKE) -C src
	$(MAKE) -C sdl gridSDL.o
	if [ ! -d "lib" ];then mkdir lib; fi
	if [ ! -f "lib/libgrid.a" ];then ar -cr ./lib/libgrid.a ./src/grid.o; fi
	if [ ! -f "lib/libgridSDl.a" ];then ar -cr ./lib/libgridSDL.a ./sdl/gridSDL.o; fi



check: all
	$(MAKE) -C test-fonction check


clean:
	for i in $(LIBS);do\
		$(MAKE) -C $$i clean;\
	done
		$(MAKE) -C src clean;\
	if [ -d "lib" ];then rm -rf lib; fi
	if [ -d "bin" ];then rm -rf bin; fi
	if [ -f "include/grid.h" ];then mv include/grid.h ./src; fi
	if [ -f "include/strategy.h" ];then mv include/strategy.h ./strategy; fi
	if [ -f "include/gridSDL.h" ];then mv include/gridSDL.h ./sdl; fi
	if [ -f "include/fonction-test.h" ];then mv include/fonction-test.h ./test-fonction; fi
	if [ -d "include" ];then rm -rf include; fi	
