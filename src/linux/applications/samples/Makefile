INC		= ../Headers
OPTIONS	= -D __Linux__
LIB		= -Wl,-L../../library -lMidiShare


all : msTutorial msSendNote msArpege msSequencer msSendSeq msDisplay msMidiPlayer msEcho msClock msPlayer

install:


msTutorial : msTutorial.c  /usr/lib/libMidiShare.so
	gcc msTutorial.c  -g -I$(INC) -o msTutorial $(LIB)


msSendNote : msSendNote.c    /usr/lib/libMidiShare.so
	gcc msSendNote.c  -g -I$(INC) -o msSendNote $(LIB) 

msReset : msReset.c    /usr/lib/libMidiShare.so
	gcc msReset.c  -g -I$(INC) -o msReset $(LIB) 


msArpege : msArpege.c    /usr/lib/libMidiShare.so
	gcc msArpege.c  -g -I$(INC) -o msArpege $(LIB) 


msSequencer : msSequencer.c    /usr/lib/libMidiShare.so
	gcc msSequencer.c  -g -I$(INC) -o msSequencer $(LIB) 


msSendSeq : msSendSeq.c    /usr/lib/libMidiShare.so
	gcc msSendSeq.c  -g -I$(INC) -o msSendSeq $(LIB) 


msDisplay : msDisplay.c    /usr/lib/libMidiShare.so
	gcc msDisplay.c  -g -I$(INC) -o msDisplay $(LIB) 

msMidiPlayer : msMidiPlayer.c  midifile.c    /usr/lib/libMidiShare.so
	gcc msMidiPlayer.c  midifile.c -g $(OPTIONS) -I$(INC) -o msMidiPlayer $(LIB) 

msEcho : msEcho.c        /usr/lib/libMidiShare.so
	gcc   msEcho.c -g $(OPTIONS) -I$(INC) -o msEcho $(LIB) 

msClock : msClock.c      /usr/lib/libMidiShare.so
	gcc   msClock.c -g $(OPTIONS) -I$(INC) -o msClock $(LIB) 


msPlayer : msPlayer.c Player.h /usr/lib/libPlayer.so
	gcc msPlayer.c  -g $(OPTIONS) -o msPlayer $(LIB) -lPlayer 



clean:
	rm msTutorial msSendNote msArpege msSequencer msSendSeq msDisplay msMidiPlayer msEcho msClock msPlayer
