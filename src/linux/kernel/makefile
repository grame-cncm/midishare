
TARGET = ../MidiShareKernel.o
OPTIONS=-DMODVERSIONS -D__Linux__ -D__Pentium__ -DCONFIG_KERNELD -DMODULE -D__KERNEL__ -DLINUX
INC = ../../common/Headers
CFLAGS := -O6 -Wall $(OPTIONS)  -I$(INC)
CC = gcc
DRIVER = nodriver

SRC = *.c $(DRIVER)/*.c

OBJ = msLoader.o MidiShareLinux.o MidiShare.o $(DRIVER)/msDriver.o ../msCommon.o


$(TARGET) : $(OBJ)
	ld -m elf_i386 -r -o $(TARGET) $(OBJ)
	
dep:
	makedepend -- $(CFLAGS) -- -Y -m $(SRC)

MidiShare.o : MidiShare.c
	$(CC) $(CFLAGS) -D__NO_VERSION__ -c MidiShare.c
	
clean:
	rm *.o $(DRIVER)/*.o $(TARGET)


# DO NOT DELETE

MidiShare.o: ../../common/Headers/msKernel.h ../../common/Headers/lffifo.h
MidiShare.o: ../../common/Headers/msSync.h ../../common/Headers/msTypes.h
MidiShare.o: ../../common/Headers/lflifo.h ../../common/Headers/msDefs.h
MidiShare.o: ../../common/Headers/msAppls.h ../../common/Headers/msMemory.h
MidiShare.o: ../../common/Headers/msSorter.h ../../common/Headers/msDriver.h
MidiShare.o: ../../common/Headers/msInit.h msLoader.h
MidiShare.o: ../../common/Headers/msAppFun.h ../../common/Headers/msTasks.h
MidiShare.o: ../../common/Headers/msXmtRcv.h ../../common/Headers/msEvents.h
MidiShare.o: ../../common/Headers/msConnx.h ../../common/Headers/msFilter.h
MidiShareLinux.o: msLoader.h ../../common/Headers/msDefs.h
MidiShareLinux.o: ../../common/Headers/msTypes.h
MidiShareLinux.o: ../../common/Headers/msKernel.h
MidiShareLinux.o: ../../common/Headers/lffifo.h ../../common/Headers/msSync.h
MidiShareLinux.o: ../../common/Headers/lflifo.h
MidiShareLinux.o: ../../common/Headers/msAppls.h
MidiShareLinux.o: ../../common/Headers/msMemory.h
MidiShareLinux.o: ../../common/Headers/msSorter.h
MidiShareLinux.o: ../../common/Headers/msDriver.h
MidiShareLinux.o: ../../common/Headers/msExtern.h
MidiShareLinux.o: ../../common/Headers/msTasks.h
MidiShareLinux.o: ../../common/Headers/msEvents.h
msLoader.o: ../../common/Headers/msAppFun.h ../../common/Headers/msKernel.h
msLoader.o: ../../common/Headers/lffifo.h ../../common/Headers/msSync.h
msLoader.o: ../../common/Headers/msTypes.h ../../common/Headers/lflifo.h
msLoader.o: ../../common/Headers/msDefs.h ../../common/Headers/msAppls.h
msLoader.o: ../../common/Headers/msMemory.h ../../common/Headers/msSorter.h
msLoader.o: ../../common/Headers/msDriver.h msLoader.h
nodriver/msDriver.o: ../../common/Headers/msDriver.h
nodriver/msDriver.o: ../../common/Headers/msDefs.h
nodriver/msDriver.o: ../../common/Headers/msTypes.h
