/*

  Copyright © Grame 1996-2006

  This library is free software; you can redistribute it and modify it under 
  the terms of the GNU Library General Public License as published by the 
  Free Software Foundation version 2 of the License, or any later version.

  This library is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public License 
  for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

  Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
  research@grame.fr

*/

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
 		virtual ~TTempoMap() {}
 		
 		void Init ();
 		
 		ULONG CurDateTicks() {return fCurdate_ticks;}
		ULONG CurDateMicro() {return fCur_date_ten_micro;}
		ULONG CurDateMs() {return fCur_date_ten_micro/100;}
		const TPos CurDateBBU() {return fCur_pos;}
		
		short GetNum()     {return fTimeConverter.GetNum();}
		short GetDenom()   {return fTimeConverter.GetDenom();}
		short GetNClocks()  {return fTimeConverter.GetNClocks();}
		short GetN32nd()    {return fTimeConverter.GetN32nd();}
		
		void SetNum(short val)     {fTimeConverter.SetNum(val);}
		void SetDenom(short val)   {fTimeConverter.SetDenom(val);}
		void SetNClocks(short val)  {fTimeConverter.SetNClocks(val);}
		void SetN32nd(short val)    {fTimeConverter.SetN32nd(val);}
			
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
