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
//	TTempoMap.h		    
// ===========================================================================
//

#ifndef __TTempoMap__
#define __TTempoMap__


#include "TTempoConverter.h"
#include "TTimeConverter.h"


//-----------------
// Class TTempoMap
//-----------------
/*! 
 \brief A class used to maintain the current Tempo and TimeSign state and provide conversion functions 
*/

class TTempoMap {

	protected:
		
		/*! Current date in Ticks */
		ULONG	fCurdate_ticks;		
		/*! Current position in Bar,Beat,Unit	*/
		TPos	fCur_pos;         
		/*! Current date in 10 micro sec 	*/	
		ULONG	fCur_date_ten_micro;   	
		
		/*! Tempo converter */
		TTempoConverter fTempoConverter;  
		/*! TimeSign converter */
		TTimeConverter  fTimeConverter;   
				
	public:
 
 		TTempoMap(ULONG tpq);
 		virtual ~TTempoMap(){}
 		
 		void Init ();
 		
 		ULONG CurDateTicks() {return fCurdate_ticks;}
		ULONG CurDateMicro() {return fCur_date_ten_micro;}
		ULONG CurDateMs() {return fCur_date_ten_micro/100;}
		const TPos CurDateBBU() {return fCur_pos;}
		
		short GetNum()     {return fTimeConverter.GetNum();}
		short GetDenom()   {return fTimeConverter.GetDenom();}
		short GetNClocks()  {return fTimeConverter.GetNClocks();}
		short GetN32nd()    {return fTimeConverter.GetN32nd();}
			
		ULONG GetTempo()   {return fTempoConverter.GetTempo();}
		void SetTempo(ULONG date_ticks,ULONG tempo) {fTempoConverter.Update(date_ticks,tempo);}
			
		const TPos ConvertTickToBBU(ULONG date_tick) {return fTimeConverter.ConvertTickToBBU (date_tick);}
		ULONG ConvertBBUToTick(const TPos& pos) {return fTimeConverter.ConvertBBUToTick (pos);}
		
		ULONG ConvertTickToMs(ULONG date_tick) {return fTempoConverter.ConvertTickToMs(date_tick);}
		ULONG ConvertMsToTick(ULONG date_ms) {return fTempoConverter.ConvertMsToTick(date_ms);}
		
		ULONG ConvertTickToMicroSec(ULONG date_tick) {return fTempoConverter.ConvertTickToMicroSec(date_tick);}
		ULONG ConvertMicroSecToTick(ULONG date_ms)   {return fTempoConverter.ConvertMicroSecToTick(date_ms);}
		
		void UpdateBBU(const TPos& pos);
		void UpdateTicks(ULONG date_ticks);
		void UpdateTenMicroSec(ULONG date_ten_micro);	
		void UpdateMs(ULONG date_ms);	
			
		Boolean SupEq(ULONG date_ticks, ULONG date_ten_micro) 	{return  fTempoConverter.ConvertTickToMicroSec(date_ticks) >= date_ten_micro;}
		Boolean Sup(ULONG date_ticks, ULONG date_ten_micro) 	{return  fTempoConverter.ConvertTickToMicroSec(date_ticks) > date_ten_micro;}
		Boolean InfEq(ULONG date_ticks, ULONG date_ten_micro) 	{return  fTempoConverter.ConvertTickToMicroSec(date_ticks) <= date_ten_micro;}
		Boolean Inf(ULONG date_ticks, ULONG date_ten_micro) 	{return  fTempoConverter.ConvertTickToMicroSec(date_ticks) < date_ten_micro;}
		
		Boolean SupEq(ULONG date_ticks, const TPos& pos) 	{return  fTimeConverter.SupEq(fTimeConverter.ConvertTickToBBU(date_ticks), pos);}
		Boolean Sup(ULONG date_ticks, const TPos& pos) 		{return  fTimeConverter.Sup(fTimeConverter.ConvertTickToBBU(date_ticks), pos);}
		Boolean InfEq(ULONG date_ticks, const TPos& pos) 	{return  fTimeConverter.InfEq(fTimeConverter.ConvertTickToBBU(date_ticks), pos);}
		Boolean Inf(ULONG date_ticks, const TPos& pos) 		{return  fTimeConverter.Inf(fTimeConverter.ConvertTickToBBU(date_ticks), pos);}
		
};

typedef TTempoMap FAR * TTempoMapPtr;


#endif
