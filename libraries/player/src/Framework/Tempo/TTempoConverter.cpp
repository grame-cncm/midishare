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
//	TTempoConverter.cpp		    
// ===========================================================================


#include "TTempoConverter.h"
#include "UMath.h"
#include "UTools.h"
#include "UDebug.h"

/*--------------------------------------------------------------------------*/
/*!
 \brief Inits with a new Tick_Per_Quarter value.
 \param tpq is the tick_per_quarter value.
*/

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
/*!
 \brief Inits with the current Tick_Per_Quarter value.
*/

void TTempoConverter::Init () 
{
	fLast_tempo_ten_micro = 0;
	fTempo = kDefaultTempo;
	fLast_tempo = 0;
	fTen_micro_sec_per_tick = kDefaultTempo/fTicks_per_quarter;  
	fTen_micro_sec_per_tick_remain = kDefaultTempo % fTicks_per_quarter;
}	

/*--------------------------------------------------------------------------*/
/*!
 \brief Updates internal state with a tempo event using it's date and tempo value.
 \param  ev is the tempo event.
*/

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
/*!
 \brief Updates internal state with a tempo and a date.
 \param date_ticks is the date in ticks.
 \param tempo is the tempo in micro-sec per quarter note.
*/

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
/*!
 \brief Converts a date in ticks in a 10 micro-sec date. 
 \param date_ticks is the date in ticks.
 \return The result is the date in 10 micro-sec.
*/

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
/*!
 \brief Convert a 10 micro-sec date in a date in ticks.
 \param date_ten_micro is the date in 10 micro-sec.
 \return The result is the date in ticks.
*/

ULONG TTempoConverter::ConvertMicroSecToTick (ULONG date_ten_micro)
{
	if (date_ten_micro >= fLast_tempo_ten_micro){
		return fLast_tempo +  ConvertMicroSecToTicksAux(date_ten_micro - fLast_tempo_ten_micro);
	}else{
		return fLast_tempo -  ConvertMicroSecToTicksAux(fLast_tempo_ten_micro - date_ten_micro);
	}
}

/*--------------------------------------------------------------------------*/
/*!
 \brief Convert a date in ticks in a date in milliseconds.
 \param date_ticks is the date in ticks
 \return The result is the date in milliseconds.
*/

ULONG TTempoConverter::ConvertTickToMs  (ULONG date_ticks) {return ConvertTickToMicroSec (date_ticks) / 100;}

/*--------------------------------------------------------------------------*/
/*!
 \brief Convert a date in millisecond in a date in ticks.
 \param date_ms is the date in milliseconds
 \return The result is the date in ticks.
*/

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

	#ifdef __MacOS9__
	ULONG TTempoConverter::ConvertMicroSecToTicksAux (ULONG ten_micro_sec)
	{	
		if (fTempo) {
			return UMath::CalcLong(ten_micro_sec, fTicks_per_quarter, fTempo);
		}else{
			return 1;
	    }
	}
	#else
	ULONG TTempoConverter::ConvertMicroSecToTicksAux (ULONG ten_micro_sec)
	{	
		if (fTempo) {
			return (ULONG)(((double)ten_micro_sec * (double) fTicks_per_quarter) / (double) fTempo);
		}else{
			return 1;
	    }
	}
	#endif
	
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


