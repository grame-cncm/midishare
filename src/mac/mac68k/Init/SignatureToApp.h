////
////	SignatureToApp.h		Interface for SignatureToApp
////
////	By Jens Alfke			©1991 Apple Computer, Inc. All rights reserved.
////

#ifndef __SIGNATURETOAPP__
	#define __SIGNATURETOAPP__

#ifndef __PROCESSES__
	#include <Processes.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum{
	Sig2App_FindProcess		=0,
	Sig2App_FindApplication,
	Sig2App_LaunchApplication
} Sig2App_Mode;

OSErr
SignatureToApp(	OSType sig,						// Signature of app
				ProcessSerialNumber *psn,		// Returns PSN of app (ignored if NULL)
				FSSpec *fileSpec,				// Returns location of app (ignored if NULL)
				Boolean *launched,				// Set to TRUE if I had to launch (ignored if NULL)
				Sig2App_Mode mode,				// Mode -- find proc/find app/launch app
				LaunchFlags launchControlFlags	// Launch control flags; see Process Mgr
			  );
#ifdef __cplusplus
}
#endif

#endif



