// ===========================================================================
// The Player Library is Copyright (c) Grame, Computer Music Research Laboratory 
// 1996-1999, and is distributed as Open Source software under the Artistic License;
// see the file "Artistic" that is included in the distribution for details.
//
// Grame : Computer Music Research Laboratory
// Web : http://www.grame.fr/Research
// E-mail : MidiShare@rd.grame.fr
// ===========================================================================


#ifndef __PLAYER__
#define __PLAYER__

/*------------------------------------------------------------------
  Player: a library to develop  multi-tracks synchronisable
			MidiShare sequencers

  version 1.00
  version 1.11  08/12/97  New InsertAllTrackPlayer and InsertTrackPlayer functions
  version 2.00  03/30/99  Complete rewrite of the library, new kExternalSync mode, 
						    new SetTempoPlayer function

  ------------------------------------------------------------------*/

#ifdef __Macintosh__
	#if GENERATINGCFM
		#include <midisharePPC.h>
	#else
		#include <midishare.h>
	#endif
#endif

#ifdef __Linux__
	#include "MidiShare.h"
#endif


/*--------------------------------------------------------------------------*/
/* Player state  */
/*--------------------------------------------------------------------------*/

enum playerstatus {kIdle = 0, kPause, kRecording, kPlaying, kWaiting };

/*--------------------------------------------------------------------------*/
/* Tracks state  */
/*--------------------------------------------------------------------------*/

#define kMaxTrack		256
#define kMuteOn  		1
#define kMuteOff  		0
#define kSoloOn  		1
#define kSoloOff  		0
enum trackparameter 	{ kMute = 0,  kSolo };

/*--------------------------------------------------------------------------*/
/* Recording management */
/*--------------------------------------------------------------------------*/

#define kNoTrack		-1
#define kEraseMode  	1
#define kMixMode 		0

/*--------------------------------------------------------------------------*/
/* Loop  management */
/*--------------------------------------------------------------------------*/

enum loop {kLoopOn = 0, kLoopOff};

/*--------------------------------------------------------------------------*/
/* Step Playing */
/*--------------------------------------------------------------------------*/

#define kStepPlay  		1
#define kStepMute  		0

/*--------------------------------------------------------------------------*/
/* Synchronisation */
/*--------------------------------------------------------------------------*/

enum rcvsynchro	{ kInternalSync = 0  , kClockSync, kSMPTESync ,kExternalSync };
enum sendsynchro{ kNoSyncOut = 0,  kClockSyncOut };

/*--------------------------------------------------------------------------*/
/* MIDIfile */
/*--------------------------------------------------------------------------*/

enum midifiletypes	{ midifile0 = 0, midifile1, midifile2};

#define TicksPerQuarterNote		0
#define Smpte24					24
#define Smpte25					25
#define Smpte29					29
#define Smpte30					30


/*--------------------------------------------------------------------------*/
/* Errors */
 /*--------------------------------------------------------------------------*/
 // for the player
 /*--------------------------------------------------------------------------*/

#define PLAYERnoErr 			-1		/* no error			            		*/
#define PLAYERerrAppl			-2		/* Unable to open MidiShare application	*/
#define PLAYERerrEvent  		-3		/* No more MidiShare Memory 			*/
#define PLAYERerrMemory			-4		/* No more Mac Memory 			      */
#define PLAYERerrSequencer		-5		/* Sequencer error			         */


 /*--------------------------------------------------------------------------*/
 // for MidiFile
 /*--------------------------------------------------------------------------*/

#define noErr			0		/* no error 					*/
#define ErrOpen			1		/* file open error 				*/
#define ErrRead			2		/* file read error				*/
#define ErrWrite		3		/* file write error				*/
#define ErrVol			4		/* Volume error 				*/
#define ErrGetInfo 		5		/* GetFInfo	error				*/
#define ErrSetInfo		6		/* SetFInfo error				*/
#define ErrMidiFileFormat	7	/* bad  MidiFile 	format		*/

/*--------------------------------------------------------------------------*/

typedef struct PlayerState  * PlayerStatePtr;
typedef struct PlayerState{
	long date;
	long tempo;
	short tsnum;
	short tsdenom;
	short tsclick;
	short tsquarter;
	short bar;
	short beat;
	short unit;
	short state;
	short syncin;
	short syncout;
}PlayerState;

/*--------------------------------------------------------------------------*/

typedef struct Pos  * PosPtr;
typedef struct Pos{
	short bar;
	short beat;
	short unit;
}Pos;

/*--------------------------------------------------------------------------*/

typedef struct MidiFileInfos  * MidiFileInfosPtr;
typedef struct MidiFileInfos
{
	long format;			/* file format								*/
	long timedef;			/* time representation						*/
	long clicks;			/* ticks per quarter/frame					*/
	long tracks;            /* tracks number             				*/

}MidiFileInfos;

/*--------------------------------------------------------------------------*/


#ifdef __cplusplus
extern "C" {
#endif


short  OpenPlayer (MidiName name);
void   ClosePlayer (short refnum);

// Transport control

void  StartPlayer (short refnum);
void  ContPlayer (short refnum);
void  StopPlayer (short refnum);
void  PausePlayer (short refnum) ;

// Record management

void  SetRecordModePlayer (short refnum, short state) ;
void  RecordPlayer 	(short refnum, short tracknum);
void  SetRecordFilterPlayer	(short refnum, MidiFilterPtr filter);


// Position management

void  SetPosBBUPlayer (short refnum, PosPtr pos);
void  SetPosMsPlayer (short refnum, long date_ms);

// Loop management

void   SetLoopPlayer (short refnum, short state);
long   SetLoopStartBBUPlayer (short refnum,  PosPtr pos);
long   SetLoopEndBBUPlayer (short refnum,  PosPtr pos);
long   SetLoopStartMsPlayer(short refnum,   long date_ms);
long   SetLoopEndMsPlayer (short refnum,   long date_ms);


// Synchronisation management

void  SetSynchroInPlayer (short refnum, short state);
void  SetSynchroOutPlayer (short refnum,short state);
void  SetSMPTEOffsetPlayer (short refnum, SmpteLocPtr smptepos) ;
void  SetTempoPlayer (short refnum, long tempo) ;


// State management

void   GetStatePlayer (short refnum, PlayerStatePtr playerstate) ;
void   GetEndScorePlayer (short refnum, PlayerStatePtr playerstate);


// Step playing 

void   ForwardStepPlayer (short refnum , short flag);
void   BackwardStepPlayer (short refnum , short flag);


// Tracks management

MidiSeqPtr   GetAllTrackPlayer (short refnum);
MidiSeqPtr   GetTrackPlayer (short refnum, short tracknum);

long   SetTrackPlayer (short refnum, short tracknum, MidiSeqPtr s);
long   SetAllTrackPlayer (short refnum, MidiSeqPtr s, long ticks_per_quarter);

void   SetParamPlayer (short refnum, short tracknum, short param , short value);
short  GetParamPlayer (short refnum, short tracknum, short  param);

long   InsertAllTrackPlayer(short refnum,MidiSeqPtr s);
long   InsertTrackPlayer(short refnum, short tracknum, MidiSeqPtr s);


// Midifile management

int   MidiFileSave( char * name, MidiSeqPtr seq, MidiFileInfosPtr infos);
int   MidiFileLoad( char * name, MidiSeqPtr seq, MidiFileInfosPtr infos);

#ifdef __cplusplus
}
#endif

#endif
