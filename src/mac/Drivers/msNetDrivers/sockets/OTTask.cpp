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


#include <OpenTransport.h>
#include "OTTasks.h"
#include "MidiShare.h"
#include "MidiTools.h"

#ifdef __cplusplus
extern "C" {
#endif

pascal long		OTCreateTimerTask(OTProcessProcPtr proc, void* arg);
pascal void		OTDestroyTimerTask(long task);
pascal Boolean	OTScheduleTimerTask(long task, OTTimeout mls);
pascal Boolean	OTCancelTimerTask(long task);

#ifdef __cplusplus
}
#endif

//____________________________________________________________
static pascal void __OTTimerTask (void * arg)
{
	OTTask * task = (OTTask *)arg;
	task->Run (task->fDate, task->fRefNum, task->fArg1, task->fArg2);
}

//____________________________________________________________
OTTask::OTTask ()
{
	fRefNum = -1;
	fArg1 = fArg2 = 0;
	fOTTask = OTCreateTimerTask (__OTTimerTask, this);
}

//____________________________________________________________
OTTask::~OTTask ()
{
	Forget ();
	if (fOTTask) OTDestroyTimerTask (fOTTask);
}

//_______________________________________________________________________________
Boolean OTTask::Schedule (long date, short refNum, long a1,long a2)
{
	if (fOTTask) {
		long delay = date - MidiGetTime();
		if (delay < 0) {
			delay = 0;
		}
		fRefNum = refNum;
		fArg1 = a1;
		fArg2 = a2;
		fDate = date;
		return OTScheduleTimerTask (fOTTask, delay);
	}
	return false;
}

//____________________________________________________________
void OTTask::Forget (void)
{
	if (fOTTask) OTCancelTimerTask (fOTTask);
}

