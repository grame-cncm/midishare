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
//	TScoreIterator.cpp		    
// ===========================================================================

#ifndef __TScoreIterator__
#define __TScoreIterator__

#include "TScore.h"

//----------------------
// Class TScoreIterator 
//----------------------
/*!
\brief  An iterator on a TScore object.
*/

class TScoreIterator {

	friend class TScore;
	
	private:
	 
		void SetPosTicksForward (ULONG date_ticks);
		void SetPosTicksBackward (ULONG date_ticks);
	
	protected:
	
		TScorePtr 		fScore;     // score as a RING  
		TEventPtr 		fCur;
		ULONG 			fCurDate;
		Boolean     	fFirst;
		Boolean     	fLast;
		TScoreIterator* fNextIt;
		
		void ItemsInserted(TEventPtr ev);
		void ItemsRemoved(TEventPtr ev);
	
	public:	
	
		TScoreIterator ();
		TScoreIterator (TScorePtr score);
		TScoreIterator (TScorePtr score,TEventPtr cur);
		
		virtual ~TScoreIterator();
		
		void			SetNextIterator(TScoreIterator* inIt){ fNextIt = inIt;}
		TScoreIterator*	GetNextIterator(){return fNextIt;}
		
		void Init();
		void Init(TEventPtr cur);
		
		void Detach();
		
		TEventPtr 	CurEv();
		MidiEvPtr 	CurMidiEv();
		ULONG 		CurDate() ;
		Boolean 	IsFirstEv();
		Boolean 	IsLastEv();
		
		TEventPtr NextEv();
		TEventPtr PrevEv();
		TEventPtr NextDateEv();
		TEventPtr PrevDateEv();
		
		TEventPtr SetPosTicks (ULONG date_ticks);
};

typedef class TScoreIterator  FAR * TScoreIteratorPtr;

//-----------------------
// Class TScoreIterator1 
//-----------------------
/*!
\brief  An iterator on a TScore object that will not be attached to the score.
*/

class TScoreIterator1 :	public TScoreIterator {
		
		public:
		
			virtual ~TScoreIterator1(){}
		
			TScoreIterator1 (TScorePtr score) 
			{
				fScore = score; 
				fNextIt = 0;
				Init();
			}

			TScoreIterator1 (TScorePtr score,TEventPtr cur) 
			{ 
				fScore = score;
				fNextIt = 0;
				Init(cur);
			}
};
#endif

