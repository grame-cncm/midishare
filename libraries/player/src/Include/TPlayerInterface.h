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
//	TPlayerInterface.h			    
// ===========================================================================
/*!
	\brief An interface for Players.
*/ 

#ifndef __TPlayerInterface__
#define __TPlayerInterface__


//------------------------
// Class TPlayerInterface 
//------------------------

class TPlayerInterface {
	
	 public:
	 
	 	virtual ~TPlayerInterface(){};
	 
		virtual void Start() = 0;
		virtual void Stop() = 0;
		virtual void Pause() = 0;
		virtual void Cont() = 0;
		
		virtual void PlaySliceForward() = 0;
		virtual void PlaySliceBackward() = 0;
	
		virtual void SetPosTicks (ULONG date_ticks) = 0;		
		virtual ULONG GetPosTicks() = 0;
		
		virtual void SetTempo (ULONG tempo) = 0;
		virtual ULONG GetTempo () = 0;
		
		virtual void RcvClock (ULONG date_ms) = 0;
};

typedef TPlayerInterface FAR * TPlayerInterfacePtr;


#endif