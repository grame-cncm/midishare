
DIRS = msconnect mscontrol msdisplay msecho msplayer mssync msclock

all:
	make -C lib
	for a in $(DIRS); do make -C $$a; done
	
clean:
	make -i -C lib clean
	for a in $(DIRS); do make -i -C $$a clean; done

install:
	for a in $(DIRS); do make -C $$a install; done
