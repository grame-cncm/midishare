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
//	CStruct.h			    
// ===========================================================================
//
//	Definition of C strutures
//


#ifndef __CStruct__
#define __CStruct__

#include "GenericMidiShare.h"

/*--------------------------------------------------------------------------*/

typedef struct PlayerState  FAR * PlayerStatePtr;
typedef struct PlayerState{
	long date;
	long tempo;
	short tsnum;
	short tsdenom;
	short tsclick;
	short tsquarter;
	short bar;
	short beat;
	short unit;
	short state;
	short syncin;
	short syncout;

}PlayerState;

/*--------------------------------------------------------------------------*/

typedef struct Pos  FAR * PosPtr;
typedef struct Pos{
	short bar;
	short beat;
	short unit;
}Pos;

#endif
