
TARGET = libMidiShare.so.1.0
FLAGS := -O0 -Wall -g 
CFLAGS := $(FLAGS) -fPIC
CC = gcc

OBJ = libMidiShare.o msMemory.o msEvents.o msSeq.o msMem.o
SRC = *.c 

$(TARGET) :  libMidiShare.o msMemory.o  msEvents.o msSeq.o msMem.o
	$(CC) $(OBJ) -g -lpthread -shared -fPIC -W1,soname,libMidiShare.so.1 -o $(TARGET)

dep:
	makedepend -- $(FLAGS) -- -Y -m $(SRC)

clean : 
	rm *.o $(TARGET)

# DO NOT DELETE

libMidiShare.o: libMidiShare.h MidiShareKernel.h msDefs.h msTypes.h msAppls.h
libMidiShare.o: msMemory.h lflifo.h msEvents.h msSeq.h
msEvents.o: msEvents.h msDefs.h msTypes.h lflifo.h
msMem.o: msMem.h msTypes.h
msMemory.o: msDefs.h msTypes.h msMemory.h lflifo.h msMem.h
msSeq.o: msSeq.h msDefs.h msTypes.h lflifo.h msEvents.h
