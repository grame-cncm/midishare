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


#include "MidiShare.h"
#include "msMidiDriver.h"
#include "msMidiInOut.h"

/*#include "msMMState.h" */
#include "FilterUtils.h"
#include "EventToMidiStream.h"
#include "MidiStreamToEvent.h"

#include <pthread.h>
#include <CoreFoundation/CFRunLoop.h>

static void Start();
static void Stop();

void Load_MidiDriver() {Start();}
void Dispose_MidiDriver() {Stop();}

static void MSALARMAPI msWakeup (short refnum);
static void MSALARMAPI msSleep  (short refnum);

short gRefNum = 0;
TDriverInfos drvInfos   = { "MacOSX Driver", kMidiDriverVersion, 0 };
TDriverOperation drvOps = { msWakeup, msSleep };
TFilter	gFilter;
LinearizeMthTbl gLinMethods  = { 0 };
ParseMethodTbl	gParseTbl  = { 0 };
Status2TypeTbl	gTypeTbl  = { 0 };

/* MacOSX Midi client */
MIDIClientRef 	gClient = NULL;
MIDIPortRef		gInPort = NULL;
MIDIPortRef		gOutPort = NULL;
static pthread_t 		gThread; 		// For NotificationProc 
static long 			gReenter;	    // Notification reentrancy counter
static Boolean 			gInit;

typedef void * ( * threadProcPtr) (void * ptr);

#define TIME_OUT 30  // 30 second


//____________________________________________________________
pthread_t create_thread (int priority, threadProcPtr proc)
{
	pthread_t thread;
	int  ret = pthread_create(&thread, NULL, proc, 0);
	if (!ret)
		return thread;	
	else 
		printf ("pthread_create failed: (%s)\n");
	return 0;	
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
		printf (" stopThread ..\n");
		pthread_cancel (thread);
		printf (" pthread_cancel ..\n");
		pthread_join (thread, &threadRet);
		printf (" pthread_join ..\n");
	}
}


//________________________________________________________________________________________
/*
void MyNotifyProc(const MIDINotification *message, void *refCon)
{
	int res;
	
	printf ("MyNotifyProc \n");
	
	if (message->messageID == kMIDIMsgSetupChanged){
		if (gRefNum > 0) {
		
			// Mutex for Slot list management : access by this thread and global thread
			gReenter++;
			res = pthread_mutex_trylock(&gMutex);
			
			// In case of reentrancy
			if (res == EBUSY){
				printf ("LOCK BUSY \n");
				 return;
			}
			//do {
				printf ("gReenter %ld\n",gReenter);
				RemoveSlots(gRefNum);
				AddSlots (gRefNum);
			//}while(gReenter--);
			
			res =  pthread_mutex_unlock(&gMutex);
		}
	}
}
*/

void MyNotifyProc(const MIDINotification *message, void *refCon)
{
	printf ("MyNotifyProc \n");
	
	if (message->messageID == kMIDIMsgSetupChanged) {
		if (gRefNum > 0 ) {
		
			// Mutex for Slot list management : access by this thread and global thread
			gReenter++;
			if (!gInit) {
				gInit = true;
				
				printf ("gReenter %ld\n",gReenter);
				do {
					RemoveSlots(gRefNum);
					AddSlots (gRefNum);
				}while(gReenter--);
				
				gInit = false;
			}
		}
	}
}

//________________________________________________________________________________________
static Boolean InitMidiClient ()
{
	OSStatus err;
	//set_cancel();

	err = MIDIClientCreate(CFSTR("MidiShare"), MyNotifyProc, NULL, &gClient);
	//printf("MIDIClientCreate OK\n");
	if (!gClient) {
		printf("Can not open Midi client\n");
		goto error;
	}
	
	err = MIDIInputPortCreate(gClient, CFSTR("Input port"), MyReadProc, NULL, &gInPort);
	//printf("MIDIInputPortCreate OK\n");
	
	if (!gInPort) {
		printf("Can not open Midi in port\n");
		goto error;
	}
	
	err = MIDIOutputPortCreate(gClient, CFSTR("Output port"), &gOutPort);
	//printf("MIDIOutputPortCreate OK\n");
	
	if (!gOutPort) {
		printf("Can not open Midi out port\n");
		goto error;
	}
	
	return true;
	
error :
	if (gInPort) MIDIPortDispose(gInPort);
	if (gOutPort) MIDIPortDispose(gOutPort);
	if (gClient) MIDIClientDispose(gClient);
	return false;
}

//________________________________________________________________________________________
static void * MidiThread (void * ptr)
{
	if (InitMidiClient()) 	
		CFRunLoopRun();
}



//_________________________________________________________
static void Start()
{
	//printf("Start DRIVER\n");
	gRefNum = MidiRegisterDriver (&drvInfos, &drvOps);
}

//_________________________________________________________
static void Stop()
{
	//printf("Stop DRIVER\n");
	if (gRefNum > 0) MidiUnregisterDriver (gRefNum);
	gRefNum = -1;
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

//_________________________________________________________
static void msWakeup (short refnum)
{
	int i,res;
	
	gThread = create_thread(0,MidiThread);
	gReenter = -1;
	
	// Wait for the MIDI thread to open client and ports.
	for (i = 0 ; i<TIME_OUT; i++) {
	  	//printf("WAIT for MIDI Opening\n");
		if (gOutPort) break;
		sleep(1);
	}
	
	if (!gOutPort) {
		printf("ERROR TIME OUT\n");
		goto error;
	}
	
	MidiSetRcvAlarm (refnum, RcvAlarm);
	MidiSetApplAlarm (refnum, ApplAlarm);
    SetupFilter (&gFilter);
    MidiSetFilter (refnum, &gFilter);	
	MidiConnect (MidiShareDrvRef, refnum, true);
	MidiConnect (refnum, MidiShareDrvRef, true);
	MidiStreamInitMthTbl (gLinMethods);
	MidiParseInitMthTbl	 (gParseTbl);
	MidiParseInitTypeTbl (gTypeTbl);
	
	// Mutex for Slot list management : access by this thread and NotificationProc
	gInit = true;
	RemoveSlots(gRefNum);
	AddSlots (refnum);
	gInit = false;
	
	return;
	
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
}

static void msSleep (short refnum)
{
	/*SaveState (gInSlots, gOutSlots);*/
	
	//printf("msSleep\n");
	//stopThread(gThread); // NE MARCHE PAS : A VOIR
	
	if (gInPort) MIDIPortDispose(gInPort);
	if (gOutPort) MIDIPortDispose(gOutPort);
	if (gClient) MIDIClientDispose(gClient);
	
	RemoveSlots (refnum);
}

