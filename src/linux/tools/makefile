
INC = ../Include
ToolSet = MidiGetVersion MidiCountAppls MidiGetIndAppl MidiGetName MidiGetNamedAppl MidiConnect MidiIsConnected MidiGetTime
InstallDir = $(HOME)/bin
PublicDir  = /usr/local/bin
ManDir     = /usr/local/man/man1

all : $(ToolSet) 

MidiGetVersion : MidiGetVersion.c
	gcc MidiGetVersion.c  -g -I$(INC) -o MidiGetVersion -lMidiShare -lpthread

MidiCountAppls : MidiCountAppls.c
	gcc MidiCountAppls.c  -g -I$(INC) -o MidiCountAppls -lMidiShare -lpthread

MidiGetIndAppl : MidiGetIndAppl.c
	gcc MidiGetIndAppl.c  -g -I$(INC) -o MidiGetIndAppl -lMidiShare -lpthread

MidiGetName : MidiGetName.c
	gcc MidiGetName.c  -g -I$(INC) -o MidiGetName -lMidiShare -lpthread

MidiGetNamedAppl : MidiGetNamedAppl.c
	gcc MidiGetNamedAppl.c  -g -I$(INC) -o MidiGetNamedAppl -lMidiShare -lpthread

MidiConnect : MidiConnect.c
	gcc MidiConnect.c  -g -I$(INC) -o MidiConnect -lMidiShare -lpthread

MidiIsConnected : MidiIsConnected.c
	gcc MidiIsConnected.c  -g -I$(INC) -o MidiIsConnected -lMidiShare -lpthread

MidiGetTime: MidiGetTime.c
	gcc MidiGetTime.c  -g -I$(INC) -o MidiGetTime -lMidiShare -lpthread

install : $(ToolSet)
	strip $(ToolSet)
	mv $(ToolSet) $(InstallDir)

public : $(ToolSet)
	strip $(ToolSet)
	cp $(ToolSet) $(PublicDir)
	bzip2 -k -f man1/*.1
	mv -f man1/*.bz2 $(ManDir)

clean : 
	rm -f $(ToolSet)
