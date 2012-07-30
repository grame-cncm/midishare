
system	:= $(shell uname -s)
ifeq ($(system), Darwin)
	OPTIONS = -Dmacintosh -D__Types__
	LIB = -F../cmake/Release -framework MidiShare
else
ifeq ($(system), MINGW32_NT-6.1)
  	LIB =  ../cmake/Release/MidiShare.lib
else
	LIB = -lMidiShare
endif
endif

INC = -I../include -I../lib
CFLAGS := -O3 $(OPTIONS) $(INC)
CC    = cc
TESTS = TFonctions TEvents TFilters TDrivers TOutEvents TLin
RES   = results

all : $(TESTS)

TLin : TLin.o ../lib/EventToMidiStream.o ../lib/MidiStreamToEvent.o
	$(CC) $(CFLAGS) $(LIB) TLin.o ../lib/EventToMidiStream.o ../lib/MidiStreamToEvent.o -o TLin

TDrivers : TDrivers.c TDrivers.h
	$(CC) $(CFLAGS) $(LIB) TDrivers.c -o TDrivers

TEvents : TEvents.c TEvents.h
	$(CC) $(CFLAGS) $(LIB) TEvents.c -o TEvents

TFilters : TFilters.c TFilters.h
	$(CC) $(CFLAGS) $(LIB) TFilters.c -o TFilters

TFonctions : TFonctions.c TFonctions.h
	$(CC) $(CFLAGS) $(LIB) TFonctions.c -o TFonctions

TOutEvents : TOutEvents.c TOutEvents.h
	$(CC) $(CFLAGS) $(LIB) TOutEvents.c -o TOutEvents

run :
	[ -d $(RES) ] || mkdir $(RES)
	./TFonctions > $(RES)/Tfonctions.txt
	./TEvents    > $(RES)/TEvents.txt
	./TFilters   > $(RES)/TFilters.txt
	./TDrivers   > $(RES)/TDrivers.txt

clean :
	rm $(TESTS)
