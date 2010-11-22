/*

  Copyright � Grame 2002

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
#include "msTimeImpl.h"

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <pthread.h>

#ifdef __msBench__
#include "benchs.h"
#endif

/*------------------------------------------------------------------------------*/
/* MacOSX specific resources          						*/
typedef struct MacOSXDriver MacOSXDriver, * MacOSXDriverPtr;

struct MacOSXDriver {
	MacOSXDriverPtr	next;
	void*		handle;
};

typedef struct {
    pthread_mutex_t mutex;
    int             initialized;
} TMutex;

static TMutex gMutex[kMutexCount]; // = { 0 };
static MacOSXDriverPtr gMacOSXDriver; // = { 0 };
static unsigned long gTimeMode = 0;

/*------------------------------------------------------------------------------*/
void msOpenMutex  (unsigned int mutex)
{
    if (mutex < kMutexCount) {
        if (!gMutex[mutex].initialized) {
            pthread_mutex_init (&gMutex[mutex].mutex, 0);
            gMutex[mutex].initialized = 1;
        }
        pthread_mutex_lock (&gMutex[mutex].mutex);
    }
}

/*------------------------------------------------------------------------------*/
void msCloseMutex (unsigned int mutex)
{
    if (mutex < kMutexCount) {
        if (gMutex[mutex].initialized)
            pthread_mutex_unlock (&gMutex[mutex].mutex);
	}
}


/*------------------------------------------------------------------------------*/
Boolean MSCompareAndSwap (void** adr, void* compareTo, void* swapWith)
{
	*adr = swapWith;
	return true;
}

/*------------------------------------------------------------------------------*/
/*                      Drivers loading                     			*/
/*------------------------------------------------------------------------------*/

typedef Boolean (* Start) ();
typedef void (* Stop) ();

/*------------------------------------------------------------------------------*/
/*
	Launch the MidiShare install script.
*/
void CheckInstall()
{
	system("[ -x /private/etc/MidiShare/checkinstall ] && /private/etc/MidiShare/checkinstall");
}

/*------------------------------------------------------------------------------*/
void *  LoadLibrary(const char *filename, const char *symbol)
{
	void * handle = dlopen(filename, RTLD_LAZY);
	Start fun ;
        
	if (!handle) { 
		Report("MidiShare", "can't load driver", filename);
		return 0;
	} 

	fun = (Start)dlsym(handle,symbol);
//	if ((fun = (Start)dlsym(handle,symbol)) && (res = (*fun)())) {
	if (!fun) {
		Report("MidiShare", "can't find driver entry point", filename);
		dlclose(handle);
		return 0;
	} 
	
	if (!(*fun)()) {
		Report("MidiShare", "can't start driver", filename);
		dlclose(handle);
		return 0;
	} 

	return handle;
}

/*------------------------------------------------------------------------------*/
void FreeLibrary(void * handle, const char *symbol)
{ 
	Stop fun = (Stop)dlsym(handle, symbol);
	if (fun) (*fun)(); 
    /*
    Andre Schnoor 18/12/2008 : unlike Tiger, Leopard doesn't like unloading dynamic libs while their code is still in use (due to threads cleanup). 
    IMHO, it doesn't harm to never actually unload the rather small MS driver libraries (60KB only)
    */
	//if (fun) (*fun)(); 
	//dlclose(handle);
}

/*------------------------------------------------------------------------------*/
/*                      initializations : wakeup & sleep                        */
/*------------------------------------------------------------------------------*/
static Boolean LoadDriver(char *drvName) 
{
	MacOSXDriverPtr mem = (MacOSXDriverPtr)AllocateMemory(kStdMemory, sizeof(MacOSXDriver));
	if (!mem) return false;
	
	mem->next = gMacOSXDriver;
	mem->handle = LoadLibrary(drvName,"Start");
	
	if (mem->handle) {
		gMacOSXDriver = mem;
	} else {
		DisposeMemory(mem);
		return false;
	}
	return true;
}

/*------------------------------------------------------------------------------*/
void SpecialWakeUp(TMSGlobalPtr g) 
{
	unsigned short i, n;
	char str[256];
   
	CheckInstall();
	g->error = MIDInoErr;
	n = CountDrivers();
	for (i = 0; i < n; i++) {
		if (GetDriver(i, str, 256)) {
			if (!LoadDriver(str))
				g->error += MIDIerrDriverLoad;
		}
	}
}

/*------------------------------------------------------------------------------*/
void SpecialSleep(TMSGlobalPtr g)
{
	MacOSXDriverPtr next, drv = gMacOSXDriver;
	gMacOSXDriver = 0;
	while (drv) {
		next = drv->next;
		FreeLibrary(drv->handle,"Stop");
		DisposeMemory(drv);
		drv = next;
	}
 }

/*------------------------------------------------------------------------------*/
/*                   client applications context and tasks                      */
/*------------------------------------------------------------------------------*/
TApplContextPtr CreateApplContext ()  {return 0;}
void  DisposeApplContext (TApplContextPtr context)  {}

/*_________________________________________________________________________*/
void CallApplAlarm(TApplContextPtr context, ApplAlarmPtr alarm, short refNum, long alarmCode)
{
     (*alarm) (refNum, alarmCode);     /* real alarm call */
}

/*_________________________________________________________________________*/
void CallRcvAlarm(TApplContextPtr context, RcvAlarmPtr alarm, short refNum)
{
    (*alarm) (refNum);                /* real alarm call */
}

/*_________________________________________________________________________*/
void CallTaskCode(TApplContextPtr context, MidiEvPtr e)
{
    TTaskExtPtr task = (TTaskExtPtr)LinkST(e);      /* event extension */
    (*task->fun)(Date(e), RefNum(e), task->arg1, task->arg2, task->arg3);
}

/*_________________________________________________________________________*/
void CallDTaskCode(TApplContextPtr context, MidiEvPtr e)
{
	TTaskExtPtr task = (TTaskExtPtr)LinkST(e);      /* event extension */
	(*task->fun)(Date(e), RefNum(e), task->arg1, task->arg2, task->arg3);
}

/*_________________________________________________________________________*/
void DriverWakeUp(TApplPtr appl) 
{
	WakeupPtr wakeup = Wakeup(appl);
	if (wakeup) {
		wakeup (appl->refNum);
	}
}

/*_________________________________________________________________________*/
void DriverSleep(TApplPtr appl)
{
	SleepPtr sleep = Sleep(appl);
	if (sleep) {
		 sleep (appl->refNum);
	}
}

/*_________________________________________________________________________*/
Boolean ForgetTaskSync(MidiEvPtr * taskPtr, MidiEvPtr content)
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
void* AllocateMemory(MemoryType type, unsigned long size) {return (void*)malloc(size);}
void DisposeMemory(void* memPtr) {if (memPtr) free(memPtr);}

/*_________________________________________________________________________*/
/* time task                                                               */
/*_________________________________________________________________________*/
void OpenTimeInterrupts (TMSGlobalPtr g)
{
#ifdef __msBench__
    Report("MidiShare", "time accuracy bench will run","");
	bench_reset();
#endif
	gTimeMode = LoadTimeMode();
	switch (gTimeMode) {
		case kAudioTime:
			OpenAudioTime (g);
			break;
		default:
			OpenTimer (g);
	}
}

void CloseTimeInterrupts(TMSGlobalPtr g)
{
	switch (gTimeMode) {
		case kAudioTime:
			CloseAudioTime (g);
			break;
		default:
			CloseTimer (g);
	}
#ifdef __msBench__
	print_bench();
#endif
}
