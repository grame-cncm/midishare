
INC = ../Include
LIB = -Wl,-L../library -lMidiShare

ToolSet = MidiGetVersion MidiCountAppls MidiGetIndAppl MidiGetName MidiGetNamedAppl MidiConnect MidiIsConnected MidiGetTime
InstallDir = $(HOME)/bin
PublicDir  = /usr/local/bin
ManDir     = /usr/local/man/man1

all : $(ToolSet) 

MidiGetVersion : MidiGetVersion.c
	gcc MidiGetVersion.c  -g -I$(INC) -o MidiGetVersion $(LIB) 

MidiCountAppls : MidiCountAppls.c
	gcc MidiCountAppls.c  -g -I$(INC) -o MidiCountAppls $(LIB) 

MidiGetIndAppl : MidiGetIndAppl.c
	gcc MidiGetIndAppl.c  -g -I$(INC) -o MidiGetIndAppl $(LIB) 

MidiGetName : MidiGetName.c
	gcc MidiGetName.c  -g -I$(INC) -o MidiGetName $(LIB) 

MidiGetNamedAppl : MidiGetNamedAppl.c
	gcc MidiGetNamedAppl.c  -g -I$(INC) -o MidiGetNamedAppl $(LIB) 

MidiConnect : MidiConnect.c
	gcc MidiConnect.c  -g -I$(INC) -o MidiConnect $(LIB) 

MidiIsConnected : MidiIsConnected.c
	gcc MidiIsConnected.c  -g -I$(INC) -o MidiIsConnected $(LIB) 

MidiGetTime: MidiGetTime.c
	gcc MidiGetTime.c  -g -I$(INC) -o MidiGetTime $(LIB) 


install : $(ToolSet)
	strip $(ToolSet)
	cp $(ToolSet) $(PublicDir)
	bzip2 -k -f man1/*.1
	install -d $(ManDir)
	mv -f man1/*.bz2 $(ManDir)

clean : 
	rm -f $(ToolSet)
