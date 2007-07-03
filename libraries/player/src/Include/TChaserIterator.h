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
//	TChaserIterator.h			    
// ===========================================================================

#ifndef __TChaserIterator__
#define __TChaserIterator__
 
#include "TChaserVisitor.h"
#include "TScoreIterator.h"

//------------------------
// Class TChaserInterface
//------------------------
/*!
	\brief Interface for chase objects.
*/ 

class TChaserInterface {

	public:
	
		virtual ~TChaserInterface() {}
	
		virtual void Init() = 0;
		
		virtual void  ChaseOn (ULONG date_ticks) = 0;
		virtual void  ChaseOff (ULONG date_ticks) = 0;
			
		virtual ULONG GetPosTicks () = 0;
};

typedef TChaserInterface FAR * TChaserInterfacePtr;


//-----------------------
// Class TChaserIterator
//-----------------------
/*!
	\brief The chaser iterator use a chaser visitor to keep the state of KeyOn, CtrlChange, 
    ProgChange, KeyPress, ChanPress, PitchBend, Sysex and Tune events when moving 
    along the score.
    The chaser iterator is used when starting or stopping the Player and will send 
    events that occurred previously in the tracks.
*/ 

class TChaserIterator :public TChaserInterface {

	private:
	
		TChaserVisitor 	fChaser;   		
		TScoreIterator 	fIterator;
		
		ULONG 			fCurdate_ticks;
		
		void SetPosTicks (ULONG date_ticks);
		void GoBeginDateTicks (ULONG date_ticks);
			
	public:
	
		TChaserIterator(TScorePtr score, TEventSenderInterfacePtr user):fChaser(user),fIterator(score) {}
		virtual ~TChaserIterator() {}
	
		void Init();
		
		void  ChaseOn (ULONG date_ticks);
		void  ChaseOff (ULONG date_ticks);
			
		ULONG GetPosTicks () {return fCurdate_ticks;}
};

typedef TChaserIterator FAR * TChaserIteratorPtr;


#endif
