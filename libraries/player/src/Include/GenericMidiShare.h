// ===========================================================================
// The Player Library is Copyright (c) Grame, Computer Music Research Laboratory 
// 1996-2003, and is distributed as Open Source software under the Artistic License;
// see the file "Artistic" that is included in the distribution for details.
//
// Grame : Computer Music Research Laboratory
// Web : http://www.grame.fr/Research
// E-mail : MidiShare@rd.grame.fr
// ===========================================================================


#ifndef __GenericMidiShare__
#define __GenericMidiShare__

#ifdef __Linux__
	#include "MidiShare.h"
	#define true 1
	#define false 0
	#define nil 0
	#define errno 
	#define FAR
	#define NEAR
	#define EXPORT
	#define MSALARMAPI
	#define MFAPI
#endif

#ifdef __Macintosh__ 
	#ifdef __MacOS9__
		#define MidiSharePPC_68k
                #define __SupportOldSTDef__
	#else
		//#define MSALARMAPI
	#endif
	
	
	#include <MidiShare.h>
	
	
	//#define FAR
	//#define NEAR
	//#define MFAPI
#endif


#ifdef  __MSWindows__
	#ifdef WIN32
		#include <windows.h>
		#define __SupportOldSTDef__ 
   		#include "MidiShare.h"
   		#define  EXPORT  __declspec(dllexport)
   		#define  MFAPI
   		#define __DEBUG__ 0
		#define true 1
		#define false 0
		#define nil 0
	#else
		#include <MidiShare.h>
		#define true 1
		#define false 0
		#define nil 0
		#define EXPORT WINAPI _export
   		#define MFAPI WINAPI _export
	#endif
#endif

#endif
