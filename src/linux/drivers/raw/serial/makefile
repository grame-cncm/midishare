
INC   = -I../../../Include
UTILS = ../../../lib/msUtilities.o
LIB   = $(UTILS) -lMidiShare -lpthread
BIN   = /usr/local/bin

Target = msRawSerial

all : $(Target)

$(Target) : $(Target).c $(UTILS)
	gcc $(Target).c  -g $(INC) $(LIB) -o $(Target)

clean : 
	rm -f $(Target)

install :
	install -m 6755 -s $(Target) $(BIN)
	install msRawDev $(BIN)

uninstall :
	rm -f $(BIN)/$(Target) $(BIN)/msRawDev
