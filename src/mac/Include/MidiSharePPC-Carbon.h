/*
  Copyright © Grame 1999-2002

  This program is free software; you can redistribute it and modify it under 
  the terms of the GNU General Public License as published by the 
  Free Software Foundation version 2 of the License.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for 
  more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

  Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
  grame@grame.fr

*/

#ifndef __MidiSharePPC_Carbon__
#define __MidiSharePPC_Carbon__

#include <Carbon.h>

#ifdef __cplusplus
extern "C" {
#endif

	
/*
Re-implement a special CallUniversalProc
*/

typedef long (*CallUniversalProcProcPtr)( UniversalProcPtr theProcPtr, ProcInfoType procInfo,...) ;
#define CallUniversalProc (*tCallUniversalProcProcPtr)


/*
Use a global static variable which contains the real CallUniversalProc entry point
*/

static CallUniversalProcProcPtr tCallUniversalProcProcPtr = nil;


/* 
Include the standard MidiShare.h file
*/
	
#include "MidiSharePPC-68k.h" 


/*
Load the real CallUniversalProc entry point
*/

void Load_CallUniversalProc () 
{
	CFragConnectionID  			connectionID;
	Ptr                			mainAddress;
	Str255             			errorString;
	OSErr              			anErr;

	if (nil == tCallUniversalProcProcPtr)	/* if it's not defined yet */
	{
		/* Resolve the symbol against InterfaceLib. */
		anErr = GetSharedLibrary("\pInterfaceLib", kPowerPCCFragArch, kFindCFrag, &connectionID, &mainAddress, errorString);
		if (noErr == anErr)
		{
			CFragSymbolClass    symbolClass;
		        anErr = FindSymbol(connectionID, "\pCallUniversalProc",
		       			(Ptr *) &tCallUniversalProcProcPtr,
									&symbolClass);
		        if (noErr != anErr)
		       	tCallUniversalProcProcPtr = nil;
		}
	}

}

/*
Re-define the MidiShare() function to call the Load_CallUniversalProc, since MidiShare() is supposed to be called
before any other MidiShare function.

Otherwise Load_CallUniversalProc can be called at initialisation time.
*/


#ifdef MidiShare
#undef MidiShare
int MidiShare()
{
	 Load_CallUniversalProc();
	 return	( **((long**)0xB8) == 0xD080D080 );
}
#endif

#ifdef __cplusplus
}
#endif

#endif