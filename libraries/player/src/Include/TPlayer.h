/*

  Copyright © Grame 1996-2006

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
//	TPlayer.h			    
// ===========================================================================

#ifndef __TPlayer__
#define __TPlayer__

#include "TPlayerFactory.h"
#include "TTrackTable.h"
#include "TSMPTEInfos.h"

/*--------------------------------------------------------------------------*/

typedef  void (* ApplyScorePtr)(MidiEvPtr e, void* data);

/*--------------------------------------------------------------------------*/
// Pattern FACADE
/*--------------------------------------------------------------------------*/
/*!
 \brief The main class which implements all functions from the API.
*/

class TPlayer : public TMidiAppl {

	friend class TPlayerFactory;
	friend class TPlayerMemento;

	private:

		TPlayerFactory   		fFactory;  		// Player factory
		TGenericPlayerInterfacePtr 	fPlayer;	// Current player
			
		TTrackTable		 fTrackTable;		// TrackTable: tracks state
		TPlayerScore	 fScore;			// Internal score
			
		TEventRecorderPtr  	fEventReceiver;		// The recorder object
		TClockConverterPtr  fClockConverter;	// Shared between several objects
		TLoopManagerPtr     fLoopManager;		// Loop manager
		TScoreInserterPtr  	fInserter;			// For insertion in the score
		
		TRunningState	 	fRunningState;    	// Running state
		
		TScoreStatePtr   	fScoreState;		// For internal state 
		TSMPTEInfos	        fSmpteInfos;        // For SMPTE synchronization
		
		ULONG	fTick_per_quarter;  	// Current resolution
		short   fSyncIn;         // SynchroIn state
		short   fSyncOut;		// SynchroOut state
		short   fOutput;		// Output state
			
	 public:
	 	
		TPlayer();
		virtual ~TPlayer();

		// Opening/Closing
		short Open(MidiName name);
		void Close();

		// Transport
		void Start();
		void Stop();
		void Pause();
		void Cont();

		// Position
		void SetPosBBU(PosPtr pos);
		void SetPosMs(long date_ms);
		void SetPosTicks(long date_ticks);

		// Loop management
		void SetLoop(Boolean state) ;
		long SetLoopStartBBU(PosPtr pos);
		long SetLoopEndBBU(PosPtr pos);
		long SetLoopStartMs(long date_ms);
		long SetLoopEndMs(long date_ms);

		// Tracks management
		MidiSeqPtr GetAllTrack();
		MidiSeqPtr GetTrack(short tracknum);

		long SetTrack(short tracknum, MidiSeqPtr seq);
		long SetAllTrack(MidiSeqPtr seq, long ticks_per_quarter);

		void SetParam(short tracknum, short p, short v);
		short GetParam(short tracknum, short p);

		long InsertAllTrack(MidiSeqPtr s);
		long InsertTrack(short tracknum,MidiSeqPtr s);

		// Synchronisation management
		void SetSynchroIn(short state) ;
		void SetSynchroOut(short state);
		void SetSMPTEOffset(SmpteLocPtr smptepos);
		void SetTempo(long tempo);

		// Record management
		void SetRecordMode(short state);
		void Record(short tracknum);
		void SetRecordFilter(MidiFilterPtr filter);

		// Step playing 
		void ForwardStep(short flag);
		void BackwardStep(short state);

		// Player state
		void GetState(PlayerStatePtr ps);
		void GetEndScore(PlayerStatePtr ps);

		// Alarms (Interface TMidiAppl)
		void ReceiveAlarm(short ref);
		void ApplAlarm(short ref,long code);

		// Output
		long SetOutput(short output);

		// Initialization
		static void Init() {TEventFactory::Init();}
		static void Destroy() {TEventFactory::Destroy();}
 };

typedef TPlayer FAR * TPlayerPtr;

#endif
