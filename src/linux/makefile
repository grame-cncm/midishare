# modified for conformance with LSB 3.1

CONF = MidiShare.conf
MODF = /lib/modules/`uname -r`/misc
INCF = /usr/local/include
UDEV = /etc/udev/rules.d
INIT = /etc/init.d

SERV = $(INIT)/MidiShare
#SERV = MidiShare

# make, make install of the full MidiShare distribution

midishare: kernel
	make -f appls 

kernel: common
	make  -C kernel

common:
	make -C ../common 
	make -C ../common/Memory
	make -C library 

install: libinstall clientsinstall
	[ -d $(MODF) ] || mkdir $(MODF)
	install -m 644 kernel/midishare.ko $(MODF)
	/sbin/depmod -a
	install -m 644 midishare-udev.rules $(UDEV)
	install MidiShare $(INIT)
	/usr/lib/lsb/install_initd $(SERV)
	./MidiShare start

uninstall: libuninstall clientsuninstall
	./MidiShare stop
	/usr/lib/lsb/remove_initd $(SERV)
	rm -f $(INIT)/MidiShare
	rm -f $(MODF)/midishare.ko
	rm -f $(UDEV)/midishare-udev.rules
	rm -f /etc/$(CONF)

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
	rm -f $(INCF)/MidiShare.h
	make -C library uninstall 

libinstall:
	make -i -C library install
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

