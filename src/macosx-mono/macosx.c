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
  
  modifications history:

*/
#include "msKernel.h"

#include "msExtern.h"
#include "msMem.h"
#include "msTasks.h"
#include "msPrefs.h"

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dlfcn.h"

#include "portaudio.h"

/*------------------------------------------------------------------------------*/
/* MacOSX specific resources          											*/

#define SAMPLE_RATE 44100
#define AUDIO_MS_INT (SAMPLE_RATE/100)

#define AUDIO_DEVICE "Built-in audio controller"

typedef struct MacOSXDriver MacOSXDriver, * MacOSXDriverPtr;

struct MacOSXDriver {
	MacOSXDriverPtr	next;
	void*		handle;
};

static MacOSXDriverPtr gMacOSXDriver = { 0 };
static PortAudioStream * gStream;
static long gFrames = 0;
static long gAudioSize = 0; // 10 * real size


MutexResCode msOpenMutex  (MutexRef ref) { return kSuccess; }
MutexResCode msCloseMutex (MutexRef ref) { return kSuccess; }
/*------------------------------------------------------------------------------*/
Boolean MSCompareAndSwap (FarPtr(void) *adr, FarPtr(void) compareTo, FarPtr(void) swapWith)
{
        *adr = swapWith;
        return true;
}

/*------------------------------------------------------------------------------*/
/*                      Drivers loading                     					*/
/*------------------------------------------------------------------------------*/

typedef Boolean (* Start) ();
typedef void (* Stop) ();

/*------------------------------------------------------------------------------*/
/*
	Launch the MidiShare install script.
*/
void CheckInstall()
{
	system("[ -x /Applications/MidiShare/.checkinstall ] && /Applications/MidiShare/.checkinstall");
}

/*------------------------------------------------------------------------------*/
void *  LoadLibrary( const char *filename, const char *symbol)
{
	void * handle = dlopen(filename,RTLD_LAZY);
	Start fun ;
	Boolean res;
	
	if (handle && (fun = (Start) dlsym(handle,symbol)) && (res = (*fun)())) {
		return handle;
	}else {
		Report("MidiShare", "can't load driver",filename);
		if (handle) dlclose(handle);
		return 0;
	}
}

/*------------------------------------------------------------------------------*/

void FreeLibrary(void * handle, const char *symbol)
{ 
	Stop fun;
	if (fun = (Stop) dlsym(handle,symbol)) (*fun)(); 
	dlclose(handle);
}


/*------------------------------------------------------------------------------*/
/*                      initializations : wakeup & sleep                        */
/*------------------------------------------------------------------------------*/
static Boolean LoadDriver (char *drvName) 
{
	   MacOSXDriverPtr mem = (MacOSXDriverPtr)AllocateMemory (kStdMemory, sizeof(MacOSXDriver));
        if (!mem) return false;
        
        mem->next = gMacOSXDriver;
        mem->handle = LoadLibrary (drvName,"_Start");
        
        if (mem->handle) {
            gMacOSXDriver = mem;
        }else {
            DisposeMemory (mem);
            return false;
        }
      
        return true;
}

/*------------------------------------------------------------------------------*/
void SpecialWakeUp (TMSGlobalPtr g) 
{
       unsigned short i, n;
       char str[256];
       
       CheckInstall();

	   n = CountDrivers ();
       for (i=0; i<n; i++) {
           if (GetDriver (i, str, 256))  LoadDriver (str);
       }
}


/*------------------------------------------------------------------------------*/
void SpecialSleep  (TMSGlobalPtr g)
{
        MacOSXDriverPtr next, drv = gMacOSXDriver;
        gMacOSXDriver = 0;
        while (drv) {
                next = drv->next;
                FreeLibrary (drv->handle,"_Stop");
                DisposeMemory (drv);
                drv = next;
        }
 }

/*------------------------------------------------------------------------------*/
/*                   client applications context and tasks                      */
/*------------------------------------------------------------------------------*/
TApplContextPtr CreateApplContext ()     { return 0; }
void  DisposeApplContext (TApplContextPtr context)    { }

/*_________________________________________________________________________*/
void CallApplAlarm (TApplContextPtr context, ApplAlarmPtr alarm, short refNum, long alarmCode)
{
     (*alarm) (refNum, alarmCode);     /* real alarm call */
}

/*_________________________________________________________________________*/
void CallRcvAlarm (TApplContextPtr context, RcvAlarmPtr alarm, short refNum)
{
    (*alarm) (refNum);                /* real alarm call */
 }

/*_________________________________________________________________________*/
void CallTaskCode  (TApplContextPtr context, MidiEvPtr e)
{
    TTaskExtPtr task = (TTaskExtPtr)LinkST(e);      /* event extension */
    (*task->fun)(Date(e), RefNum(e), task->arg1, task->arg2, task->arg3);
}

/*_________________________________________________________________________*/
void CallDTaskCode  (TApplContextPtr context, MidiEvPtr e)
{
        TTaskExtPtr task = (TTaskExtPtr)LinkST(e);      /* event extension */
        (*task->fun)(Date(e), RefNum(e), task->arg1, task->arg2, task->arg3);
}

/*_________________________________________________________________________*/
void DriverWakeUp (TApplPtr appl) 
{
        WakeupPtr wakeup = Wakeup(appl);
        if (wakeup) {
        	wakeup (appl->refNum);
        }
}

/*_________________________________________________________________________*/
void DriverSleep (TApplPtr appl)
{
        SleepPtr sleep = Sleep(appl);
        if (sleep) {
             sleep (appl->refNum);
        }
}

/*__________________________________________________________________________*/
/*      Interrupt handlers                                                                                                              */
/*__________________________________________________________________________*/

//____________________________________________________________

static int AudioClockHandler( void *inputBuffer, void *outputBuffer,
                             unsigned long framesPerBuffer,
                             PaTimestamp outTime, void *userData )
{
  	int i;
	
	gFrames += gAudioSize;
	for (i = 0 ; i < gFrames/AUDIO_MS_INT ; i++) {
		ClockHandler((TMSGlobalPtr)userData);
	}
	gFrames %= AUDIO_MS_INT;
	
 	return 0; 
}


/*_________________________________________________________________________*/
void OpenTimeInterrupts(TMSGlobalPtr g)
{
	PaError err;
	const PaDeviceInfo* info;
	int device;
    err = Pa_Initialize();
  	if( err != paNoError ) goto error_recovery;
  	
  	// Load audio sizr from the .ini file
  	gAudioSize = LoadBufferSize()*10;
  	
  	// Look for the internal built-in sound device
  	for (device= 0 ; device<Pa_CountDevices();device++) {
  		info = Pa_GetDeviceInfo(device);
  		if (strcmp (AUDIO_DEVICE,info->name) == 0) break;
  	}
  
	// Open Audio stream
    err = Pa_OpenStream(
				&gStream,
				paNoDevice,		/* default input device */
				0,         		/* no input */
				paFloat32, 	
				NULL,
				device, 			
				2,       		/* stereo output */
				paFloat32,     
				NULL,
				SAMPLE_RATE,
				gAudioSize/10,  /* frames per buffer */
				0,              /* number of buffers, if zero then use default minimum */
				paClipOff,      /* we won't output out of range samples so don't bother clipping them */
				AudioClockHandler,
				g );
 
	 if( err != paNoError ) goto error_recovery;
	 err = Pa_StartStream( gStream );
	 if( err != paNoError ) goto error_recovery;
	 return;
	 
error_recovery:
	 Report("MidiShare", "cannot open audio timer","");
	 CloseTimeInterrupts(g);
}

/*_________________________________________________________________________*/
void CloseTimeInterrupts(TMSGlobalPtr g)
{
	if(gStream) Pa_CloseStream(gStream);
	Pa_Terminate();
}


/*_________________________________________________________________________*/
Boolean ForgetTaskSync (MidiEvPtr * taskPtr, MidiEvPtr content)
{
	if (*taskPtr == content) {
      		EvType(content) = typeDead;
    		*taskPtr = 0;
    		return true;
	}
	return false; 
	// return CompareAndSwap (taskPtr, content, 0);
}


/*_________________________________________________________________________*/
/* memory allocation implementation                                        */
/*_________________________________________________________________________*/
FarPtr(void) AllocateMemory (MemoryType type, unsigned long size){ return (void*)malloc(size);}
void DisposeMemory  (FarPtr(void) memPtr) {if (memPtr) free(memPtr);}

