
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
  
  S Letz, M Stauffer, Stefan Csomor
  
*/

#ifndef __MidiSharePPC_Carbon__
#define __MidiSharePPC_Carbon__

#include <MixedMode.h> 

#ifdef __cplusplus
extern "C" {
#endif


/*
Re-implement a special CallUniversalProc, NewRoutineDescriptor and
DisposeRoutineDescriptor
*/

typedef long (*CallUniversalProcProcPtr)( UniversalProcPtr theProcPtr,
ProcInfoType procInfo,...) ;
#define CallUniversalProc (*tCallUniversalProcProcPtr)


typedef UniversalProcPtr (*NewRoutineDescriptorProcPtr)(ProcPtr theProc,
ProcInfoType , ISAType );
#define NewRoutineDescriptor (*tNewRoutineDescriptorProcPtr)

typedef void (*DisposeRoutineDescriptorProcPtr)(UniversalProcPtr);
#define DisposeRoutineDescriptor (*tDisposeRoutineDescriptorProcPtr)

/*
Use global static variables which contains the real CallUniversalProc,
NewRoutineDescriptor and DisposeRoutineDescriptor entry points
*/

extern CallUniversalProcProcPtr tCallUniversalProcProcPtr ;
extern NewRoutineDescriptorProcPtr tNewRoutineDescriptorProcPtr ;
extern DisposeRoutineDescriptorProcPtr tDisposeRoutineDescriptorProcPtr;


/*
Include the standard MidiShare.h file
*/

#include "MidiSharePPC-68k.h"

/*
Re-define the MidiShare() function to call the Load_FunctionProc, since
MidiShare() is supposed to be called before any other MidiShare function.
Otherwise Load_FunctionProc can be called at initialisation time.
*/

#ifdef MidiShare
#undef MidiShare
extern int MidiShare() ;
extern void Load_FunctionProc (); 
#endif

#ifdef __cplusplus
}
#endif

#endif
