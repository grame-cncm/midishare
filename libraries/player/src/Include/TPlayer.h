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
//	TPlayer.h			    
// ===========================================================================
//
//	The main class which implements all functions from the API
// 
// 


#ifndef __TPlayer__
#define __TPlayer__


#include "TPlayerFactory.h"
#include "TTrackTable.h"
#include "TSMPTEInfos.h"

/*--------------------------------------------------------------------------*/

typedef  void (* ApplyScorePtr)( MidiEvPtr e , void* data);

/*--------------------------------------------------------------------------*/
// Pattern FACADE
/*--------------------------------------------------------------------------*/

class TPlayer :public  TMidiAppl{

	friend class TPlayerFactory;
	friend class TPlayerMemento;

	private:

		TPlayerFactoryPtr   		fFactory;  			// Player factory
		TGenericPlayerInterfacePtr 	fPlayer;			// Current player
			
		TTrackTablePtr 	  			fTrackTable;		// TrackTable: tracks state
		TPlayerScorePtr	 			fScore;				// Internal score
			
		TEventRecorderPtr  			fEventReceiver;     // The recorder object
		TClockConverterPtr  		fClockConverter;	// Shared between several objects
		TLoopManagerPtr     		fLoopManager;		// Loop manager
		TScoreInserterPtr  			fInserter;			// For insertion in the score
		TRunningStatePtr 			fRunningState;    	// Running state
		TScoreStatePtr   			fScoreState;		// For internal state 
		TSMPTEInfosPtr              fSmpteInfos;        // For SMPTE synchronization
		
		ULONG	fTick_per_quarter;  // Current resolution
		short   fSyncIn;         	// SynchroIn state
		short   fSyncOut;		 	// SynchroOut state
		short   fOutput;		 	// Output state
			
		void Create();
	
	 public:
	 	
	 	TPlayer();
	 	~TPlayer();
	 
	 	// Opening/Closing
	 	 short Open (MidiName name);
		 void Close ();
	 		
		 // Transport
		 void Start();
		 void Stop();
		 void Pause();
		 void Cont();
		
		 // Position
		 void SetPosBBU (PosPtr pos);
		 void SetPosMs (long date_ms);
		 void SetPosTicks (long date_ticks);
		
		
		 // Loop management
		 void SetLoop (short state) ;
		 long SetLoopStartBBU (PosPtr pos);
		 long SetLoopEndBBU (PosPtr pos);
		 long SetLoopStartMs(long date_ms);
		 long SetLoopEndMs(long date_ms);
		
		 // Tracks management
		 MidiSeqPtr GetAllTrack ();
		 MidiSeqPtr GetTrack (short tracknum);

		 long SetTrack (short tracknum, MidiSeqPtr seq);
		 long SetAllTrack (MidiSeqPtr seq, long ticks_per_quarter);
		
		 void SetParam (short tracknum, short p, short v);
		 short GetParam (short tracknum, short p);
		
		 long InsertAllTrack(MidiSeqPtr s);
		 long InsertTrack(short tracknum,MidiSeqPtr s);
		 
		 // Synchronisation management
		 void SetSynchroIn (short state) ;
		 void SetSynchroOut (short state);
		 void SetSMPTEOffset (SmpteLocPtr smptepos);
		 void SetTempo(long tempo);
		 void SetTempoFactor(float factor);

		 // Record management
		 void SetRecordMode (short state);
		 void Record (short tracknum);
		 void SetRecordFilter(MidiFilterPtr filter);

		 // Step playing 
		 void ForwardStep(short flag);
		 void BackwardStep(short flag);
		
		 // Player state
		 void GetState (PlayerStatePtr ps);
		 void GetEndScore (PlayerStatePtr ps);
		 
		 // Alarms (Interface TMidiAppl)
		 void ReceiveAlarm (short ref);
		 void ApplAlarm (short ref,long code);
		 
		 // Output
		 long SetOutput(short output);
	 	
};

typedef TPlayer FAR * TPlayerPtr;



#endif
