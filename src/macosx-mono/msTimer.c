/*

  Copyright © Grame 1999

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

#include "msKernel.h"
#include "msPrefs.h"
#include "msTimeImpl.h"

#include <CoreServices/CoreServices.h>

extern TMSGlobalPtr gMem;
static     TMTask   gTask;                 /* Time Manager task record    */
static unsigned long gTimeRes;

/*__________________________________________________________________________*/
static void timeManagerTask(TMTaskPtr tmTaskPtr) 
{
	PrimeTimeTask((QElemPtr)&gTask, gTimeRes);
	unsigned long c = gTimeRes;
	while (c--)
		ClockHandler(gMem); 
}

/*__________________________________________________________________________*/
/* time interrrupt management                                               */
/*__________________________________________________________________________*/
void OpenTimer (TMSGlobalPtr g)
{
	OSErr err;
	gTimeRes = LoadTimeRes();
	if (gTimeRes < 1) gTimeRes = 1;
	
	gTask.tmAddr = NewTimerUPP(timeManagerTask);
	gTask.tmWakeUp = 0;
	gTask.tmReserved = 0;
	InstallXTimeTask ((QElemPtr)&gTask);
	err = PrimeTimeTask((QElemPtr)&gTask, gTimeRes);
    ReportN("MidiShare", "open time interrupt using a timer - timeresolution is", gTimeRes);
}

void CloseTimer (TMSGlobalPtr g)
{
	QElemPtr task = (QElemPtr)&gTask;
	RemoveTimeTask(task);
	DisposeTimerUPP (gTask.tmAddr);
}

