
CONF = MidiShare.conf
MODF = /lib/modules/`uname -r`/misc
INCF = /usr/local/include
RC   = /etc/rc.d
INIT = $(RC)/init.d

# make, make install of the full MidiShare distribution

midishare: kernel
	make -f appls

kernel: common
	make -C kernel

common:
	make -C ../common
	make -C ../common/Memory
	make -C library

install: libinstall clientsinstall
	[ -d $(MODF) ] || mkdir $(MODF)
	install MidiShare.o $(MODF)
	/sbin/depmod -a
	install MidiShare $(RC)/init.d
	ln -s $(INIT)/MidiShare $(RC)/rc5.d/S90MidiShare
	ln -s $(INIT)/MidiShare $(RC)/rc3.d/S90MidiShare
	/sbin/modprobe MidiShare 

uninstall: libuninstall clientsuninstall
	rm -f /etc/$(CONF)
	rm -f $(MODF)/MidiShare.o
	rm -f $(INIT)/MidiShare $(RC)/rc[35].d/*MidiShare
	/sbin/rmmod MidiShare
	
clean:
	make -i -C ../common/Memory clean 
	make -i -C ../common clean 
	make -i -C kernel clean 
	make -i -C library clean 
	make -f appls clean


#Compile applications, tools and drivers
	
clients:
	make -f appls
	
#Install applications, tools and drivers

clientsinstall:
	make -f appls install

clientsuninstall:
	make -f appls uninstall

libuninstall:
	rm -f /dev/MidiShare
	rm  /usr/local/include/MidiShare.h
	make -C library uninstall 

devinstall: libinstall
	/sbin/insmod MidiShare.o

libinstall:
	make -i -C library install
	if [ ! -c /dev/MidiShare ]; then mknod /dev/MidiShare c 151 0; fi
	cp $(CONF) /etc/$(CONF)
	[ -d $(INCF) ] || mkdir $(INCF)
	install  Include/MidiShare.h $(INCF)
	
test:
	make -C tests all
	
driver:
	make -C drivers all

	
dep:
	make -i -C ../common/Memory dep 
	make -i -C ../common dep 
	make -i -C kernel dep 
	make -i -C library dep 
