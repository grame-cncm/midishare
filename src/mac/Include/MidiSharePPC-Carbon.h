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
Re-implement a special CallUniversalProc, NewRoutineDescriptor and DisposeRoutineDescriptor
*/

typedef long (*CallUniversalProcProcPtr)( UniversalProcPtr theProcPtr, ProcInfoType procInfo,...) ;
#define CallUniversalProc (*tCallUniversalProcProcPtr)


typedef UniversalProcPtr (*NewRoutineDescriptorProcPtr)(ProcPtr theProc, ProcInfoType , ISAType );
#define NewRoutineDescriptor (*tNewRoutineDescriptorProcPtr)

typedef void (*DisposeRoutineDescriptorProcPtr)(UniversalProcPtr);
#define DisposeRoutineDescriptor (*tDisposeRoutineDescriptorProcPtr)

/*
Use global static variables which contains the real CallUniversalProc, NewRoutineDescriptor and DisposeRoutineDescriptor entry points
*/

static CallUniversalProcProcPtr tCallUniversalProcProcPtr = nil;
static NewRoutineDescriptorProcPtr tNewRoutineDescriptorProcPtr = nil;
static DisposeRoutineDescriptorProcPtr tDisposeRoutineDescriptorProcPtr = nil;


/* 
Include the standard MidiShare.h file
*/
	
#include "MidiSharePPC-68k.h" 


/*
Load the real CallUniversalProc, NewRoutineDescriptor and DisposeRoutineDescriptor entry points
*/

void Load_FunctionProc () 
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
		       	
		        anErr = FindSymbol(connectionID, "\pNewRoutineDescriptor",
		       			(Ptr *) &tNewRoutineDescriptorProcPtr,
									&symbolClass);
		        if (noErr != anErr)
		       	tNewRoutineDescriptorProcPtr = nil;
		       	
		       	anErr = FindSymbol(connectionID, "\pDisposeRoutineDescriptor",
		       			(Ptr *) &tDisposeRoutineDescriptorProcPtr,
									&symbolClass);
		        if (noErr != anErr)
		       	tDisposeRoutineDescriptorProcPtr = nil;
		}
		
		
	}
	
}

/*
Re-define the MidiShare() function to call the Load_FunctionProc, since MidiShare() is supposed to be called
before any other MidiShare function.

Otherwise Load_FunctionProc can be called at initialisation time.
*/


#ifdef MidiShare
#undef MidiShare
int MidiShare()
{
	 Load_FunctionProc();
	 return	( **((long**)0xB8) == 0xD080D080 );
}
#endif

#ifdef __cplusplus
}
#endif

#endif