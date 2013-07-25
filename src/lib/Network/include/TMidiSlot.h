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


#ifndef __TMidiSlot__
#define __TMidiSlot__

#include "MidiShare.h"

/*_______________________________________________________________________________*/
class TMidiSlot
{
	public:		
				 TMidiSlot (short drvref);
		virtual ~TMidiSlot ();

		virtual bool Open 	(MidiName name, SlotDirection dir = MidiInputOutputSlot);
		virtual void 	Close	();
				bool Opened 	();
				MidiName GetName();

				short 		DrvRef ()		{ return fRefNum.drvRef; }
				short 		SlotRef ()		{ return Slot(fRefNum); }
				SlotRefNum 	GetRefNum ()	{ return fRefNum; }

	protected:
		enum { undefinedRefNum = -1 };
		SlotRefNum	fRefNum;
		short		fDrvRef;
		TSlotInfos  fInfos;
};

/*_______________________________________________________________________________*/
inline bool TMidiSlot::Opened()	{ return fRefNum.slotRef != undefinedRefNum; }
#endif

