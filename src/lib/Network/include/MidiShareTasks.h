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


#ifndef __MidiShareTasks__
#define __MidiShareTasks__

#include "MidiShare.h"


#if macintosh
#define msCallback pascal void
#elif WIN32
#define msCallback void MSALARMAPI
#else 
#define msCallback void
#endif

typedef class MidiShareTask * MidiShareTaskPtr;

/*_______________________________________________________________________________*/
class MidiShareTask
{
	public:
				 MidiShareTask ();
		virtual ~MidiShareTask ();
		virtual bool Schedule	(long date, short refNum, long a1,long a2);
		virtual void 	Run			(long date, short refNum, long a1,long a2) = 0;
		virtual void 	Forget 		(void);
		virtual bool Busy 		(void)		{ return fTask != 0; }
				void	Clear 		(void)		{ fTask = 0; }

		MidiEvPtr	fTask;
#if defined(macintosh) && defined(MidiSharePPC_68k)
		UPPTaskPtr	fUPPTask;
#endif
};

/*_______________________________________________________________________________*/
class MidiShareDTask : public MidiShareTask
{
	public:
		virtual bool Schedule	(long date, short refNum, long a1,long a2);
};

#endif
