/*

  Copyright © Grame 1999-2000

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
  grame@rd.grame.fr

*/

#include "Events.h"
#include "Gestalt.h"
#include "Memory.h"
#include "Resources.h"
#include "ToolUtils.h"
#include "Types.h"

#include "LVAL.h"
#include "msKernel.h"
#include "MidiShareMac.h"
#include "ShowInitIcon.h"

/*_______________________________________________________________________________*/
/* constants definition  */
enum { 	
		goodIcon	= -4064,
		badIcon	= 128
	 };

#define 	MSCodeRsrcName "\pMidiShare"
#define 	Align(val)			(((val) >> 2) << 4)	/* 32 bits alignment macro */

/*_______________________________________________________________________________*/
/* MidiShare code resource structure */
typedef struct {
	void *	memPtr;
	Ptr		code; 
} codeRes, * codeResPtr;

typedef	long * LongIntPtr;

pascal  void  MidiShareSpecialInit	(long defaultSize) 		={0x7037, 0x2078, 0x00B8, 0x4E90};
void __Startup__();

/*_______________________________________________________________________________*/
void __Startup__()
{
	Handle codeHandler; Ptr codeMem, msMem;
	void **dPtr ; long codeSize, memSize; 
	long 	vmAttr; Boolean vm; OSErr err;
	codeResPtr	cPtr;
	LVALResHdl	sizeRes;

	err = Gestalt (gestaltVMAttr, &vmAttr);
	vm  = (err == noErr) && BitTst (&vmAttr, gestaltVMPresent);
	
	codeHandler= GetNamedResource ('CODE', MSCodeRsrcName);		/* load the CODE resource 	*/
	if (!codeHandler) goto failed;								/* loading failed			*/

	codeSize = InlineGetHandleSize (codeHandler);				/* code resource size			*/
	memSize = codeSize + sizeof(TMSGlobal) + 4;
	msMem = NewPtrSys (memSize);
	if (msMem) {
		codeMem = (Ptr)(Align((long)msMem + sizeof(TMSGlobal)) + 4);
		if (vm) HoldMemory(msMem, memSize);
		BlockMove(*codeHandler, codeMem, codeSize);			/* duplicates the CODE resource 	*/

		cPtr = (codeResPtr)(codeMem);
		cPtr->memPtr = msMem;
		dPtr  = (void **)kTrap14;						/* TRAP #14 adress			*/
		*dPtr = &cPtr->code;							/* stores MidiShare entry point	*/
	}
	else goto failed;

	sizeRes= (LVALResHdl)GetResource('LVAL',-4048);			/* LVAL resource handle			*/
	MidiShareSpecialInit (sizeRes ? (*sizeRes)->memory : 4000);
	ShowInitIcon (goodIcon, true);
	return;

failed:
	ShowInitIcon (badIcon, true);
}
