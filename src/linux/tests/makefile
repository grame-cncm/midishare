INC = ../Include
OPTIONS =-D __Linux__
LIB =  /usr/lib/libMidiShare.so.1.0 /usr/lib/libMidiShare.so.1 /usr/lib/libMidiShare.so
SRC = ../../tools

all : TestFunctions TestEvents

TestFunctions : $(SRC)/TFonctions.c $(INC)/MidiShare.h $(LIB)
	gcc $(SRC)/TFonctions.c -I$(INC) -g  $(OPTIONS) -o TestFunctions -lMidiShare -lpthread


TestEvents : $(SRC)/TEvents.c $(INC)/MidiShare.h  $(LIB)
	gcc $(SRC)/TEvents.c -I$(INC) -g  $(OPTIONS) -o TestEvents -lMidiShare -lpthread
