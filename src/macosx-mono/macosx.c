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
#include <dlfcn.h>

#include <CoreAudio/CoreAudio.h>
#include <AudioToolbox/AudioConverter.h>

/*------------------------------------------------------------------------------*/
/* MacOSX specific resources          						*/

#define DriverMaxEntry	512

typedef struct MacOSXDriver MacOSXDriver, * MacOSXDriverPtr;

struct MacOSXDriver {
	MacOSXDriverPtr	next;
	void*		handle;
};

static MacOSXDriverPtr gMacOSXDriver = { 0 };
static long gFrames = 0;
static long gAudioSize = 0; // 10 * real size
static long gSampleRate = 0; 
static long gAudioMsInt = 0;
static AudioDeviceID gCoreDeviceID;

MutexResCode msOpenMutex  (MutexRef ref) {return kSuccess;}
MutexResCode msCloseMutex (MutexRef ref) {return kSuccess;}
/*------------------------------------------------------------------------------*/
Boolean MSCompareAndSwap (FarPtr(void) *adr, FarPtr(void) compareTo, FarPtr(void) swapWith)
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
	system("[ -x /Applications/MidiShare/.checkinstall ] && /Applications/MidiShare/.checkinstall");
}

/*------------------------------------------------------------------------------*/
void *  LoadLibrary(const char *filename, const char *symbol)
{
	void * handle = dlopen(filename, RTLD_LAZY);
	Start fun ;
	Boolean res;
        
        if (!handle) { 
            Report("MidiShare", "can't load driver", filename);
        }else if ((fun = (Start)dlsym(handle,symbol)) && (res = (*fun)())){
            return handle;
        }else {
            Report("MidiShare", "can't start driver", filename);
            if (handle) dlclose(handle);
	}
	return 0;
}

/*------------------------------------------------------------------------------*/
void FreeLibrary(void * handle, const char *symbol)
{ 
	Stop fun;
	if (fun = (Stop)dlsym(handle, symbol)) (*fun)(); 
	dlclose(handle);
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
        }else {
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

       n = CountDrivers();
       for (i=0; i<n; i++) {
           if (GetDriver(i, str, 256)) LoadDriver(str);
       }
}

/*------------------------------------------------------------------------------*/
void SpecialSleep(TMSGlobalPtr g)
{
        MacOSXDriverPtr next, drv = gMacOSXDriver;
        gMacOSXDriver = 0;
        while (drv) {
                next = drv->next;
				FreeLibrary (drv->handle,"Stop");
                DisposeMemory (drv);
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

/*__________________________________________________________________________*/
//      Interrupt handlers  : using CoreAudio                                                                                                            
/*__________________________________________________________________________*/

static OSStatus AudioClockHandler(AudioDeviceID inDevice, 
                                const AudioTimeStamp * inNow,
                                const AudioBufferList * inInputData, 
                                const AudioTimeStamp * inInputTime,
                                AudioBufferList * outOutputData,
                                const AudioTimeStamp * inOutputTime,
                                void * inClientData)
{
    gFrames += gAudioSize;
 	
    while (gFrames >= gAudioMsInt) {
        ClockHandler((TMSGlobalPtr)inClientData);
        gFrames -= gAudioMsInt;
    }
    
    return kAudioHardwareNoError; 
}

/*_________________________________________________________________________*/
/*
Get the device nominal sample rate.
*/
static bool GetSampeRate(AudioDeviceID inDeviceID, long* sr)
{
    Float64 sampleRate = 0;
    Boolean isInput = false;
    UInt32 theSize = sizeof(Float64);
    
    OSStatus err = AudioDeviceGetProperty(inDeviceID, 0, isInput, 
                            kAudioDevicePropertyNominalSampleRate, &theSize, &sampleRate);
    if (err != kAudioHardwareNoError) return false;	                            
    *sr = (long)sampleRate;                                                
    return true;
}

/*_________________________________________________________________________*/
/*
Check that the wanted buffer size can be used, otherwise returns the nearer possible
buffer size.
*/
static bool GetBufferSize(AudioDeviceID inDeviceID, long* buffersize)
{
    Boolean isInput = false;
    UInt32 theSize = sizeof(AudioValueRange);		
    AudioValueRange range;
    OSStatus err = AudioDeviceGetProperty(inDeviceID, 0, isInput, 
            kAudioDevicePropertyBufferFrameSizeRange, &theSize, &range);				
    if (err != kAudioHardwareNoError) return false;
    
    if ((*buffersize < range.mMinimum) || (*buffersize > range.mMaximum)) { 
        char size[64];
        sprintf (size, "%ld",*buffersize);
        Report("MidiShare", "Unsupported buffer size:", size);
        *buffersize = (*buffersize < range.mMinimum) ? range.mMinimum : range.mMaximum;
        sprintf (size, "%ld",*buffersize);
        Report("MidiShare", "Use new buffer size:", size);
    }
    return true;
}

/*_________________________________________________________________________*/
static bool SetBufferSize( AudioDeviceID devID, long buffersize)
{
    UInt32 dataSize = sizeof(UInt32);
    UInt32 ioBufferSize = buffersize;
   
    OSStatus err = AudioDeviceSetProperty( devID, 0, 0, false,
                            kAudioDevicePropertyBufferFrameSize, dataSize,
                            &ioBufferSize);
                            
    return (err == kAudioHardwareNoError);
}

/*_________________________________________________________________________*/
static bool GetDefaultDeviceID(char* driver_name, AudioDeviceID* id)
{
    UInt32 theSize = sizeof(AudioDeviceID);
    UInt32 outSize;
    Boolean  outWritable;
    OSStatus err = AudioHardwareGetProperty(kAudioHardwarePropertyDefaultOutputDevice, &theSize, id);
    if (err != kAudioHardwareNoError) return false;
    
    err =  AudioDeviceGetPropertyInfo(*id, 0, true, kAudioDevicePropertyDeviceName, &outSize, &outWritable);
    if (err != kAudioHardwareNoError) return false;    
    
    err = AudioDeviceGetProperty(*id, 0, true,  kAudioDevicePropertyDeviceName, &outSize, (void *) driver_name);
    return (err == kAudioHardwareNoError);
}

/*_________________________________________________________________________*/
static bool GetDeviceID(char* driver_name, AudioDeviceID* id)
{
    OSStatus err = noErr;
    UInt32   outSize;
    Boolean  outWritable;
    int      numCoreDevices;
    AudioDeviceID * coreDeviceIDs;
    bool res = false;
    int i;
    
    // Find out how many Core Audio devices there are, if any
    outSize = sizeof(outWritable);
    err = AudioHardwareGetPropertyInfo(kAudioHardwarePropertyDevices, &outSize, &outWritable);
    if (err != kAudioHardwareNoError) return false;
       
    // Calculate the number of device available
    numCoreDevices = outSize/sizeof(AudioDeviceID);

    // Bail if there aren't any devices
    if (numCoreDevices < 1)  return false;
    
    // Make space for the devices we are about to get
    coreDeviceIDs = (AudioDeviceID *) malloc(outSize);

    // Get an array of AudioDeviceIDs
    err = AudioHardwareGetProperty(kAudioHardwarePropertyDevices, &outSize, (void *)coreDeviceIDs);
    if (err != kAudioHardwareNoError) return false;
 
    // Look for the CoreAudio device corresponding to the wanted driver
    char coreaudio_name[256];
    
    for (i = 0; i<numCoreDevices; i++) {
    
        err =  AudioDeviceGetPropertyInfo(coreDeviceIDs[i], 0, true, 
            kAudioDevicePropertyDeviceName, &outSize, &outWritable);
        
        if (err != kAudioHardwareNoError) return false;
        
        err = AudioDeviceGetProperty(coreDeviceIDs[i], 0, true, 
            kAudioDevicePropertyDeviceName, &outSize, (void *) coreaudio_name);
     
        if (err != kAudioHardwareNoError) return false;
          
        if (strcmp(coreaudio_name,driver_name) == 0) {
            *id = coreDeviceIDs[i];
            res = true;
            break;
        }
    }
    
    free(coreDeviceIDs);
    return res;
}

/*_________________________________________________________________________*/
void OpenTimeInterrupts(TMSGlobalPtr g)
{
    OSStatus err;
    long bufferSize;
    char driverName[DriverMaxEntry];
    
    bufferSize = LoadBufferSize(); 		// Load audio size from the .ini file
    LoadDriverName(driverName,DriverMaxEntry);  // Load driver name from the .ini file
    
    if (!GetDeviceID(driverName, &gCoreDeviceID)) {
        Report("MidiShare", "CoreAudio device not found:", driverName);
        if (GetDefaultDeviceID(driverName,&gCoreDeviceID)) {
            Report("MidiShare", "Use CoreAudio default system device:", driverName);
        }else{
            Report("MidiShare", "CoreAudio default system device can not be found","");
            goto error;
        }
    }
    
    if (!GetBufferSize(gCoreDeviceID,&bufferSize)){
        Report("MidiShare", "Cannot get buffer size range", "");
        goto error;
    }
    
    if (!SetBufferSize(gCoreDeviceID,bufferSize)){
        Report("MidiShare", "Cannot set buffer size range", "");
        goto error;
    }

    if (!GetSampeRate(gCoreDeviceID,&gSampleRate)) {
        Report("MidiShare", "Cannot get sample rate value", "");
        goto error;
    }
    
    gAudioMsInt = gSampleRate/100;
    gAudioSize = bufferSize*10;
    
    err = AudioDeviceAddIOProc(gCoreDeviceID, AudioClockHandler, g);
    if (err != kAudioHardwareNoError) goto error;
    
    err = AudioDeviceStart(gCoreDeviceID, AudioClockHandler);
    if (err != kAudioHardwareNoError) goto error;

    return;
	 
error:
    Report("MidiShare", "cannot open audio timer:", driverName);
    CloseTimeInterrupts(g);
}

/*_________________________________________________________________________*/
void CloseTimeInterrupts(TMSGlobalPtr g)
{
    OSStatus err = AudioDeviceStop(gCoreDeviceID, AudioClockHandler);
    if (err != kAudioHardwareNoError) goto error;
    
    err = AudioDeviceRemoveIOProc(gCoreDeviceID, AudioClockHandler);
    if (err != kAudioHardwareNoError) goto error;
    
    return;
    
error:
    Report("MidiShare", "cannot close audio timer:","");
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
FarPtr(void) AllocateMemory(MemoryType type, unsigned long size) {return (void*)malloc(size);}
void DisposeMemory(FarPtr(void) memPtr) {if (memPtr) free(memPtr);}

