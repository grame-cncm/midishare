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


// ==============================
//	TEventReceiverInterface.h		
// ==============================


#ifndef __TEventReceiverInterface__
#define __TEventReceiverInterface__

#include "GenericMidiShare.h"

//-------------------------------
// Class TEventReceiverInterface 
//-------------------------------
/*!
 \brief Interface for an object which receive incoming events.
*/

class TEventReceiverInterface  {

	public:
	
		virtual ~TEventReceiverInterface(){}
		virtual void ReceiveEvents(MidiEvPtr e) = 0;
			
};

typedef TEventReceiverInterface FAR * TEventReceiverInterfacePtr;

//------------------------
// Class TEventDispatcher 
//------------------------
/*!
 \brief An object that implements the TEventReceiverInterface and a <B> Chain of responsability pattern </B>.
*/

class TEventDispatcher : public TEventReceiverInterface{

	private:
	
		TEventDispatcher* fSuccessor;

	public:
	
		TEventDispatcher() { fSuccessor = 0;}
		TEventDispatcher(TEventDispatcher* successor){ fSuccessor = successor;}
		virtual ~TEventDispatcher(){}
		
		void SetSuccessor(TEventDispatcher* successor){ fSuccessor = successor;}
	
		virtual void ReceiveEvents(MidiEvPtr e)
		{ 
			if (fSuccessor) { fSuccessor->ReceiveEvents(e); } else { MidiFreeEv(e);}
		}
		
};


typedef TEventDispatcher FAR * TEventDispatcherPtr;

#endif
