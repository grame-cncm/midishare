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
//	TEventProducerInterface.h			    
// ===========================================================================
//
//	Event producer interface
//


#ifndef __TEventProducerInterface__
#define __TEventProducerInterface__
 
#include "GenericMidiShare.h"

//-------------------------------
// Class TEventProducerInterface 
//-------------------------------
// The event returned by the ProduceEvent event can be freely used.

class TEventProducerInterface {
	
	public :
 
 		virtual ~TEventProducerInterface() {}
 		
 		virtual void Init(ULONG date_ticks) = 0;
 		virtual ULONG GetPosTicks() = 0;
		virtual void SetPosTicks(ULONG date_ticks) = 0;
		virtual MidiEvPtr ProduceEvent(ULONG date_ticks) = 0;
	
 };
 
 
typedef TEventProducerInterface FAR * TEventProducerInterfacePtr;


#endif