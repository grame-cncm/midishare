
CONF = MidiShare.conf

midishare: common
	make -C kernel

midishare-alsa: common
	make -C kernel -f make-alsa

common:
	make -C ../common
	make -C ../common/Memory
	make -C library

install:
	./checksu
	make -i -C library install
	if [ ! -c /dev/MidiShare ]; then mknod /dev/MidiShare c 151 0; fi
	if [ ! -f /etc/$(CONF) ]; then cp $(CONF) /etc/$(CONF); fi
	insmod MidiShare.o

appls:
	make -f appls

test:
	make -C tests all

uninstall:
	./checksu
	rmmod MidiShare || echo module not installed
	if [ -c /dev/MidiShare ]; then rm /dev/MidiShare; fi
	if [ -f /etc/$(CONF) ]; then rm /etc/$(CONF); fi
	make -C library uninstall 
dep:
	make -i -C ../common/Memory dep 
	make -i -C ../common dep 
	make -i -C kernel dep 
	make -i -C library dep 
	
clean:
	make -i -C ../common/Memory clean 
	make -i -C ../common clean 
	make -i -C kernel clean 
	make -i -C library clean 
