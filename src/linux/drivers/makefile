
DIRS = raw

all:
	make -C ../../lib
	for a in $(DIRS); do make -C $$a; done
	
clean:
	for a in $(DIRS); do make -i -C $$a clean; done

install:
	for a in $(DIRS); do make -C $$a install; done

uninstall:
	for a in $(DIRS); do make -i -C $$a uninstall; done
