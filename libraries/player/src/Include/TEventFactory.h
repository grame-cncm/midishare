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
//	TEventFactory.h			    
// ===========================================================================

#ifndef __TEventFactory__
#define __TEventFactory__

#include "TEvent.h"


//---------------------
// Class TEventFactory 
//---------------------
/*!
	\brief A TEventFactory object allows to build TEvent objects using the MidiShare event type.
*/

typedef  TEventPtr  (*mthptr)(MidiEvPtr e);


class TEventFactory {

	private:
		
		mthptr fBuildTable[256];
		static TEventFactory* fInstance;
	
	public: 
		
		TEventFactory();
		virtual ~TEventFactory(){}
		
		static TEventPtr GenericCreateEvent(MidiEvPtr e) {return fInstance->fBuildTable[EvType(e)](e);}
		static void Init() {if (fInstance == 0) fInstance = new TEventFactory();}
		static void Destroy() 
		{
			if (fInstance){ 
				delete(fInstance);
			 	fInstance = 0;
			}
		}
};

typedef TEventFactory* TEventFactoryPtr;

#endif
