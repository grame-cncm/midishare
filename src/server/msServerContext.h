/*
  Copyright © Grame 2003

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

#ifndef __msServerContext__
#define __msServerContext__

#include "lffifo.h"
#include "msAppls.h"
#include "msKernel.h"
#include "msMem.h"
#include "EventToStream.h"
#include "StreamToEvent.h"

#include "msCommChans.h"
#include "msKernelPrefs.h"
#include "msSharedMem.h"
#include "msThreads.h"

typedef struct {
	void * 				sharedmem;				/* ptr to the shared memory segment */
	msThreadPtr 		meetingPointThread;		/* meeting point management thread  */
	MeetingPointChan 	meetingPoint;			/* meeting point handler			*/
	msKernelPrefs *		prefs;					/* the server preferences			*/

	/* methods tables for the streams - shared between all the streams */
    msStreamParseMethodTbl 	parseMthTable;
    msStreamMthTbl			streamMthTable;
} msServerContext, *msServerContextPtr;

#define ServerContext 	(msServerContextPtr)gMem->context

#ifdef __cplusplus
extern "C" {
#endif

extern TMSGlobalPtr gMem;

#ifdef __cplusplus
}
#endif


#endif
