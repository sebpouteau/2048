LIBS= src

all:
	if [ ! -d "bin" ];then mkdir bin; fi
	if [ ! -d "lib" ];then mkdir lib; fi
	$(MAKE) -C $(LIBS) all	
	



check: all
	$(MAKE) -C $(LIBS) check


clean:
	$(MAKE) -C $(LIBS) clean;\
	rm -rf *.db *.g*
	if [ -d "lib" ];then rm -rf lib; fi
	if [ -d "bin" ];then rm -rf bin; fi
	if [ -d "html" ];then rm -rf html; fi
	if [ -d "latex" ];then rm -rf latex; fi
doc: 
	doxygen Doxyfile
