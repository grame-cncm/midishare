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


#ifndef __MidiShareDriver__
#define __MidiShareDriver__

#include "MidiShareAppl.h"

#define MidiShareDrvRef		127
/*_______________________________________________________________________________*/
class MidiShareDriver : public MidiShareAppl
{
	public:		
				 MidiShareDriver (short version);
		virtual ~MidiShareDriver ();

		virtual bool Open 	(MidiName name, bool wantFilter = false);
		virtual void 	Close	();
		virtual void	WakeUp (short r) = 0;
		virtual void	Sleep  (short r) = 0;

	protected:
		short fVersion;
	private:	
#if defined(macintosh) && defined(MidiSharePPC_68k)
		UPPWakeupProcPtr	fUPPWakeup;
		UPPSleepProcPtr 	fUPPSleep;
#endif
};

#endif
