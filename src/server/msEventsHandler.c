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
  
  modifications history:

*/

#include <stdio.h>

#include "msFunctions.h"

#include "msApplContext.h"
#include "msCommChans.h"
#include "msEventsHandler.h"
#include "msFCallHandler.h"
#include "msServerContext.h"
#include "msMutex.h"
#include "msLog.h"

/*__________________________________________________________________________*/
#define typeLastStdEvent 	typePortPrefix

/*__________________________________________________________________________*/
static MidiEvPtr NetStdEventHandler (MidiEvPtr e, CommunicationChan cc)
{
    MidiSend (RefNum(e), e);
    return 0;
}

/*__________________________________________________________________________*/
static MidiEvPtr NetUnexpected (MidiEvPtr e, CommunicationChan cc)
{
    LogWrite ("Unexpected net event received (type %d)", EvType(e));
    MidiFreeEv(e);
    return 0;
}

/*__________________________________________________________________________*/
static MidiEvPtr NetMidiConnect (MidiEvPtr e, CommunicationChan cc)
{
    if (Date(e) > MidiGetTime())
        MidiSend (RefNum(e), e);
    else FCMidiConnect (e);
    return 0;
}

/*__________________________________________________________________________*/
static MidiEvPtr NetMidiSetName (MidiEvPtr e, CommunicationChan cc)
{
    if (Date(e) > MidiGetTime())
        MidiSend (RefNum(e), e);
    else FCMidiSetName (e);
    return 0;
}

/*__________________________________________________________________________*/
static MidiEvPtr NetMidiSetInfo (MidiEvPtr e, CommunicationChan cc)
{
    if (Date(e) > MidiGetTime())
        MidiSend (RefNum(e), e);
    else FCMidiSetInfo (e);
    return 0;
}

/*__________________________________________________________________________*/
static MidiEvPtr NetMidiSetFilter (MidiEvPtr e, CommunicationChan cc)
{
    if (Date(e) > MidiGetTime())
        MidiSend (RefNum(e), e);
    else FCMidiSetFilter (e);
    return 0;
}

/*____________________________________________________________________________*/
static MidiEvPtr NetMidiOpen (MidiEvPtr e, CommunicationChan cc)
{
    char name[256]; short ref=0; msApplContextPtr context=0;
	msServerContext * sc = ServerContext;
	
	msMutexLock (sc->OCMutex);
printf ("%ld NetMidiOpen start...", MidiGetTime()); fflush(stdout);
	MidiEvPtr reply = MidiNewEv (typeMidiOpenRes);
	if (!reply) { 
		LogWrite ("NetMidiOpen: MidiShare memory allocation failed");
		goto err;
	}
	context = NewApplContext();
	if (!context) { 
		LogWrite ("NetMidiOpen: memory allocation failed\n");
		goto err;
	}
	Event2Text (e, name, 256);
	ref = MidiOpen (name);
	RefNum(reply) = (unsigned char)ref;
	if (ref > 0) {
		TApplPtr appl = GetAppl(ref);
		if (!appl) {
			LogWrite ("NetMidiOpen: unconsistent application state");
			goto err;
		}
		appl->netFlag = 1;	/* this flag indicates that this is a remote application */
							/* set to 0 by default, the application state is non-consistent */
							/* between the MidiOpen call and this step. It's however considered */
							/* that we can support this unconsistency as it only affects the way */
							/* tasks and alarms are handled by the kernel and none of them */
							/* can be scheduled before the call returned */
		context->chan = cc;
		context->filterh = 0;
		CCInc (cc);
		appl->context = context;
	}
printf ("opened %d (%ld)\n", ref, MidiGetTime()); fflush(stdout);
	msMutexUnlock (sc->OCMutex);
	return reply;

err:
	msMutexUnlock (sc->OCMutex);
	if (context) FreeApplContext(context);
	if (reply) MidiFreeEv(reply);
	if (ref > 0) MidiClose (ref);
	return 0;
}

/*____________________________________________________________________________*/
static MidiEvPtr NetMidiClose (MidiEvPtr e, CommunicationChan cc)
{
	MidiEvPtr reply = 0;
	short ref = RefNum(e);
	msApplContextPtr context = ApplContext(ref);
	msServerContext * sc = ServerContext;

	msMutexLock (sc->OCMutex);
printf ("%ld NetMidiClose start %d ... ", MidiGetTime(), ref);
	if (context && (context->chan == cc)) {
		if (context->filterh) msSharedMemClose(context->filterh);
		MidiClose (ref);
		FreeApplContext(context);
		CCDec (cc);
	}
	reply = MidiNewEv (typeMidiCommSync);
	if (!reply) {
		LogWrite ("NetMidiClose: MidiShare memory allocation failed");
	}
printf ("closed %d\n", ref);
	msMutexUnlock (sc->OCMutex);
	return reply;
}

/*__________________________________________________________________________*/
void InitNetEventsHandler (NetEventsHandlerTable table, int realtime)
{
  int i;

  for (i=typeNote;i<=typeLastStdEvent;i++)     table[i]= NetStdEventHandler;
  for (i=typeRcvAlarm;i<=typeApplAlarm;i++)    table[i]= NetUnexpected;
  table[typeMidiOpen]     = realtime ? NetUnexpected : NetMidiOpen;
  table[typeMidiOpenRes]  = NetUnexpected;
  table[typeMidiClose]    = realtime ? NetUnexpected : NetMidiClose;
  table[typeMidiConnect]  = NetMidiConnect;
  table[typeMidiSetName]  = NetMidiSetName;
  table[typeMidiSetInfo]  = NetMidiSetInfo;
  table[typeMidiSetFilter]= NetMidiSetFilter;

  for (i=typeReserved;i<=typeDead;i++)  table[i]= NetUnexpected;
}
