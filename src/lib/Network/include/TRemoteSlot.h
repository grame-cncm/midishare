/*
  MidiShare Project
  Copyright (C) Grame 2001
  Copyright (C) Mil Productions 2001

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/


#ifndef __TMidiRemoteSlot__
#define __TMidiRemoteSlot__

#include "MidiShareAppl.h"
#include "TMidiSlot.h"
#include "StatesManager.h"

//____________________________________________________________
class TRemoteSlot : public MidiShareAppl
{
	public:
			 	 TRemoteSlot (IPNum id, short drvRef);
		virtual ~TRemoteSlot () { Close(); }
	
				bool	Open (MidiName name, bool wantFilter=false);
				void 	Close ();

				MidiName 	GetName ();
				void 		Send 	(MidiEvPtr e);
				MidiEvPtr 	GetEv 	();
				SlotRefNum 	InRef () 	{ return fInSlot.GetRefNum(); }
				SlotRefNum 	OutRef () 	{ return fOutSlot.GetRefNum(); }
				short 		InSlotRef ()  { return fInSlot.SlotRef(); }
				short 		OutSlotRef () { return fOutSlot.SlotRef(); }
				void 		Put 	(MidiEvPtr e)  { MidiAddSeq (fSeq, e); }

	private:
		IPNum		fID;
		TMidiSlot	fInSlot, fOutSlot;
		MidiSeqPtr	fSeq;
		StatesManager  	fStateManager;
		SlotState *		fSlotsState;	
};

#endif
