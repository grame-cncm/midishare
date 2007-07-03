/*

  Copyright © Grame 2002

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

#include "msMidiDriver.h"
#include "msMidiInOut.h"

#include "msMidiState.h" 
#include "FilterUtils.h"
#include "EventToMidiStream.h"
#include "MidiStreamToEvent.h"

#include <pthread.h>
#include <unistd.h>
#include <CoreFoundation/CFRunLoop.h>

#define profileName "msMidiDriver.ini"

#define PRINT(x) { printf x; fflush(stdout); }
#define DBUG(x)     /*PRINT(x) */

static char * fullProfileName = 0;

Boolean Start();
void Stop();

static void msWakeup (short refnum);
static void msSleep  (short refnum);

short gRefNum = 0;
TDriverInfos drvInfos   = { "MacOSX Driver", kMidiDriverVersion, 0 };
TDriverOperation drvOps = { msWakeup, msSleep };
TFilter	gFilter;
LinearizeMthTbl gLinMethods  = { 0 };
ParseMethodTbl	gParseTbl  = { 0 };
Status2TypeTbl	gTypeTbl  = { 0 };

/* MacOSX Midi client */
MIDIClientRef   gClient = NULL;
MIDIPortRef		gInPort = NULL;
MIDIPortRef		gOutPort = NULL;
static pthread_t 	gThread; 	// For NotificationProc 
static long 		gReenter;	// Notification reentrancy counter
static Boolean 		gInit;

typedef void * (* threadProcPtr) (void * ptr);

//____________________________________________________________
pthread_t create_thread (int priority, threadProcPtr proc)
{
	pthread_t thread;
	int  ret = pthread_create(&thread, NULL, proc, 0);
	if (!ret){
		return thread;	
	}else{
		return 0;	
	}
}

//____________________________________________________________
void set_cancel ()
{
	int old;
	pthread_setcancelstate (PTHREAD_CANCEL_ENABLE, &old);
	pthread_setcanceltype (PTHREAD_CANCEL_DEFERRED, &old);
}

//____________________________________________________________
void stopThread (pthread_t thread)
{
	void *threadRet; 
	if (thread) {
		pthread_cancel (thread);
		pthread_join (thread, &threadRet);
	}
}

//_________________________________________________________
static void SetupFilter (MidiFilterPtr filter)
{
	short i;
	for (i = 0; i<256; i++) {                                                                               
		RejectBit(filter->evType,i);
		AcceptBit(filter->port,i);
	}
	for (i = 0; i<16; i++)
		AcceptBit(filter->channel,i);               
	for (i = 0; i<typeProcess; i++)
		AcceptBit(filter->evType,i);      
	for (i = typeQuarterFrame; i<=typeRegParam; i++)
		AcceptBit(filter->evType,i);
}

//________________________________________________________________________________________
void NotifyProc(const MIDINotification *message, void *refCon)
{
	if (message->messageID == kMIDIMsgSetupChanged) {
	
		if (gRefNum > 0 ) {
		
			// Mutex for Slot list management : access by this thread and global thread
			gReenter++;
			if (!gInit) {
				gInit = true;
				do {
					RemoveSlots(gRefNum);
					AddSlots (gRefNum);
				}while(gReenter--);
					LoadState (gInSlots, gOutSlots,fullProfileName);  // To be checked
					gInit = false;
			}
		}
	}
}

//________________________________________________________________________________________
static void * OpenThread (void * ptr)
{
	OSStatus err;
	fullProfileName = GetProfileFullName (profileName);
        
	gInit = true; // Start initialisation
	//pthread_testcancel();
	
	DBUG(("MIDIClientCreate \n"));
  	err = MIDIClientCreate(CFSTR("MidiShare"), NotifyProc, NULL, &gClient);
   	if (!gClient) {
		printf("Can not open Midi client\n");
		goto error;
	}
	DBUG(("MIDIClientCreate OK\n"));
        
	DBUG(("MIDIInputPortCreate\n"));
   	err = MIDIInputPortCreate(gClient, CFSTR("Input port"), ReadProc, NULL, &gInPort);
   	if (!gInPort) {
		printf("Can not open Midi in port\n");
		goto error;
	}
	DBUG(("MIDIInputPortCreate OK\n"));
        
	DBUG(("MIDIOutputPortCreate \n"));
	err = MIDIOutputPortCreate(gClient, CFSTR("Output port"), &gOutPort);
	if (!gOutPort) {
		printf("Can not open Midi out port\n");
		goto error;
	}
	DBUG(("MIDIOutputPortCreate OK\n"));
	
	MidiSetRcvAlarm (gRefNum, RcvAlarm);
	MidiSetApplAlarm (gRefNum, ApplAlarm);
	SetupFilter (&gFilter);
	MidiSetFilter (gRefNum, &gFilter);	
	MidiConnect (MidiShareDrvRef, gRefNum, true);
	MidiConnect (gRefNum, MidiShareDrvRef, true);
	MidiStreamInitMthTbl (gLinMethods);
	MidiParseInitMthTbl  (gParseTbl);
	MidiParseInitTypeTbl (gTypeTbl);
	
	RemoveSlots(gRefNum);
	AddSlots (gRefNum);
        
	DBUG(("Init OK\n"));
   	LoadState (gInSlots, gOutSlots,fullProfileName); 
	DBUG(("LoadState OK\n"));
	
	gReenter = -1;
	gInit = false; // End of initialisation
        
	CFRunLoopRun();
	return 0;
	
error :
	if (gInPort){
		 MIDIPortDispose(gInPort);
		 gInPort = NULL;
	}
	if (gOutPort) {
		MIDIPortDispose(gOutPort);
		gInPort = NULL;
	}
	if (gClient) {
		MIDIClientDispose(gClient);
		gClient = NULL;
	}
	return 0;
}

//_________________________________________________________
Boolean Start()
{
	gRefNum = MidiRegisterDriver (&drvInfos, &drvOps);
	return (gRefNum > 0);
}

//_________________________________________________________
void Stop()
{
	if (gRefNum > 0) MidiUnregisterDriver (gRefNum);
	gRefNum = -1;
}

//_________________________________________________________
static void msWakeup (short refnum)
{
	DBUG(("msWakeup \n"));
	gThread = create_thread(0,OpenThread);
	DBUG(("msWakeup OK\n"));
}

//_________________________________________________________
static void msSleep (short refnum)
{
	SaveState (gInSlots, gOutSlots, fullProfileName);  
	DBUG(("msSleep \n"));
	
	//stopThread(gThread); // Does not work because of pthread_cancel
	
	if (gInPort) MIDIPortDispose(gInPort);
	if (gOutPort) MIDIPortDispose(gOutPort);
	if (gClient) MIDIClientDispose(gClient);
	gClient = NULL;
	gInPort = NULL;
	gOutPort = NULL;
	RemoveSlots (refnum);
	/* when sysex are still sent, the completion routine may still be called... wait 1 sec 
	hoping the CompletionProc will see the null gClient.... */
	usleep(100000);
}

