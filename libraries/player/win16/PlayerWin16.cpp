/* ===========================================================================
// The Player Library is Copyright (c) Grame, Computer Music Research Laboratory 
// 1996-1999, and is distributed as Open Source software under the Artistic License;
// see the file "Artistic" that is included in the distribution for details.
//
// Grame : Computer Music Research Laboratory
// Web : http://www.grame.fr/Research
// E-mail : MidiShare@rd.grame.fr
// ===========================================================================


// ===========================================================================
//	Player.cpp			    
// ===========================================================================
//
//	The C++/C interface to the Player API
/*/

#include  "TPlayer.h"
#include  "Tmtrack.h"
/*----------------------------------------------------------------------------*/
// Prototypes
/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
	extern "C" {
#endif

#ifdef __Macintosh__
	#pragma export on
#endif

// FOR THUNKING
// Player structure management

short  EXPORT OpenPlayer16 (MidiName name ) ;
void   EXPORT ClosePlayer16 (short refnum);

// Transport control

void  EXPORT StartPlayer16 (short refnum);
void  EXPORT ContPlayer16 (short refnum);
void  EXPORT StopPlayer16 (short refnum);
void  EXPORT PausePlayer16 (short refnum) ;

// Record management

void  EXPORT SetRecordModePlayer16 (short refnum, short state) ;
void  EXPORT RecordPlayer16 (short refnum, short tracknum);
void  EXPORT SetRecordFilterPlayer16(short refnum, FilterPtr filter);

// Position management

void  EXPORT SetPosBBUPlayer16 (short refnum, PosPtr pos);
void  EXPORT SetPosMsPlayer16 (short refnum, long date_ms);

// Loop management

void  EXPORT SetLoopPlayer16 (short refnum, short state);
long  EXPORT SetLoopStartBBUPlayer16 (short refnum,  PosPtr pos);
long  EXPORT SetLoopEndBBUPlayer16 (short refnum,  PosPtr pos);
long  EXPORT SetLoopStartMsPlayer16(short refnum,   long date_ms);
long  EXPORT SetLoopEndMsPlayer16 (short refnum,   long date_ms);

// Synchronisation management

void  EXPORT SetSynchroInPlayer16 (short refnum, short state);
void  EXPORT SetSynchroOutPlayer16 (short refnum,short state);
void  EXPORT SetSMPTEOffsetPlayer16 (short refnum,SmpteLocPtr smptepos) ;
void  EXPORT SetTempoPlayer16 (short refnum,long tempo);

// State management

void  EXPORT GetStatePlayer16 (short refnum, PlayerStatePtr playerstate) ;
void  EXPORT GetEndScorePlayer16 (short refnum, PlayerStatePtr playerstate);

// Step playing 

void  EXPORT ForwardStepPlayer16 (short refnum , short flag);
void  EXPORT BackwardStepPlayer16 (short refnum , short flag);

// Tracks management

MidiSeqPtr  EXPORT GetAllTrackPlayer16 (short refnum);
MidiSeqPtr  EXPORT GetTrackPlayer16 (short refnum, short tracknum);

long  EXPORT SetTrackPlayer16 (short refnum, short tracknum, MidiSeqPtr s);
long  EXPORT SetAllTrackPlayer16 (short refnum, MidiSeqPtr s, long ticks_per_quarter);

void  EXPORT SetParamPlayer16 (short refnum, short tracknum, short param , short value);
short EXPORT GetParamPlayer16 (short refnum, short tracknum, short  param);

long EXPORT InsertAllTrackPlayer16(short refnum,MidiSeqPtr s);
long EXPORT InsertTrackPlayer16(short refnum, short tracknum, MidiSeqPtr s);

int  EXPORT MidiFileLoad16( char * name, MidiSeqPtr seq, MidiFileInfosPtr infos);
int  EXPORT MidiFileSave16( char * name, MidiSeqPtr seq, MidiFileInfosPtr infos);

#ifdef __Macintosh__
	#pragma export off
#endif

#ifdef __cplusplus
	}
#endif

/*--------------------------------------------------------------------------*/

short  EXPORT OpenPlayer16 (MidiName name )
{
	TPlayerPtr player = new TPlayer();
	short res = player->Open(name);
 	if (res < 0) delete player; 		// correspond a une erreur
	return res;
}

/*--------------------------------------------------------------------------*/

void   EXPORT ClosePlayer16 (short refnum)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	if (player){
		player->Close();
		delete(player);
	}
}

/*--------------------------------------------------------------------------*/
// Transport control
/*--------------------------------------------------------------------------*/

void  EXPORT StartPlayer16 (short refnum)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	if (player) player->Start();
}

/*--------------------------------------------------------------------------*/

void  EXPORT ContPlayer16 (short refnum)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	if (player) player->Cont();
}

/*--------------------------------------------------------------------------*/

void  EXPORT StopPlayer16 (short refnum)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	if (player) player->Stop();
}

/*--------------------------------------------------------------------------*/

void  EXPORT PausePlayer16 (short refnum)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	if (player) player->Pause();
}

/*--------------------------------------------------------------------------*/
// Record management
/*--------------------------------------------------------------------------*/

void  EXPORT SetRecordModePlayer16 (short refnum, short state)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	if (player) player->SetRecordMode(state);
}
/*--------------------------------------------------------------------------*/

void  EXPORT RecordPlayer16 (short refnum, short tracknum)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	if (player) player->Record(tracknum);
}

/*--------------------------------------------------------------------------*/

void  EXPORT SetRecordFilterPlayer16 (short refnum, FilterPtr filter)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	if (player) player->SetRecordFilter(filter);
}

/*--------------------------------------------------------------------------*/
// Position management
/*--------------------------------------------------------------------------*/

void  EXPORT SetPosBBUPlayer16 (short refnum, PosPtr pos)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	if (player) player->SetPosBBU(pos);
}

/*--------------------------------------------------------------------------*/

void  EXPORT SetPosMsPlayer16 (short refnum, long date_ms)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	if (player) player->SetPosMs(date_ms);
}

/*--------------------------------------------------------------------------*/
// Loop management
/*--------------------------------------------------------------------------*/

void  EXPORT SetLoopPlayer16 (short refnum, short state)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	if (player) player->SetLoop(state);
}

/*--------------------------------------------------------------------------*/

long  EXPORT SetLoopStartBBUPlayer16 (short refnum,  PosPtr pos)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	return (player) ? player->SetLoopStartBBU(pos) :kErrSequencer;
}

/*--------------------------------------------------------------------------*/

long  EXPORT SetLoopEndBBUPlayer16 (short refnum,  PosPtr pos)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	return (player)? player->SetLoopEndBBU(pos):kErrSequencer;
}

/*--------------------------------------------------------------------------*/

long  EXPORT SetLoopStartMsPlayer16(short refnum,  long date_ms)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	return (player) ? player->SetLoopStartMs(date_ms): kErrSequencer;
}

/*--------------------------------------------------------------------------*/

long  EXPORT SetLoopEndMsPlayer16 (short refnum,   long date_ms)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	return (player) ? player->SetLoopEndMs(date_ms) : kErrSequencer;
}

/*--------------------------------------------------------------------------*/
// Synchronisation management
/*--------------------------------------------------------------------------*/

void  EXPORT SetSynchroInPlayer16 (short refnum, short state)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	if (player) player->SetSynchroIn(state);
}

/*--------------------------------------------------------------------------*/

void  EXPORT SetSynchroOutPlayer16 (short refnum,short state)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	if (player) player->SetSynchroOut(state);
}

/*--------------------------------------------------------------------------*/

void  EXPORT SetTempoPlayer16 (short refnum,long tempo)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	if (player) player->SetTempo(tempo);
}

/*--------------------------------------------------------------------------*/

void  EXPORT SetSMPTEOffsetPlayer16 (short refnum, SmpteLocPtr smptepos)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	if (player) player->SetSMPTEOffset(smptepos);
}
/*--------------------------------------------------------------------------*)
// State management
/*--------------------------------------------------------------------------*/

void  EXPORT GetStatePlayer16 (short refnum, PlayerStatePtr playerstate)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	if (player) player->GetState(playerstate);
}

/*--------------------------------------------------------------------------*/

void  EXPORT GetEndScorePlayer16 (short refnum, PlayerStatePtr playerstate)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	if (player) player->GetEndScore(playerstate);
}

/*--------------------------------------------------------------------------*/
// Step playing 
/*--------------------------------------------------------------------------*/

void  EXPORT ForwardStepPlayer16 (short refnum , short flag)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	if (player) player->ForwardStep(flag);
}

/*--------------------------------------------------------------------------*/

void  EXPORT BackwardStepPlayer16 (short refnum , short flag)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	if (player) player->BackwardStep(flag);
}

/*--------------------------------------------------------------------------*/
// Tracks management
/*--------------------------------------------------------------------------*/

MidiSeqPtr  EXPORT GetAllTrackPlayer16 (short refnum)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	return (player) ? player->GetAllTrack() : 0;
}

/*--------------------------------------------------------------------------*/

MidiSeqPtr  EXPORT GetTrackPlayer16 (short refnum, short tracknum)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	return (player) ? player->GetTrack(tracknum) : 0;
}

/*--------------------------------------------------------------------------*/

long  EXPORT SetTrackPlayer16 (short refnum, short tracknum, MidiSeqPtr s)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	return (player) ? player->SetTrack(tracknum,s) :kErrSequencer;
}

/*--------------------------------------------------------------------------*/

long  EXPORT SetAllTrackPlayer16 (short refnum, MidiSeqPtr s, long ticks_per_quarter)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	return (player) ? player->SetAllTrack(s,ticks_per_quarter) :kErrSequencer;
}

/*--------------------------------------------------------------------------*/

void  EXPORT SetParamPlayer16 (short refnum, short tracknum, short param , short value)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	if (player) player->SetParam(tracknum,param,value);
}

/*--------------------------------------------------------------------------*/

short  EXPORT GetParamPlayer16 (short refnum, short tracknum, short  param)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	return (player) ? player->GetParam(tracknum,param) : kErrSequencer;
}

/*--------------------------------------------------------------------------*/

long  EXPORT InsertAllTrackPlayer16 (short refnum,  MidiSeqPtr  s)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	return (player) ? player->InsertAllTrack(s) : kErrSequencer;
}

/*--------------------------------------------------------------------------*/

long  EXPORT InsertTrackPlayer16 (short refnum, short tracknum, MidiSeqPtr  s)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	return (player) ? player->InsertTrack(tracknum,s) : kErrSequencer;
}

/*--------------------------------------------------------------------------*/

int  EXPORT MidiFileLoad16( char * name, MidiSeqPtr seq, MidiFileInfosPtr infos)
{
	return MidiFileLoad(name,seq,infos);
}

/*--------------------------------------------------------------------------*/

int  EXPORT MidiFileSave16( char * name, MidiSeqPtr seq, MidiFileInfosPtr infos)
{
	return MidiFileSave(name,seq,infos);
}
