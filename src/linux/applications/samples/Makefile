#INC	= ../Headers
INC	= ../../Include
OPTIONS	=
LIB	= -Wl,-L../../library -lMidiShare


all : msTutorial msSendNote msArpege msSequencer msSendSeq msDisplay msMidiPlayer msEcho msClock 

install:


msTutorial : msTutorial.c  
	gcc msTutorial.c  -g -I$(INC) -o msTutorial $(LIB)


msSendNote : msSendNote.c    
	gcc msSendNote.c  -g -I$(INC) -o msSendNote $(LIB) 

msReset : msReset.c    
	gcc msReset.c  -g -I$(INC) -o msReset $(LIB) 


msArpege : msArpege.c    
	gcc msArpege.c  -g -I$(INC) -o msArpege $(LIB) 


msSequencer : msSequencer.c    
	gcc msSequencer.c  -g -I$(INC) -o msSequencer $(LIB) 


msSendSeq : msSendSeq.c    
	gcc msSendSeq.c  -g -I$(INC) -o msSendSeq $(LIB) 


msDisplay : msDisplay.c    
	gcc msDisplay.c  -g -I$(INC) -o msDisplay $(LIB) 

msMidiPlayer : msMidiPlayer.c  midifile.c    
	gcc msMidiPlayer.c  midifile.c -g $(OPTIONS) -I$(INC) -o msMidiPlayer $(LIB) 

msEcho : msEcho.c        
	gcc   msEcho.c -g $(OPTIONS) -I$(INC) -o msEcho $(LIB) 

msClock : msClock.c      
	gcc   msClock.c -g $(OPTIONS) -I$(INC) -o msClock $(LIB) 

clean:
	rm msTutorial msSendNote msArpege msSequencer msSendSeq msDisplay msMidiPlayer msEcho msClock 
