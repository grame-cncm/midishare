/*

  Copyright © Grame 1996-2004

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
//	TGenericPlayerInterface.h			    
// ===========================================================================

/*!
  \brief An interface for players.
*/


#ifndef __TGenericPlayerInterface__
#define __TGenericPlayerInterface__

//--------------------------------
// Class TGenericPlayerInterface 
//-------------------------------

class TGenericPlayerInterface {
	
	 public:
	 
	 	virtual ~TGenericPlayerInterface(){};
	 
	 	virtual void Start() = 0;
		virtual void Stop() = 0;
		virtual void Pause() = 0;
		virtual void Cont() = 0;
		
		virtual void PlaySliceForward() = 0;
		virtual void PlaySliceBackward() = 0;
	
		virtual void SetPosTicks (ULONG date_ticks) = 0;
		virtual void SetPosBBU (const TPos& pos) = 0;
		virtual void SetPosMs (ULONG date_ms) = 0;
			
		virtual ULONG GetPosTicks() = 0;
		
		virtual void  SetTempo (ULONG tempo) = 0;
		virtual ULONG GetTempo () = 0;
		
		virtual void RcvClock (ULONG date_ms) = 0;
		
		virtual void ReceiveEvents (MidiEvPtr e) = 0;
};

typedef TGenericPlayerInterface FAR * TGenericPlayerInterfacePtr;

#endif