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
//	TTime.h			    
// ===========================================================================
//
//	Time Signature management, time in BBU manipulation
//


#ifndef __TTime__
#define __TTime__

 
#include "TPlayerConstants.h"
#include "GenericMidiShare.h"

//-----------------------
// Class TPos
//-----------------------
//
//	Position in BBU representation
//

class TTime;

class TPos{

	friend class TTime;
	
	private:
	
		ULONG fBar;
		ULONG fBeat;
		ULONG fUnit;
	
	public :
	
		TPos (){Init();}
		TPos (ULONG bar, ULONG beat, ULONG unit) { fBar = bar; fBeat = beat; fUnit = unit;}
		void Init(){fBar = fBeat = fUnit = 0;} 
		~TPos() {}
		
		ULONG GetBar()  {return fBar;}
		ULONG GetBeat() {return fBeat;}
		ULONG GetUnit() {return fUnit;}
};

typedef TPos FAR * TPosPtr;


//-----------------------
// Class TTime
//-----------------------
//
//	Algebra for BBU representation 
//


class TTime{

	friend class TTimeConverter;

	private :
	
		ULONG fTick_per_beat;
		ULONG fBeat_per_bar;
		ULONG fTick_per_bar;
		ULONG fTicks_per_quarter; 
		
		const TPos Normalize (const TPos& p1);
		const TPos Normalize1 (const TPos& p1);
		
	public :
	
		TTime() {Init(kDefaultTpq);}
		~TTime() {}
	
		void Init (ULONG tick);
		
		const TPos ConvertTicksToBBUInBar (ULONG nb_ticks);
		ULONG ConvertBBUToTicksInBar(const TPos& pos);
		
		const TPos 	Add (const TPos& p1, const TPos& p2);
		const TPos 	Sub (const TPos& p1, const TPos& p2);
		
		Boolean Inf (const TPos& p1, const TPos& p2);
		Boolean Sup (const TPos& p1, const TPos& p2);
		Boolean InfEq (const TPos& p1, const TPos& p2);
		Boolean SupEq (const TPos& p1, const TPos& p2);

};

typedef TTime FAR * TTimePtr;

#endif
