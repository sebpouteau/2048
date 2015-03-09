LIBS=  test-fonction ncurses

all: install
	for i in $(LIBS);do\
		$(MAKE) -C $$i;\
	done
	mv ./ncurses/jeu ./bin

install:
	$(MAKE) -C src;\
	if [ ! -d "lib" ];then mkdir lib; fi
	if [ ! -f "lib/libgrid.a" ];then ar -cr ./lib/libgrid.a ./src/grid.o; fi
	if [ ! -d "bin" ];then mkdir bin; fi


check: all
	$(MAKE) -C test-fonction check


clean:
	for i in $(LIBS);do\
		$(MAKE) -C $$i clean;\
	done
	if [ -d "lib" ];then rm -rf lib; fi
	if [ -d "bin" ];then rm -rf bin; fi
