
CC = gcc
FLAGS := -O3 -Wall -g -D_REENTRANT -D__Pentium__
CFLAGS := $(FLAGS) -fPIC -I../../common/Headers -I../../common/atomic

LMS = libMidiShare
LIB = $(LMS).so
DEST = /usr/lib

OBJ = $(LMS).o ../libCommon.o
SRC = $(LMS).c

$(LIB).1.0 :  $(OBJ)
	$(CC) $(OBJ) -g -lpthread -shared -fPIC -W1,soname,$(LIB).1 -o $(LIB).1.0
	if [ ! -f $(LIB).1 ]; then ln -s $(LIB).1.0 $(LIB).1; fi
	if [ ! -f $(LIB) ]; then ln -s $(LIB).1 $(LIB); fi

install: $(DEST)/$(LIB).1.0 
	if [ ! -f $(DEST)/$(LIB) ]; then ln -s $(DEST)/$(LIB).1 $(DEST)/$(LIB); fi
	if [ ! -f $(DEST)/$(LIB).1 ]; then ln -s $(DEST)/$(LIB).1.0 $(DEST)/$(LIB).1; fi

uninstall: 
	if [ -f $(DEST)/$(LIB) ]; then rm $(DEST)/$(LIB); fi
	if [ -f $(DEST)/$(LIB).1 ]; then rm $(DEST)/$(LIB).1; fi
	if [ -f $(DEST)/$(LIB).1.0 ]; then rm $(DEST)/$(LIB).1.0; fi

$(DEST)/$(LIB).1.0 : $(LIB).1.0
	cp $(LIB).1.0   $(DEST)/$(LIB).1.0

dep:
	makedepend -- $(CFLAGS) -- -Y -m $(SRC)

clean : 
	if [ -f $(LMS).o ]; then rm $(LMS).o; fi
	if [ -f $(LIB) ]; then rm $(LIB); fi
	if [ -f $(LIB).1 ]; then rm $(LIB).1; fi
	if [ -f $(LIB).1.0 ]; then rm $(LIB).1.0;fi

# DO NOT DELETE

libMidiShare.o: libMidiShare.h ../kernel/msIOCtl.h
libMidiShare.o: ../../common/Headers/msDefs.h ../../common/Headers/msTypes.h
libMidiShare.o: msAppls.h ../../common/Headers/msMemory.h
libMidiShare.o: ../../common/atomic/lflifo.h ../../common/atomic/msAtomic.h
libMidiShare.o: ../../common/atomic/msAtomicIntel.h
libMidiShare.o: ../../common/Headers/msEvents.h ../../common/Headers/msSeq.h
libMidiShare.o: ../../common/Headers/msMem.h
