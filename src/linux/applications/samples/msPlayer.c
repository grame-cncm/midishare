/*====================== A MIDI SHARE TOOL  (© GRAME 99-01) =====================

NAME
      msPlayer -- a very simple MidiShare tool which use the Player library to play MifiFiles.

SYNOPSIS
      msPlayer [MidiFile] 

DESCRIPTION
      "msPlayer" open a Player session, loads the MidiFile and play it.

===============================================================================*/
 
#include "Player.h"

#define nil     0
#define true    1
#define false   0

MidiEvPtr dispTask;

//_______________________________________________________________________

 void   DisplayTask (long date, short ref, long a1, long a2, long a3)
 {
 	PlayerState state;
	GetStatePlayer(ref, &state);
	printf ("%02d:%02d:%02d:\n", state.bar, (int)state.beat, (int)state.unit);
	dispTask = MidiTask(DisplayTask, date+1000, ref, 0,0,0);
 }

//_______________________________________________________________________

static ChangeTempo(short ref, char c) 
{
	PlayerState state;
	GetStatePlayer(ref, &state);
	
	if (state.tempo > 50000) {
		switch(c) {
		 	case '-': state.tempo+= 30000; break;
		 	case '+': state.tempo-= 30000; break;
		}
		SetTempoPlayer(ref, state.tempo);
	}
}

//_______________________________________________________________________

static SetLoop(short ref) 
{
	PlayerState state;
	Pos pos;
	GetEndScorePlayer(ref, &state);
	
	pos.bar = state.bar;
	pos.beat = state.beat;
	pos.unit = state.unit;
	SetLoopEndBBUPlayer(ref, &pos);
	pos.bar = 1;
	pos.beat = 1;
	pos.unit = 1;
	SetLoopStartBBUPlayer(ref, &pos);
	SetLoopPlayer(ref, kLoopOn);
}

//_______________________________________________________________________

static Start(short ref) 
{
	StartPlayer(ref); 
	MidiForgetTask(&dispTask);
	
	DisplayTask(MidiGetTime(), ref,0,0,0);
}

//_______________________________________________________________________

static Stop(short ref) 
{
	StopPlayer(ref); 
	MidiForgetTask(&dispTask);
}

//_______________________________________________________________________

static Cont(short ref) 
{
	ContPlayer(ref); 
	MidiForgetTask(&dispTask);
	DisplayTask(MidiGetTime(), ref,0,0,0);
}

//_______________________________________________________________________

int main( int argc, char *argv[])
{
		short ref = -1;
		MidiSeqPtr seq = 0;
    		MidiFileInfos info;
		char* synchro;
		char c;
		int res;
		
		Pos pos;
		
		if (argc < 2) {
			printf("Usage : msPlayer  [MidiFile] \n");
			goto exit;
		}
		
        	ref = OpenPlayer("msPlayer");      		// Open a new Player 
		
		if (ref < 0) {
			printf("Can not allocate a new Player\n");
			goto exit;
		}
		
		seq = MidiNewSeq();                    		// Allocate a MidiShare sequence
		
		if (!seq) {
			printf("Can not allocate a new Sequence\n");
			goto exit;
		}
		res = MidiFileLoad(argv[1], seq, &info);	// Load the MifiFile in the MidiShare sequence
		
		if (res) {
			printf("Can not load the MidiFile, error : %d\n",res);
			MidiFreeSeq(seq);
			goto exit;
		}
		
		SetAllTrackPlayer(ref, seq, info.clicks);	// Set the sequence in the Player
		//SetSynchroInPlayer(ref, kExternalSync);	// Change the Player synchronisation mode
		//SetSynchroInPlayer(ref, kClockSync);
		
		MidiConnect(ref,0, true);
		MidiConnect(0,ref, true);			// Connect the Player to plysical midi in and out
			
		printf("\nUse p or P to start playing\n",res);
		printf("Use s or S to stop playing\n",res);
		printf("Use c or C to continue playing\n",res);
		printf("Use q or Q to quit\n",res);
		
		//printf("Use + to speed up the Tempo\n",res);
		//printf("Use - to slow down the Tempo\n\n",res);
		//SetLoop(ref);
		
		while ((c = getchar()) && c != 'q' && c != 'Q')  switch (c) {
               			case 'P': case 'p':Start(ref); break;
				case 'S': case 's':Stop(ref); break;
				case 'C': case 'c':Cont(ref);break;
				case '+': case '-':ChangeTempo(ref, c); break;
        	}
		
		exit:
		Stop(ref);
		ClosePlayer(ref);   // Close the Player
                                    
}

