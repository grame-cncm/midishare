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
//	TTempoConverter.cpp		    
// ===========================================================================
//
// Tempo management: musical time (ticks) to absolute time (millisecond) 
// conversion functions (and vice versa)
//
// Computation are done using Integer values and managing remainders to be exact
// 

#include "TTempoConverter.h"
#include "UMath.h"
#include "UTools.h"
#include "UDebug.h"


/*--------------------------------------------------------------------------*/
// Init with a new Tick_Per_Quarter value
/*--------------------------------------------------------------------------*/

void TTempoConverter::Init (ULONG tpq) 
{
	assert (tpq > 0);
	
 	fTicks_per_quarter = tpq;
 	fLast_tempo_ten_micro =  0;
	fTempo = kDefaultTempo;
	fLast_tempo = 0;
	fTen_micro_sec_per_tick = kDefaultTempo/fTicks_per_quarter;
	fTen_micro_sec_per_tick_remain = kDefaultTempo % fTicks_per_quarter;
}	

/*--------------------------------------------------------------------------*/
// Init with the current Tick_Per_Quarter value
/*--------------------------------------------------------------------------*/

void TTempoConverter::Init () 
{
	fLast_tempo_ten_micro = 0;
	fTempo = kDefaultTempo;
	fLast_tempo = 0;
	fTen_micro_sec_per_tick = kDefaultTempo/fTicks_per_quarter;  
	fTen_micro_sec_per_tick_remain = kDefaultTempo % fTicks_per_quarter;
}	

/*--------------------------------------------------------------------------*/
// Update internal state with a Tempo event (using it's date and tempo value)
/*--------------------------------------------------------------------------*/


void TTempoConverter::Update (MidiEvPtr ev)
{
	ULONG newtempo = Tempo(ev)/10; 

	if ((newtempo > 0) && (fTempo != newtempo)) { // Update values only if the Tempo has changed
 		fLast_tempo_ten_micro = ConvertTickToMicroSec (Date(ev));
		fLast_tempo = Date(ev);
		fTempo = newtempo;
		fTen_micro_sec_per_tick = fTempo/fTicks_per_quarter;
		fTen_micro_sec_per_tick_remain = fTempo % fTicks_per_quarter;
	}
}	

/*--------------------------------------------------------------------------*/
// Update internal state with a Tempo and a date 
/*--------------------------------------------------------------------------*/

void TTempoConverter::Update (ULONG date_ticks, ULONG tempo) 
{
	ULONG newtempo = tempo/10; 
	
	if ((newtempo > 0) && (fTempo !=  newtempo)) { // Update values only if the Tempo has changed
	
		fLast_tempo_ten_micro = ConvertTickToMicroSec (date_ticks);
		fLast_tempo = date_ticks;
		fTempo = newtempo;
		fTen_micro_sec_per_tick = fTempo/fTicks_per_quarter;
		fTen_micro_sec_per_tick_remain = fTempo % fTicks_per_quarter;
	}
}	

/*--------------------------------------------------------------------------*/
// Convert a date in (10  micro sec) in ticks 
/*--------------------------------------------------------------------------*/

ULONG TTempoConverter::ConvertTickToMicroSec (ULONG date_ticks)
{
	ULONG delta ;
	
	if (date_ticks >= fLast_tempo){
	
		delta =  date_ticks - fLast_tempo;
		return fLast_tempo_ten_micro + 
		 	delta * fTen_micro_sec_per_tick +  
			(delta *  fTen_micro_sec_per_tick_remain) / fTicks_per_quarter;

	}else{
	
		delta =  fLast_tempo - date_ticks;		
		return fLast_tempo_ten_micro -  
		 	delta *  fTen_micro_sec_per_tick -  
			(delta *  fTen_micro_sec_per_tick_remain) / fTicks_per_quarter;
	}
}

/*--------------------------------------------------------------------------*/
// Convert a date in ticks  in (10  micro sec)
/*--------------------------------------------------------------------------*/


ULONG TTempoConverter::ConvertMicroSecToTick (ULONG date_ten_micro)
{
	if (date_ten_micro >= fLast_tempo_ten_micro){
		return fLast_tempo +  ConvertMicroSecToTicksAux(date_ten_micro - fLast_tempo_ten_micro);
	}else{
		return fLast_tempo -  ConvertMicroSecToTicksAux(fLast_tempo_ten_micro - date_ten_micro);
	}
}


/*--------------------------------------------------------------------------*/
// Convert a date in ticks in millisecond
/*--------------------------------------------------------------------------*/

ULONG TTempoConverter::ConvertTickToMs  (ULONG date_tick) {return ConvertTickToMicroSec (date_tick) / 100;}

/*--------------------------------------------------------------------------*/
// Convert a date in millisecond in ticks 
/*--------------------------------------------------------------------------*/

ULONG TTempoConverter::ConvertMsToTick  (ULONG date_ms) {return ConvertMicroSecToTick (date_ms * 100);}		


/*--------------------------------------------------------------------------*/
// Internal functions
/*--------------------------------------------------------------------------*/

ULONG TTempoConverter::ConvertTicksToMicroSecAux (ULONG ticks)
{
	return ticks * fTen_micro_sec_per_tick +  (ticks * fTen_micro_sec_per_tick_remain) / fTicks_per_quarter;
}

/*--------------------------------------------------------------------------*/


#ifdef __Macintosh__

ULONG TTempoConverter::ConvertMicroSecToTicksAux (ULONG ten_micro_sec)
{	
	if (fTempo) {
		return UMath::CalcLong(ten_micro_sec, fTicks_per_quarter, fTempo);
	}else{
		return 1;
    }
}

#endif
/*--------------------------------------------------------------------------*/

#ifdef __MSWindows__

ULONG TTempoConverter::ConvertMicroSecToTicksAux (ULONG ten_micro_sec)
{	
	if (fTempo) {
		return (ULONG)(((double)ten_micro_sec * (double) fTicks_per_quarter) / (double) fTempo);
	}else{
		return 1;
    }
}

#endif

/*--------------------------------------------------------------------------*/

#ifdef __Linux__

ULONG TTempoConverter::ConvertMicroSecToTicksAux (ULONG ten_micro_sec)
{	
	if (fTempo) {
		return (ULONG)(((double)ten_micro_sec * (double) fTicks_per_quarter) / (double) fTempo);
	}else{
		return 1;
    }
}

#endif


