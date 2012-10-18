/*

  Copyright (C) Grame 1996-2012

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
//	TScore.h			    
// ===========================================================================

#ifndef __TScore__
#define __TScore__

#include "TEvent.h"

//--------------
// Class TScore 
//--------------
/*!
 \brief A double linked list of TEvent objects
*/

class TScoreIterator;

class TScore {
	
	friend class TScoreIterator;

	protected:
	
		TEventPtr 		fLast;
		TScoreIterator* fHeadtIt;
		
		void ItemsInserted(TEventPtr ev);
		void ItemsRemoved(TEventPtr ev);
		
		void DetachIterators();
		void AttachIterators();
		
		void AttachIterator(TScoreIterator* it);
		void DetachIterator(TScoreIterator* it);
		
	public:
	
		TScore():fLast(0),fHeadtIt(0) {}
		virtual ~TScore() {DetachIterators(); Clear();}
		
		void Clear();
		
		virtual TEventPtr FirstEvent(); 
		virtual TEventPtr LastEvent(); 
		
		virtual void InsertAtBeginEvent(TEventPtr ev);
		virtual void InsertAtEndEvent(TEventPtr ev);
		
		virtual void InsertAfterEvent(TEventPtr ev1,TEventPtr ev2);
		virtual void InsertBeforeEvent(TEventPtr ev1,TEventPtr ev2);
		
		virtual void RemoveEvent(TEventPtr ev);
		
		ULONG Length();
};

typedef class TScore  FAR * TScorePtr;

#endif
