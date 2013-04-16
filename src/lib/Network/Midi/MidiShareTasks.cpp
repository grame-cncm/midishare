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

#include "MidiShareTasks.h"

//_______________________________________________________________________________
static msCallback __MidiTask ( long date, short refNum, long a1,long a2,long a3 )
{
	MidiShareTaskPtr task = (MidiShareTaskPtr)a3;
	task->fTask = 0;
	task->Run (date, refNum, a1, a2);
}

//_______________________________________________________________________________
MidiShareTask::MidiShareTask()
{
	fTask = 0;
#if defined(macintosh) && defined(MidiSharePPC_68k)
	fUPPTask = NewTaskPtr(__MidiTask);
#endif
}

//_______________________________________________________________________________
MidiShareTask::~MidiShareTask()
{
	Forget();
#if defined(macintosh) && defined(MidiSharePPC_68k)
	if (fUPPTask) DisposeRoutineDescriptor(fUPPTask);
#endif
}

//_______________________________________________________________________________
bool MidiShareTask::Schedule (long date, short refNum, long a1,long a2)
{
#if defined(macintosh) && defined(MidiSharePPC_68k)
	fTask = MidiTask (fUPPTask, date, refNum, a1, a2, (long)this);
#else
	fTask = MidiTask (__MidiTask, date, refNum, a1, a2, (long)this);
#endif
	return fTask != 0;
}

//_______________________________________________________________________________
void MidiShareTask::Forget (void)
{
	if (fTask) MidiForgetTask (&fTask);
}

//_______________________________________________________________________________
bool MidiShareDTask::Schedule (long date, short refNum, long a1,long a2)
{
#if defined(macintosh) && defined(MidiSharePPC_68k)
	fTask = MidiDTask (fUPPTask, date, refNum, a1, a2, (long)this);
#else
	fTask = MidiDTask (__MidiTask, date, refNum, a1, a2, (long)this);
#endif
	return fTask != 0;
}
