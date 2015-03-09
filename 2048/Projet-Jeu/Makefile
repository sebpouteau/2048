LIBS=  test-fonction ncurses sdl

all: install 
	for i in $(LIBS);do\
		$(MAKE) -C $$i;\
	done

install: 
	$(MAKE) -C src;\
	if [ ! -d "lib" ];then mkdir lib; fi
	if [ ! -d "lib/libgrid.a" ];then ar -cr ./lib/libgrid.a ./src/grid.o; fi



check: all
	$(MAKE) -C test-fonction check


clean:
	for i in $(LIBS);do\
		$(MAKE) -C $$i clean;\
	done
	if [ -f "lib/libgrid.a" ];then rm -rf lib; fi
