
TARGET = libMidiShare.so.1.0
FLAGS := -O0 -Wall -g -D_REENTRANT -D__Linux__ -D__Pentium__
CFLAGS := $(FLAGS) -fPIC -I../../common/Headers
LIB = /usr/lib/libMidiShare.so
CC = gcc

OBJ = libMidiShare.o ../libCommon.o
SRC = libMidiShare.c

$(TARGET) :  $(OBJ)
	$(CC) $(OBJ) -g -lpthread -shared -fPIC -W1,soname,libMidiShare.so.1 -o $(TARGET)

install: $(LIB).1.0 
	if [ ! -f $(LIB) ]; then ln -s $(LIB).1 $(LIB); fi
	if [ ! -f $(LIB).1 ]; then ln -s $(LIB).1.0 $(LIB).1; fi

uninstall: 
	if [ -f $(LIB) ]; then rm $(LIB); fi
	if [ -f $(LIB).1 ]; then rm $(LIB).1; fi
	if [ -f $(LIB).1.0 ]; then rm $(LIB).1.0; fi

$(LIB).1.0 : $(TARGET)
	cp $(TARGET)   $(LIB).1.0

dep:
	makedepend -- $(CFLAGS) -- -Y -m $(SRC)

clean : 
	rm libMidiShare.o $(TARGET)

# DO NOT DELETE

libMidiShare.o: libMidiShare.h ../kernel/msIOCtl.h
libMidiShare.o: ../../common/Headers/msDefs.h ../../common/Headers/msTypes.h
libMidiShare.o: msAppls.h ../../common/Headers/msMemory.h
libMidiShare.o: ../../common/Headers/lflifo.h ../../common/Headers/msSync.h
libMidiShare.o: ../../common/Headers/msEvents.h ../../common/Headers/msSeq.h
