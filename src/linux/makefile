
CONF = MidiShare.conf

midishare: common
	make -C kernel

midishare-alsa: common
	make -C kernel -f make-alsa

common:
	if [ ! -f ../common/makefile ]; then cp makefile.common ../common/makefile; fi
	if [ ! -x checksu ]; then chmod +x checksu; fi
	make -C ../common
	make -C library


install:
	./checksu
	make -i -C library install
	if [ ! -c /dev/MidiShare ]; then mknod /dev/MidiShare c 151 0; fi
	if [ ! -f /etc/$(CONF) ]; then cp $(CONF) /etc/$(CONF); fi
	insmod MidiShare

tools :
	make -i -C tools install 
	make -C tests all

uninstall:
	./checksu
	rmmod MidiShare
	
clean:
	make -i -C ../common clean 
	make -i -C kernel clean 
	make -i -C library clean 
	make -i -C tools clean 
	rm ../common/makefile
