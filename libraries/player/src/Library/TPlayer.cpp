// ===========================================================================
// The Player Library is Copyright (c) Grame, Computer Music Research Laboratory 
// 1996-1999, and is distributed as Open Source software under the Artistic License;
// see the file "Artistic" that is included in the distribution for details.
//
// Grame : Computer Music Research Laboratory
// Web : http://www.grame.fr/Research
// E-mail : MidiShare@rd.grame.fr
// ===========================================================================



// ===========================================================================
//	TPlayer.cpp			    
// ===========================================================================
//
//	The main class which implements all functions from the API
// 
// 


#include "TPlayer.h"
#include "UScoreManager.h"
#include "TTempoMapBuilder.h"
#include "TPlayerMemento.h"
#include "UMath.h"
#include "UMidi.h"

#include <assert.h>

/*--------------------------------------------------------------------------*/
// Creation/Destruction
/*--------------------------------------------------------------------------*/

TPlayer::TPlayer()
{
	fPlayer = 0;
	fFactory = 0;
	fTrackTable = 0;
	fScore = 0;
	fEventReceiver = 0;
	fClockConverter = 0;
	fLoopManager = 0;
	fRunningState = 0;
	fInserter = 0;
	fScoreState = 0;
	fSmpteInfos = 0;
	
	fSyncIn = kInternalSync;
	fSyncOut = kNoSyncOut;
	fOutput = kMidiShare;
	fTick_per_quarter = kDefaultTpq;
	
	// Initialisation of the Event Factory
	TEventFactory::Init();
	
	// Initialisation of the Player Factory
	fFactory = new TPlayerFactory(this);
}

/*--------------------------------------------------------------------------*/
// Create shared objects
/*--------------------------------------------------------------------------*/

void TPlayer::Create() 
{
	fTrackTable = new TTrackTable();
	fScore = new TPlayerScore();
	fRunningState = new TRunningState();
	fSmpteInfos = new TSMPTEInfos();
}

/*--------------------------------------------------------------------------*/

TPlayer::~TPlayer()
{
	if (fFactory) {	
		fFactory->DestroyPlayer(&fPlayer);
		delete (fFactory);
	}
	
	if (fTrackTable) 	delete (fTrackTable);
	if (fScore) 		delete (fScore); 
	if (fRunningState) 	delete (fRunningState);
	if (fSmpteInfos) 	delete (fSmpteInfos);
	
	TMidiAppl::Close();
}

/*--------------------------------------------------------------------------*/
// Open/Close
/*--------------------------------------------------------------------------*/

short TPlayer::Open (MidiName name) 
{ 
	// Open the MidiShare application and check the result
	short ref = TMidiAppl::Open(name);
	if (ref < 0) return kErrAppl;
	
	// Check the MidiShare memory
	MidiGrowSpace(kMemoryLimit);
	if (MidiFreeSpace() < kMemoryLimit) {
		Close();
		return kErrMemory;
	}
	
	// Create shared objects
	Create();
	
	// Set an empty score
	long res = SetAllTrack (MidiNewSeq(), kDefaultTpq);
	
	// If OK, return the reference number else close the Player
	if (res == kNoErr) {
		return ref;
	}else {
		Close();
		return kErrMemory;
	}
}

/*--------------------------------------------------------------------------*/

void TPlayer::Close() 
{ 
	if (fRunningState->IsRunning()) Stop();
	
}

/*--------------------------------------------------------------------------*/
// Transport
/*--------------------------------------------------------------------------*/

void TPlayer::Start() { fPlayer->Start(); }

/*--------------------------------------------------------------------------*/

void TPlayer::Stop() { fPlayer->Stop(); }

/*--------------------------------------------------------------------------*/

void TPlayer::Pause() { fPlayer->Pause(); }

/*--------------------------------------------------------------------------*/

void TPlayer::Cont() { fPlayer->Cont(); }

/*--------------------------------------------------------------------------*/
// Position
/*--------------------------------------------------------------------------*/

void TPlayer::SetPosBBU (PosPtr pos) { fPlayer->SetPosBBU(TPos (pos->bar - 1, pos->beat - 1, pos->unit - 1)); }

/*--------------------------------------------------------------------------*/

void TPlayer::SetPosMs (long date_ms) { fPlayer->SetPosMs(date_ms); }

/*--------------------------------------------------------------------------*/

void TPlayer::SetPosTicks (long date_ticks) { fPlayer->SetPosTicks(date_ticks); }


/*--------------------------------------------------------------------------*/
// Tracks management
/*--------------------------------------------------------------------------*/

 MidiSeqPtr TPlayer::GetAllTrack ()	{ return (fRunningState->IsIdle()) ? fScore->GetAllTrack(): 0;}
 MidiSeqPtr TPlayer::GetTrack (short tracknum) { return (fRunningState->IsIdle()) ? fScore->GetTrack(tracknum): 0;}

 /*--------------------------------------------------------------------------*/

 long 	TPlayer::SetTrack (short tracknum, MidiSeqPtr s)
 {
 	if (fRunningState->IsIdle()) {
 	
 		MidiSeqPtr s1 = UMidi::BuildTrack(s); // Build the sequence
	 	if (!s1) return kErrEvent;
	 		
	 	long res;
	 	fFactory->DestroyPlayer(&fPlayer);
	 		
	 	if ((res = fScore->SetTrack(tracknum,s1)) == kNoErr) {
	  		fPlayer = fFactory->CreatePlayer();
	 	}
	 	
	 	return res;
	 }else
	 	return kErrSequencer;
 }
 
 /*--------------------------------------------------------------------------*/

 long TPlayer::SetAllTrack (MidiSeqPtr s, long ticks_per_quarter)
 {
 	if (fRunningState->IsIdle() && (ticks_per_quarter > 0)) {
 	
 		MidiSeqPtr s1 = UMidi::BuildAllTrack(s); // Build the sequence
 		if (!s1) return kErrEvent;
	 	
 		long res;
 		fTick_per_quarter = ticks_per_quarter;
 		fFactory->DestroyPlayer(&fPlayer);

	 	if ((res = fScore->SetAllTrack(s1)) == kNoErr) {
	 		
 			fPlayer = fFactory->CreatePlayer();
	 		
	 		TPlayerMemento memento;
	 		memento.DefaultState(this);
		}
		return res;
	 	
	 }else
	 	return kErrSequencer;
 }

/*--------------------------------------------------------------------------*/
   long TPlayer::InsertAllTrack(MidiSeqPtr s)
 {
 	if (UMidi::IsEmpty(s)) return kNoErr;
 
 	long res;
 	ULONG date_ticks = Date(FirstEv(s));
 	ULONG cur_date_ticks = fPlayer->GetPosTicks();
 	
 	if ((res = fInserter->InsertAllTrack(s)) == kNoErr) { 
 		if (fRunningState->IsIdle()) {
 			if (date_ticks < cur_date_ticks){
 				fPlayer->SetPosTicks(date_ticks);
 			}else{
 				fPlayer->SetPosTicks(cur_date_ticks);
 			}
 			fPlayer->Cont();
 		}
 	}
  	return res;
  }
 
/*--------------------------------------------------------------------------*/

long TPlayer::InsertTrack(short tracknum,MidiSeqPtr s)
 { 
  	if (UMidi::IsEmpty(s)) return kNoErr;
 
 	long res;
 	ULONG date_ticks = Date(FirstEv(s));
 	ULONG cur_date_ticks =  fPlayer->GetPosTicks();	
 	
 	if ((res = fInserter->InsertTrack(tracknum,s)) == kNoErr) { 
 		if (fRunningState->IsIdle()) {
 			if (date_ticks < cur_date_ticks) {
 				fPlayer->SetPosTicks(date_ticks);
 			}else{
 				fPlayer->SetPosTicks(cur_date_ticks);
 			}
 			fPlayer->Cont();
 		}
 	}
   	return res;
 } 
 
/*--------------------------------------------------------------------------*/

void TPlayer::SetParam (short tracknum, short p, short v) 
{ 
	if(fRunningState->IsRunning()){
		fPlayer->Stop();
 		fTrackTable->SetParam (tracknum, p, v);
 		fPlayer->Cont();
 	}else{
 		fTrackTable->SetParam (tracknum, p, v);
 	}
 }

/*--------------------------------------------------------------------------*/

 short TPlayer::GetParam (short tracknum, short p)	{ return fTrackTable->GetParam (tracknum, p);}

/*--------------------------------------------------------------------------*/
// Synchronisation management
/*--------------------------------------------------------------------------*/

void TPlayer::SetSynchroIn (short state) 
{ 
	if (fRunningState->IsIdle() && (state>= 0 ) && (state < kMaxSyncIn)) {
		fSyncIn = state;

		TPlayerMemento memento;
		memento.SaveState(this);			// Save the current state
		
 		fFactory->DestroyPlayer(&fPlayer);	// Delete the current Player
 		fPlayer = fFactory->CreatePlayer();	// Allocate a new Player
 		
 		memento.RestoreState(this);			// Restore the current state
 	}
}

/*--------------------------------------------------------------------------*/

  void TPlayer::SetSynchroOut (short state)
 {
 	if(fRunningState->IsIdle() && (state>= 0 ) && (state < kMaxSyncOut)) {
 		fSyncOut = state;
  
   		TPlayerMemento memento;
		memento.SaveState(this);			// Save the current state
  		
	 	fFactory->DestroyPlayer(&fPlayer);	// Delete the current Player
	 	fPlayer = fFactory->CreatePlayer();	// Allocate a new Player
	 	
	 	memento.RestoreState(this);			// Restore the current state
	 }
 }

/*--------------------------------------------------------------------------*/

 void TPlayer::SetSMPTEOffset (SmpteLocPtr smptepos) { fSmpteInfos->SetSMPTEOffset(smptepos); }
 
/*--------------------------------------------------------------------------*/

 void TPlayer::SetTempo(long tempo){ fPlayer->SetTempo(tempo);}
 
 /*--------------------------------------------------------------------------*/

 void TPlayer::SetTempoFactor(float factor){ fPlayer->SetTempoFactor(factor);}

 
/*--------------------------------------------------------------------------*/
// Step playing
/*--------------------------------------------------------------------------*/

void TPlayer::ForwardStep(short flag) { fPlayer->PlaySliceForward(); }

/*--------------------------------------------------------------------------*/

void TPlayer::BackwardStep(short flag) { fPlayer->PlaySliceBackward(); }
		
/*--------------------------------------------------------------------------*/
// Player state
/*--------------------------------------------------------------------------*/

void TPlayer::GetState (PlayerStatePtr ps)
{
	fScoreState->FillState(ps,fPlayer->GetPosTicks());
	
	ps->tempo = fPlayer->GetTempo();
	
	ps->syncin = fSyncIn;
	ps->syncout = fSyncOut;
	
	ps->state = fRunningState->GetState();
	if ((ps->state == kPlaying) && (fEventReceiver->GetRecordTrack() != kNoTrack)) 
		ps->state = kRecording;
}

/*--------------------------------------------------------------------------*/

void TPlayer::GetEndScore (PlayerStatePtr ps)
{ 
	TScoreState tmp(fScore,fTick_per_quarter);
	tmp.FillState(ps,fScore->GetLastDate());
}
 
/*--------------------------------------------------------------------------*/
// Loop management
/*--------------------------------------------------------------------------*/

void TPlayer::SetLoop (short state) {fLoopManager->SetLoop(state);}

/*--------------------------------------------------------------------------*/

long TPlayer::SetLoopStartBBU (PosPtr pos) 
{	
	return fLoopManager->SetLoopStartBBU(TPos(pos->bar - 1, pos->beat - 1, pos->unit - 1));
}
	 
/*--------------------------------------------------------------------------*/

long TPlayer::SetLoopEndBBU (PosPtr pos) 
{
	return fLoopManager->SetLoopEndBBU(TPos(pos->bar - 1, pos->beat - 1, pos->unit - 1));
}

/*--------------------------------------------------------------------------*/

long TPlayer::SetLoopStartMs(long date_ms){ return fLoopManager->SetLoopStartMs(date_ms);}
 
/*--------------------------------------------------------------------------*/

long TPlayer::SetLoopEndMs(long date_ms){ return fLoopManager->SetLoopEndMs(date_ms);}
	
	
/*--------------------------------------------------------------------------*/
// Record management
/*--------------------------------------------------------------------------*/

void TPlayer::SetRecordMode (short state) 
{  
	if(fRunningState->IsRunning()){
		fPlayer->Stop();
 		fEventReceiver->SetRecordMode (state);
 		fPlayer->Cont();
 	}else{
 		fEventReceiver->SetRecordMode (state);
 	}
}

/*--------------------------------------------------------------------------*/

 void TPlayer::SetRecordFilter(MidiFilterPtr filter) { fEventReceiver->SetRecordFilter(filter); }
 
/*--------------------------------------------------------------------------*/

void TPlayer::Record (short tracknum) 
{ 	
	if(fRunningState->IsRunning()){
		fPlayer->Stop();
 		fEventReceiver->SetRecordTrack(tracknum);
 		fPlayer->Cont();
 	}else{
 		fEventReceiver->SetRecordTrack(tracknum);
 	}
}

/*--------------------------------------------------------------------------*/

 long TPlayer::SetOutput(short output) 
 { 
 	if(fRunningState->IsIdle() && (output >= 0) && (output < KMaxOutput)) {
 		fOutput = output;

  		TPlayerMemento memento;
		memento.SaveState(this);			// Save the current state
		
	 	fFactory->DestroyPlayer(&fPlayer);	// Delete the current Player
	 	fPlayer = fFactory->CreatePlayer();	// Allocate a new Player
	 	
	 	if (fPlayer == 0) {
	 		return kErrSequencer; // If allocation error
	 	}else {
	 		memento.RestoreState(this);		// Restore the current state
	 		return kNoErr;
	 	}
	 }else {
	 	return kErrSequencer; 
	 }
}

/*--------------------------------------------------------------------------*/
// Alarms
/*--------------------------------------------------------------------------*/

void TPlayer::ReceiveAlarm (short ref) 
{
	MidiEvPtr e1,e2;
	
	// Player may not be correctly allocated when the first ReceiveAlarm is called
	
	if (fPlayer) {  
		while (e1 = MidiGetEv(ref)) { 
			switch (EvType (e1)) {
				case typeNote:
				
					// Note events are separated in a KeyOn/KeyOff pair, the KeyOff
					// is sent to the application itself after the note duration.
					
					if (e2 = UMidi::NoteToKeyOff(e1)) {
						SendToSelf(e2);
						fPlayer->ReceiveEvents(e1);
					}else
						MidiFreeEv(e1);
					break;
				
				default:
					fPlayer->ReceiveEvents(e1);
					break;
			}
		} 
	}else{
		MidiFlushEvs(ref);
	}
}

/*--------------------------------------------------------------------------*/

void TPlayer::ApplAlarm (short ref,long code) 
{ 
	// For SMPTE synchronization, MIDISyncStart and MIDISyncStop are converted
	// in Start and Stop events for homogeneous management in the Player.
	
	if (fPlayer) {
		switch (code) {
			
			case MIDISyncStart:
				fPlayer->ReceiveEvents(MidiNewEv(typeStart));
				break;
				
			case MIDISyncStop:
				fPlayer->ReceiveEvents(MidiNewEv(typeStop));
				break;
		}
	}
}

