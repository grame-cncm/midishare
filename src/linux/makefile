
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
	make -C library -f install -i
	if [ ! -c /dev/MidiShare ]; then mknod /dev/MidiShare c 151 0; fi
	insmod MidiShareKernel
	make -C tools public -i
	make -C tests all

tools :
	make -C tools all
	make -C tests all

uninstall:
	./checksu
	rmmod MidiShareKernel
	
clean:
	make -C ../common clean -i
	make -C kernel clean -i
	make -C library clean -i
	make -C tools clean -i
	rm ../common/makefile
