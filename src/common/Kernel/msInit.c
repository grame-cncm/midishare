/*
  MidiShare Project
  Copyright (C) Grame 1999-2005

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
  
  modifications history:
   [08-09-99] DF - adaptation to the new memory management
                   version number increased

*/

#include "msAppFun.h"
#include "msInit.h"
#include "msDrvFun.h"
#include "msEvents.h"
#include "msExtern.h"
#include "msTime.h"
#include "msAlarms.h"

static int	initialized = 0;

/*===========================================================================
  External MidiShare functions implementation
  =========================================================================== */		
MSFunctionType(long) MSGetError (TMSGlobalPtr g)
{
	return g->error;
}

MSFunctionType(void) MSSpecialInit( ulong defaultSpace, TMSGlobalPtr g)
{
	if (initialized) return;
	g->error=MIDInoErr;
	InitEvents ();
	InitMemory(Memory(g), defaultSpace);
	InitAppls (Clients(g), Memory(g));
	InitTime( g);
	initialized = 1;
}

MSFunctionType(short) MSGetVersion (TMSGlobalPtr g)
{
#ifdef __x86_64__
//printf("sizeof(long): %d\n", sizeof(long));
//printf("sizeof(int): %d\n", sizeof(int));
//printf("sizeof(short): %d\n", sizeof(short));
	return 195;
#else
//printf("sizeof(long): %d\n", sizeof(long));
//printf("sizeof(int): %d\n", sizeof(int));
//printf("sizeof(short): %d\n", sizeof(short));
	return 193;
#endif
}


/*===========================================================================
  External initialization functions
  =========================================================================== */
void MidiShareWakeup (TMSGlobalPtr g) 
{
    SpecialWakeUp (g);
    fifoinit (SorterList(g));
    OpenMemory (Memory(g));
    OpenTime (g);
    OpenTimeInterrupts (g);
	OpenDrivers(g);
}

void MidiShareSleep (TMSGlobalPtr g) 
{
    CloseTimeInterrupts (g);
	CloseDrivers(g);
    CloseMemory (Memory(g));
    SpecialSleep (g);
}
