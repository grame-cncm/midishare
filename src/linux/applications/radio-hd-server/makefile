#
#  Copyright © Grame 2001
#
#  Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
#  grame@grame.fr
#


INC = -I ../../Include
UTILS = ../../lib/msUtilities.o
CXXFLAGS = -g $(INC)
CFLAGS   = $(CXXFLAGS)

LIB = $(UTILS) -Wl,-L../../library -lMidiShare
BIN=/usr/local/bin
OBJ1 = msWANConnect.o TInetAddress.o msAnything2Midi.o
OBJ2 = msWANConnect.o TInetAddress.o msControlSignal.o
SRC = msWANConnect.c TInetAddress.cpp msAnything2Midi.c msControlSignal.c
CC = gcc

Target = RadioHDServer
Control = msControlSignal

all : $(Target) $(Control)

$(Target) : $(OBJ1)
	$(CC) $(OBJ1) $(LIB) -o $(Target)

$(Control) : $(OBJ2)
	$(CC) $(OBJ2) $(LIB) -o $(Control)

clean : 
	rm -f $(Target)  $(Control) *.o

install :
	install -m 755 -s $(Target) $(BIN)
	install -m 755 -s $(Control) $(BIN)

uninstall :
	rm -f $(BIN)/$(Target)
	rm -f $(BIN)/$(Control)

dep:
	makedepend -- $(CXXFLAGS) -- -Y -m $(SRC)
	
# DO NOT DELETE

TInetAddress.o: TInetAddress.h InetTypes.h ../../Include/MidiShare.h
msAnything2Midi.o: ../../Include/MidiShare.h
msAnything2Midi.o: ../../Include/MidiStreamToEvent.h server.h
