
TARGET = libMidiShare.so.1.0
FLAGS := -O0 -Wall -g 
CFLAGS := $(FLAGS) -fPIC
LIB = /usr/lib/libMidiShare.so
CC = gcc

OBJ = libMidiShare.o msMemory.o msEvents.o msSeq.o msMem.o
SRC = *.c 

$(TARGET) :  libMidiShare.o msMemory.o  msEvents.o msSeq.o msMem.o
	$(CC) $(OBJ) -g -lpthread -shared -fPIC -W1,soname,libMidiShare.so.1 -o $(TARGET)

install: $(LIB).1.0 
	if [ ! -f $(LIB) ]; then ln -s $(LIB).1 $(LIB); fi
	if [ ! -f $(LIB).1 ]; then ln -s $(LIB).1.0 $(LIB).1; fi

$(LIB).1.0 : $(TARGET)
	cp $(TARGET)   $(LIB).1.0

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
