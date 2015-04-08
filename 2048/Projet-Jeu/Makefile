LIBS= src

all:
	if [ ! -d "bin" ];then mkdir bin; fi
	if [ ! -d "lib" ];then mkdir lib; fi
	$(MAKE) -C $(LIBS) all	
	



check: all
	$(MAKE) -C $(LIBS) check


clean:
	$(MAKE) -C $(LIBS) clean;\
	if [ -d "lib" ];then rm -rf lib; fi
	if [ -d "bin" ];then rm -rf bin; fi

