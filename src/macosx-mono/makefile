
OPTIONS = -D__Macintosh__ -D__POWERPC__
INC = -I../common/Headers -I../lib -IPortAudio/pa_common
CFLAGS := -O3 $(OPTIONS) $(INC)

COMP = -compatibility_version 1 -current_version 1

LIBO = ../lib/msPrefs.o ../lib/profport.o
OBJ  = MidiShare.o macosx.o dlopen.o $(LIBO)
COM  = msCommon.o
PA   = libPortAudio.o
TARGET = libMidiShare
VERS   = 1.8.6

all : static dylib

static : common $(PA) $(OBJ)
	ld -r $(OBJ) $(COM) $(PA) -o $(TARGET).o

dylib : common $(OBJ) $(COM) $(PA)
	libtool -arch_only ppc -noall_load $(COMP) -o $(TARGET).$(VERS).dylib -dynamic -framework CoreAudio -framework CoreServices $(OBJ) $(COM) $(PA) -lcc_dynamic -lSystem
	nmedit -s _MidiShare.exp $(TARGET).$(VERS).dylib

common :
	make -C ../common
 
$(PA) :
	make -C PortAudio

install :
	cp $(TARGET).$(VERS).dylib /usr/lib
	cp include/MidiShare.h /usr/include
	cd /usr/lib && ln -s $(TARGET).$(VERS).dylib $(TARGET).dylib

uninstall :
	rm -f /usr/lib/(TARGET).$(VERS).dylib
	rm -f /usr/lib/(TARGET).dylib
	rm /usr/include/MidiShare.h

clean :
	make -i -C ../common clean
	make -i -C PortAudio clean
	rm -f $(OBJ) $(TARGET).*
	
test :
	make -C ../tools
	
dep :
	make -i -C ../common dep 
