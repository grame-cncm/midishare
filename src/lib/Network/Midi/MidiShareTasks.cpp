/*

  Copyright © Grame 2001
  Copyright © Mil Productions 2001

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
  grame@grame.fr

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
Boolean MidiShareTask::Schedule (long date, short refNum, long a1,long a2)
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
Boolean MidiShareDTask::Schedule (long date, short refNum, long a1,long a2)
{
#if defined(macintosh) && defined(MidiSharePPC_68k)
	fTask = MidiDTask (fUPPTask, date, refNum, a1, a2, (long)this);
#else
	fTask = MidiDTask (__MidiTask, date, refNum, a1, a2, (long)this);
#endif
	return fTask != 0;
}
