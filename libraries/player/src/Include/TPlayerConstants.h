// ===========================================================================
// The Player Library is Copyright (c) Grame, Computer Music Research Laboratory 
// 1996-2003, and is distributed as Open Source software under the Artistic License;
// see the file "Artistic" that is included in the distribution for details.
//
// Grame : Computer Music Research Laboratory
// Web : http://www.grame.fr/Research
// E-mail : MidiShare@rd.grame.fr
// ===========================================================================



// ===========================================================================
//	TPlayerConstants.h			    
// ===========================================================================
//
//	Players constants


#ifndef __TPlayerConstants__
#define __TPlayerConstants__


//-----------------------
// Version Number 
//-----------------------

#define kVersion 201

//-----------------------
// MidiShare memory limit
//-----------------------

#define kMemoryLimit 100

//-----------------------
// Errors messages
//-----------------------

#define kNoErr 			-1		// No error			            		
#define	kErrAppl		-2		// Unable to open MidiShare application	
#define kErrEvent  		-3		// No more MidiShare Memory 			
#define	kErrMemory		-4		// No more System Memory 			        
#define kErrSequencer	-5		// Sequencer error			            


//-----------------------
// Tracks Description 
//-----------------------

#define kMaxTrack 256           // Maximum track number
#define kMinTrack 1          	// Min value of the usable track (Track 0 is reserverd for the TempoMap)
#define kNoTrack -1             // For record management 

#define kTempoMap  0            // Number of the TempoMap track


#define kEraseOff 	0
#define kEraseOn 	1

#define kMuteOn		1
#define kMuteOff	0
#define kSoloOn		1
#define kSoloOff	0

enum tracksParameters { kMute, kSolo, kMaxParam };
enum loopState {kLoopOn = 0, kLoopOff};

#define kEndScoreOffset 10

//---------------------------------
// For the default controler table
//---------------------------------               

#define kMaxCtrl 127

#define TrackNum(e) RefNum(e)
#define kMaxTpq 500


//-----------------------
// For HashTables (Chase)
//-----------------------

#define MatchEvent(e1,e2) ((Pitch((e1)) == Pitch((e2))) && (RefNum ((e1)) == RefNum ((e2))) && (Chan ((e1))== Chan ((e2))) &&(Port ((e1)) == Port ((e2))))
#define MatchEvent1(e1,e2) ((RefNum ((e1)) == RefNum ((e2))) && (Chan ((e1))== Chan ((e2))) && (Port ((e1))== Port ((e2))))

#define HashCode(h,c) (Byte)(((h) & 0x0F) + ((c) & 0x0F << 4))
#define kMaxHashCode 256

//-----------------------------------
// TimeSign and Tempo default values
//-----------------------------------

#define kDefaultTpq 		500   
#define kDefaultBar 		4
#define kDefaultTempo 		50000      // For internal structure
#define kDefaultTempoEv 	500000	   // For the score

#define kDefaultNum 		4
#define kDefaultDenom 		2
#define kDefaultClocks 		0
#define kDefaultN32 		0
	

//-----------------------
// Private Events
//-----------------------

#define typeScoreEnd 	19
#define typeLoopEnd 	20

//-----------------------
// Synchro states 
//-----------------------

enum syncOutState { kNoSyncOut, kClockSyncOut, kMaxSyncOut};
enum syncInState { kInternalSync = 0, kClockSync, kSMPTESync , kExternalSync, kMaxSyncIn };
	
//-----------------------
// Player states 
//-----------------------

enum playerState {kIdle = 0, kPause, kRecording, kPlaying,kWaiting};

//-----------------------
// Player Output 
//-----------------------

enum playerOutput {kMidiShare = 0, kQuickTime, KMaxOutput};

//-----------------------
// Additional types
//-----------------------

typedef unsigned long ULONG;



#endif

