INC=../Headers
OPTIONS=-D __Linux__


all : msTutorial msSendNote msArpege msSequencer msSendSeq msDisplay msMidiPlayer msEcho msClock msPlayer

install:


msTutorial : msTutorial.c  /usr/lib/libMidiShare.so
	gcc msTutorial.c  -g -I$(INC) -o msTutorial -lMidiShare


msSendNote : msSendNote.c    /usr/lib/libMidiShare.so
	gcc msSendNote.c  -g -I$(INC) -o msSendNote -lMidiShare 

msReset : msReset.c    /usr/lib/libMidiShare.so
	gcc msReset.c  -g -I$(INC) -o msReset -lMidiShare 


msArpege : msArpege.c    /usr/lib/libMidiShare.so
	gcc msArpege.c  -g -I$(INC) -o msArpege -lMidiShare 


msSequencer : msSequencer.c    /usr/lib/libMidiShare.so
	gcc msSequencer.c  -g -I$(INC) -o msSequencer -lMidiShare 


msSendSeq : msSendSeq.c    /usr/lib/libMidiShare.so
	gcc msSendSeq.c  -g -I$(INC) -o msSendSeq -lMidiShare 


msDisplay : msDisplay.c    /usr/lib/libMidiShare.so
	gcc msDisplay.c  -g -I$(INC) -o msDisplay -lMidiShare 

msMidiPlayer : msMidiPlayer.c  midifile.c    /usr/lib/libMidiShare.so
	gcc msMidiPlayer.c  midifile.c -g $(OPTIONS) -I$(INC) -o msMidiPlayer -lMidiShare 

msEcho : msEcho.c        /usr/lib/libMidiShare.so
	gcc   msEcho.c -g $(OPTIONS) -I$(INC) -o msEcho -lMidiShare 

msClock : msClock.c      /usr/lib/libMidiShare.so
	gcc   msClock.c -g $(OPTIONS) -I$(INC) -o msClock -lMidiShare 


msPlayer : msPlayer.c Player.h /usr/lib/libPlayer.so
	gcc msPlayer.c  -g $(OPTIONS) -o msPlayer -lMidiShare -lPlayer 



clean:
	rm msTutorial msSendNote msArpege msSequencer msSendSeq msDisplay msMidiPlayer msEcho msClock msPlayer
