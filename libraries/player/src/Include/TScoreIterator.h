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
//	TScoreIterator.cpp		    
// ===========================================================================
/*!
\brief  An iterator on a TScore object.
*/


#ifndef __TScoreIterator__
#define __TScoreIterator__


#include "TScore.h"

//-----------------------
// Class TScoreIterator 
//-----------------------

class TScoreIterator {

	friend class TScore;
	
	private :
	 
		void SetPosTicksForward (ULONG date_ticks);
		void SetPosTicksBackward (ULONG date_ticks);
	
	protected :
	
		TScorePtr 		fScore;     // score as a RING  
		TEventPtr 		fCur;
		ULONG 			fCurDate;
		Boolean     	fFirst;
		Boolean     	fLast;
		TScoreIterator* fNextIt;
		
		void ItemsInserted(TEventPtr ev);
		void ItemsRemoved(TEventPtr ev);
	
	public :	
	
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


class TScoreIterator1 :public TScoreIterator {
		
		public:
		
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

