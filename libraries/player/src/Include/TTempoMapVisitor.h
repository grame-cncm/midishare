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
//	TTempoMapVisitor.h		    
// ===========================================================================

#ifndef __TTempoMapVisitor__
#define __TTempoMapVisitor__

#include "TEvent.h"
#include "TScoreVisitorInterface.h"
#include "TTempoMap.h"

//------------------------
// Class TTempoMapVisitor 
//------------------------
/*! 
  \brief A score visitor which uses a TTempoMap object to maintain the current Tempo and TimeSign state.
*/

class TTempoMapVisitor : public TScoreVisitorInterface, public TTempoMap {

	public :
 
 		TTempoMapVisitor(ULONG tpq):TTempoMap(tpq){}
 		virtual ~TTempoMapVisitor (){}
 		
 		void Visite (TTempoPtr ev,Boolean forward);
		void Visite (TTimeSignPtr ev,Boolean forward); 
};

typedef TTempoMapVisitor FAR * TTempoMapVisitorPtr;

#endif
