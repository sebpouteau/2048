LIBS=  test-fonction ncurses 

all: install
	if [ ! -d "bin" ];then mkdir bin; fi
	for i in $(LIBS);do\
		$(MAKE) -C $$i all;\
	done
	mv ./ncurses/jeu_ncurses ./bin


install:
	$(MAKE) -C src;\
	$(MAKE) -C sdl;\
	if [ ! -d "lib" ];then mkdir lib; fi
	if [ ! -f "lib/libgrid.a" ];then ar -cr ./lib/libgrid.a ./src/grid.o; fi




check: 
	$(MAKE) -C test-fonction check


clean:
	for i in $(LIBS);do\
		$(MAKE) -C $$i clean;\
	done
		$(MAKE) -C src clean;\
	if [ -d "lib" ];then rm -rf lib; fi
	if [ -d "bin" ];then rm -rf bin; fi
