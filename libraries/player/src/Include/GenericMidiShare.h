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
	
	/* New type definition */
	typedef struct TFilter * MidiFilterPtr;

#else
  #ifdef WIN32
     #include "Playerwin32.h"
  #endif
  #ifdef  __MSWindows__
	#include <MidiShare.h>
	#define true 1
	#define false 0
	#define nil 0
	
	/* New type definition */
	typedef struct TFilter * MidiFilterPtr;
	
  #endif
#endif


#endif
