
INC = -I../../../Include
UTILS = ../../../lib/msUtilities.o
LIB = $(UTILS) -Wl,-L../../../library -lMidiShare -lpthread
BIN=/usr/local/bin

Target = msRawMidi

all : $(Target)

$(Target) : $(Target).c $(UTILS)
	gcc $(Target).c  -g $(INC) $(LIB) -o $(Target)

clean : 
	rm -f $(Target)

install :
	install -m 6755 -s $(Target) $(BIN)

uninstall :
	rm -f $(BIN)/$(Target)
