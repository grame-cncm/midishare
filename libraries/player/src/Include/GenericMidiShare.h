// ===========================================================================
// The Player Library is Copyright (c) Grame, Computer Music Research Laboratory 
// 1996-1999, and is distributed as Open Source software under the Artistic License;
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
	#define EXPORT
	#define MSALARMAPI
#endif

#ifdef __Macintosh__
	#if GENERATINGCFM
		#include <midisharePPC.h>
	#else
		#include <midishare.h>
	#endif
	
	#define FAR
	#define NEAR
	#define MFAPI
	/* New type definition */
	typedef struct TFilter * MidiFilterPtr;

#else
  #ifdef WIN32
    #include <windows.h>

	#define __MSWindows__ 
	#define __Windows__
	#define __UseMSOldInterface__

    #define  EXPORT  __declspec(dllexport)
    #define  MFAPI
    #define __DEBUG__ 0

  #endif
  #ifdef  __MSWindows__
	#include <MidiShare.h>
	#define true 1
	#define false 0
	#define nil 0
		
  #endif
#endif


#endif
