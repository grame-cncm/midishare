// ===========================================================================
// The Player Library is Copyright (c) Grame, Computer Music Research Laboratory 
// 1996-1999, and is distributed as Open Source software under the Artistic License;
// see the file "Artistic" that is included in the distribution for details.
//
// Grame : Computer Music Research Laboratory
// Web : http://www.grame.fr/Research
// E-mail : MidiShare@rd.grame.fr
// ===========================================================================


#ifndef __UDebug__
#define __UDebug__


#ifdef __DEBUG__

	#ifdef __Macintosh__
		#include <assert.h>
	#endif
	
	#ifdef __MSWindows__
		#define assert(b)
	#endif
	
	#ifdef __Linux__
		#include <assert.h>
	#endif
	
#else
	#define assert(b)
#endif


#endif
