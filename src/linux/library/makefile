
CC = gcc
FLAGS := -O0 -Wall -g -D_REENTRANT -D__Linux__ -D__Pentium__
CFLAGS := $(FLAGS) -fPIC -I../../common/Headers

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

$(LMS).o: $(LMS).h ../kernel/msIOCtl.h
$(LMS).o: ../../common/Headers/msDefs.h ../../common/Headers/msTypes.h
$(LMS).o: msAppls.h ../../common/Headers/msMemory.h
$(LMS).o: ../../common/Headers/lflifo.h ../../common/Headers/msSync.h
$(LMS).o: ../../common/Headers/msEvents.h ../../common/Headers/msSeq.h
