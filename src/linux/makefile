
CONF = MidiShare.conf
MODF = /lib/modules/`uname -r`/misc
RC   = /etc/rc.d
INIT = $(RC)/init.d

midishare: common
	make -C kernel

common:
	make -C ../common
	make -C ../common/Memory
	make -C library

devinstall: libinstall
	insmod MidiShare.o

libinstall:
	make -i -C library install
	if [ ! -c /dev/MidiShare ]; then mknod /dev/MidiShare c 151 0; fi
	cp $(CONF) /etc/$(CONF)

install: libinstall
	install MidiShare.o $(MODF)
	depmod -a
	install MidiShare $(RC)/init.d
	ln -s $(INIT)/MidiShare $(RC)/rc5.d/S99MidiShare
	ln -s $(INIT)/MidiShare $(RC)/rc3.d/S99MidiShare
	modprobe MidiShare

#Compile applications, tools and drivers
	
clients:
	make -f appls
	
#Install applications, tools and drivers

clientsinstall:
	make -f appls install

libuninstall:
	rm -f /dev/MidiShare
	make -C library uninstall 

uninstall: libuninstall
	rm -f /etc/$(CONF)
	rm -f $(MODF)/MidiShare.o
	rm -f $(INIT)/MidiShare $(RC)/rc[35].d/*MidiShare
	rmmod MidiShare
	
test:
	make -C tests all
	
driver:
	make -C drivers all

	
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
