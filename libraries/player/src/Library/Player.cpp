/*

  Copyright � Grame 1996-2004

  This library is free software; you can redistribute it and modify it under 
  the terms of the GNU Library General Public License as published by the 
  Free Software Foundation version 2 of the License, or any later version.

  This library is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public License 
  for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

  Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
  research@grame.fr

*/

// ===========================================================================
//	Player.cpp			    
// ===========================================================================
//
//	The C++/C interface to the Player API

#include  "TPlayer.h"

/*----------------------------------------------------------------------------*/
// Prototypes
/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
	extern "C" {
#endif

#ifdef __Macintosh__
	#pragma export on
#endif

// Player structure management

short  EXPORT Version();

// Player structure management

short  EXPORT OpenPlayer(MidiName name);
void   EXPORT ClosePlayer(short refnum);

// Transport control

void  EXPORT StartPlayer(short refnum);
void  EXPORT ContPlayer(short refnum);
void  EXPORT StopPlayer(short refnum);
void  EXPORT PausePlayer(short refnum);

// Record management

void  EXPORT SetRecordModePlayer(short refnum, short state);
void  EXPORT RecordPlayer(short refnum, short tracknum);
void  EXPORT SetRecordFilterPlayer(short refnum, MidiFilterPtr filter);

// Position management

void  EXPORT SetPosBBUPlayer(short refnum, PosPtr pos);
void  EXPORT SetPosMsPlayer(short refnum, long date_ms);
void  EXPORT SetPosTicksPlayer(short refnum, long date_ticks);

// Loop management

void  EXPORT SetLoopPlayer(short refnum, short state);
long  EXPORT SetLoopStartBBUPlayer(short refnum, PosPtr pos);
long  EXPORT SetLoopEndBBUPlayer(short refnum, PosPtr pos);
long  EXPORT SetLoopStartMsPlayer(short refnum, long date_ms);
long  EXPORT SetLoopEndMsPlayer(short refnum, long date_ms);

// Synchronisation management

void  EXPORT SetSynchroInPlayer(short refnum, short state);
void  EXPORT SetSynchroOutPlayer(short refnum, short state);
void  EXPORT SetSMPTEOffsetPlayer(short refnum, SmpteLocPtr smptepos);
void  EXPORT SetTempoPlayer(short refnum, long tempo);

// State management

void  EXPORT GetStatePlayer(short refnum, PlayerStatePtr playerstate);
void  EXPORT GetEndScorePlayer(short refnum, PlayerStatePtr playerstate);

// Step playing 

void  EXPORT ForwardStepPlayer(short refnum, short flag);
void  EXPORT BackwardStepPlayer(short refnum, short flag);

// Tracks management

MidiSeqPtr  EXPORT GetAllTrackPlayer(short refnum);
MidiSeqPtr  EXPORT GetTrackPlayer(short refnum, short tracknum);

long  EXPORT SetTrackPlayer(short refnum, short tracknum, MidiSeqPtr s);
long  EXPORT SetAllTrackPlayer(short refnum, MidiSeqPtr s, long ticks_per_quarter);

void  EXPORT SetParamPlayer(short refnum, short tracknum, short param , short value);
short EXPORT GetParamPlayer(short refnum, short tracknum, short  param);

long EXPORT InsertAllTrackPlayer(short refnum, MidiSeqPtr s);
long EXPORT InsertTrackPlayer(short refnum, short tracknum, MidiSeqPtr s);

void EXPORT ApplyAllTrackPlayer(short refnum, ApplyScorePtr fun, void* data);
void EXPORT ApplyTrackPlayer(short refnum, short tracknum, ApplyScorePtr fun, void* data);


#ifdef __Macintosh__
	#pragma export off
#endif

#ifdef __cplusplus
	}
#endif


/*--------------------------------------------------------------------------*/
// Can be called at interrupt level
/*--------------------------------------------------------------------------*/

short  EXPORT Version()  {return kVersion;}

// Player structure management

/*--------------------------------------------------------------------------*/

short  EXPORT OpenPlayer(MidiName name)
{
	TPlayer::Init();
  	TPlayerPtr player = new TPlayer();
	short res = player->Open(name);
 	if (res < 0) delete player; 		// when an error occurs
	return res;
}

/*--------------------------------------------------------------------------*/

void   EXPORT ClosePlayer(short refnum)
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

void  EXPORT StartPlayer(short refnum)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	if (player) player->Start();
}

/*--------------------------------------------------------------------------*/

void  EXPORT ContPlayer(short refnum)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	if (player) player->Cont();
}

/*--------------------------------------------------------------------------*/

void  EXPORT StopPlayer(short refnum)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	if (player) player->Stop();
}

/*--------------------------------------------------------------------------*/

void  EXPORT PausePlayer(short refnum)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	if (player) player->Pause();
}

/*--------------------------------------------------------------------------*/
// Record management
/*--------------------------------------------------------------------------*/

void  EXPORT SetRecordModePlayer(short refnum, short state)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	if (player) player->SetRecordMode(state);
}
/*--------------------------------------------------------------------------*/

void  EXPORT RecordPlayer (short refnum, short tracknum)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	if (player) player->Record(tracknum);
}

/*--------------------------------------------------------------------------*/

void  EXPORT SetRecordFilterPlayer (short refnum, MidiFilterPtr filter)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	if (player) player->SetRecordFilter(filter);
}

/*--------------------------------------------------------------------------*/
// Position management
/*--------------------------------------------------------------------------*/

void  EXPORT SetPosBBUPlayer(short refnum, PosPtr pos)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	if (player) player->SetPosBBU(pos);
}

/*--------------------------------------------------------------------------*/

void  EXPORT SetPosMsPlayer(short refnum, long date_ms)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	if (player) player->SetPosMs(date_ms);
}

/*--------------------------------------------------------------------------*/

void  EXPORT SetPosTicksPlayer(short refnum, long date_ticks)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	if (player) player->SetPosTicks(date_ticks);
}

/*--------------------------------------------------------------------------*/
// Loop management
/*--------------------------------------------------------------------------*/

void  EXPORT SetLoopPlayer(short refnum, short state)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	if (player) player->SetLoop((Boolean)state);
}

/*--------------------------------------------------------------------------*/

long  EXPORT SetLoopStartBBUPlayer (short refnum, PosPtr pos)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	return (player) ? player->SetLoopStartBBU(pos) : kErrSequencer;
}

/*--------------------------------------------------------------------------*/

long  EXPORT SetLoopEndBBUPlayer(short refnum, PosPtr pos)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	return (player)? player->SetLoopEndBBU(pos):kErrSequencer;
}

/*--------------------------------------------------------------------------*/

long  EXPORT SetLoopStartMsPlayer(short refnum, long date_ms)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	return (player) ? player->SetLoopStartMs(date_ms): kErrSequencer;
}

/*--------------------------------------------------------------------------*/

long  EXPORT SetLoopEndMsPlayer(short refnum, long date_ms)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	return (player) ? player->SetLoopEndMs(date_ms) : kErrSequencer;
}

/*--------------------------------------------------------------------------*/
// Synchronisation management
/*--------------------------------------------------------------------------*/

void  EXPORT SetSynchroInPlayer(short refnum, short state)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	if (player) player->SetSynchroIn(state);
}

/*--------------------------------------------------------------------------*/

void  EXPORT SetSynchroOutPlayer(short refnum, short state)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	if (player) player->SetSynchroOut(state);
}

/*--------------------------------------------------------------------------*/

void  EXPORT SetTempoPlayer(short refnum, long tempo)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	if (player) player->SetTempo(tempo);
}

/*--------------------------------------------------------------------------*/

void  EXPORT SetSMPTEOffsetPlayer(short refnum, SmpteLocPtr smptepos)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	if (player) player->SetSMPTEOffset(smptepos);
}
/*--------------------------------------------------------------------------*)
// State management
/*--------------------------------------------------------------------------*/

void  EXPORT GetStatePlayer(short refnum, PlayerStatePtr playerstate)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	if (player) player->GetState(playerstate);
}

/*--------------------------------------------------------------------------*/

void  EXPORT GetEndScorePlayer(short refnum, PlayerStatePtr playerstate)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	if (player) player->GetEndScore(playerstate);
}

/*--------------------------------------------------------------------------*/
// Step playing 
/*--------------------------------------------------------------------------*/

void  EXPORT ForwardStepPlayer(short refnum, short flag)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	if (player) player->ForwardStep(flag);
}

/*--------------------------------------------------------------------------*/

void  EXPORT BackwardStepPlayer(short refnum, short flag)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	if (player) player->BackwardStep(flag);
}

/*--------------------------------------------------------------------------*/
// Tracks management
/*--------------------------------------------------------------------------*/

MidiSeqPtr  EXPORT GetAllTrackPlayer(short refnum)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	return (player) ? player->GetAllTrack() : 0;
}

/*--------------------------------------------------------------------------*/

MidiSeqPtr  EXPORT GetTrackPlayer(short refnum, short tracknum)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	return (player) ? player->GetTrack(tracknum) : 0;
}

/*--------------------------------------------------------------------------*/

long  EXPORT SetTrackPlayer(short refnum, short tracknum, MidiSeqPtr s)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	return (player) ? player->SetTrack(tracknum,s) :kErrSequencer;
}

/*--------------------------------------------------------------------------*/

long  EXPORT SetAllTrackPlayer(short refnum, MidiSeqPtr s, long ticks_per_quarter)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	return (player) ? player->SetAllTrack(s,ticks_per_quarter) :kErrSequencer;
}

/*--------------------------------------------------------------------------*/

void  EXPORT SetParamPlayer(short refnum, short tracknum, short param, short value)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	if (player) player->SetParam(tracknum,param,value);
}

/*--------------------------------------------------------------------------*/

short  EXPORT GetParamPlayer(short refnum, short tracknum, short param)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	return (player) ? player->GetParam(tracknum,param) : kErrSequencer;
}

/*--------------------------------------------------------------------------*/

long  EXPORT InsertAllTrackPlayer(short refnum, MidiSeqPtr s)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	return (player) ? player->InsertAllTrack(s) : kErrSequencer;
}

/*--------------------------------------------------------------------------*/

long  EXPORT InsertTrackPlayer(short refnum, short tracknum, MidiSeqPtr s)
{
	TPlayerPtr player = (TPlayerPtr)MidiGetInfo(refnum);
	return (player) ? player->InsertTrack(tracknum,s) : kErrSequencer;
}




