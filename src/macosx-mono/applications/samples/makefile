INC=../Headers
OPTIONS= -D__Types__ -D__Macintosh__
CFLAGS := -O3 $(OPTIONS) 
LIB = -framework MidiShare
CC    = cc

all : msTutorial msSendNote msArpege msSequencer msSendSeq msDisplay msMidiPlayer msEcho msClock 


msTutorial : msTutorial.c  
	$(CC) $(CFLAGS) $(LIB) msTutorial.c  -o msTutorial 

msSendNote : msSendNote.c    
	$(CC) $(CFLAGS) $(LIB) msSendNote.c   -o msSendNote  

msArpege : msArpege.c    
	$(CC) $(CFLAGS) $(LIB) msArpege.c   -o msArpege  

msSequencer : msSequencer.c   
	$(CC) $(CFLAGS) $(LIB) msSequencer.c   -o msSequencer  

msSendSeq : msSendSeq.c   
	$(CC) $(CFLAGS) $(LIB) msSendSeq.c   -o msSendSeq  

msDisplay : msDisplay.c  
	$(CC) $(CFLAGS) $(LIB) msDisplay.c   -o msDisplay  

msMidiPlayer : msMidiPlayer.c  midifile.c   
	$(CC) $(CFLAGS) $(LIB) msMidiPlayer.c  midifile.c  -o msMidiPlayer  

msEcho : msEcho.c        
	$(CC) $(CFLAGS) $(LIB)  msEcho.c  -o msEcho  

msClock : msClock.c      
	$(CC) $(CFLAGS) $(LIB)   msClock.c  -o msClock  


clean:
	rm msTutorial msSendNote msArpege msSequencer msSendSeq msDisplay msMidiPlayer msEcho msClock 
