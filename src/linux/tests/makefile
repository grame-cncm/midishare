SRC = ../../tools
CFLAGS = -D __Linux__ -I../Include
CC  = gcc
APPLS = TestFunctions TestEvents

appls : $(APPLS)

TestFunctions : $(SRC)/TFonctions.c  $(SRC)/TFonctions.h
	$(CC) $(SRC)/TFonctions.c -g  $(CFLAGS) -o TestFunctions -lMidiShare -lpthread

TestEvents : $(SRC)/TEvents.c $(SRC)/TEvents.h
	$(CC) $(SRC)/TEvents.c -I$(INC) -g  $(CFLAGS) -o TestEvents -lMidiShare -lpthread

fmodule :
	make -f functions

emodule :
	make -f events

modules :
	make -f events
	make -f functions
	
clean :
	rm $(APPLS)
