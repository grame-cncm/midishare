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
//	TSynchroniserInterface.h	   			 
// ===========================================================================
//
//	Synchroniser interface
// 


#ifndef __TSynchroniserInterface__
#define __TSynchroniserInterface__


//------------------------------
// Class TSynchroniserInterface 
//------------------------------


class TSynchroniserInterface {
	
	public: 
		
		virtual ~TSynchroniserInterface() {};
		
		virtual void Init() = 0;
		virtual ULONG ConvertTickToMs(ULONG date_tick) = 0;
		virtual ULONG ConvertMsToTick (ULONG date_ms) = 0;
		
		virtual void  SetTempo (ULONG date_ticks, ULONG tempo) = 0;
 		virtual void  SetTempo (ULONG tempo) = 0;
 		virtual ULONG GetTempo () = 0;
 		
 		virtual void  RcvClock (ULONG date_ms) = 0;
 		virtual Boolean IsSchedulable(ULONG date_tick) = 0;
 };


typedef TSynchroniserInterface FAR * TSynchroniserInterfacePtr;


#endif
