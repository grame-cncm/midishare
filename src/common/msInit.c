/*

  Copyright © Grame 1999-2002

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
  grame@grame.fr
  
  modifications history:
   [08-09-99] DF - adaptation to the new memory management
                   version number increased

*/

#include "msAppFun.h"
#include "msInit.h"
#include "msDrvFun.h"
#include "msEvents.h"
#include "msTime.h"

/*===========================================================================
  External MidiShare functions implementation
  =========================================================================== */		

MSFunctionType(short) MSGetVersion (TMSGlobalPtr g)
{
	return g->pub->version;
}

#ifdef MSKernel
#include "msExtern.h"

#define kMidiShareVersion  193    /* the current MidiShare version number */

static void InitPublicSection (TMSGlobalPtr g, TMSGlobalPublic * pub);
static void InitClientPublicSection (TClientsPublic * clients);


MSFunctionType(void) MSSpecialInit( unsigned long defaultSpace, TMSGlobalPtr g, TMSGlobalPublic *pub)
{
	InitPublicSection (g, pub);
	InitEvents ();
	InitMemory(Memory(g), defaultSpace);
	InitAppls (Clients(g), Memory(g));
	InitTime( g);
#ifndef __mono__
    OpenMemory (Memory(g));
#endif
    g->context = 0;
    g->running = false;
}

/*===========================================================================
  External initialization functions
  =========================================================================== */
void MidiShareWakeup (TMSGlobalPtr g) 
{
    if (g->running) return;
    g->running = true;
    SpecialWakeUp (g);
    fifoinit (SorterList(g));
#ifdef __mono__
    OpenMemory (Memory(g));
#endif
    OpenTime (g);
    OpenTimeInterrupts (g);
//	OpenDrivers(g);
}

void MidiShareSleep (TMSGlobalPtr g) 
{
    if (!g->running) return;
    CloseTimeInterrupts (g);
	CloseDrivers(g);
#ifdef __mono__
    CloseMemory (Memory(g));
#endif
    SpecialSleep (g);
    g->running = false;
}

/*===========================================================================
  Internal initialization functions
  =========================================================================== */
static void InitClientPublicSection (TClientsPublic * clients)
{
	int i;
	clients->nbAppls   = 0;               /* current running clients count    */
	clients->nbDrivers = 0;               /* current registered drivers count */
	for (i=0; i<MaxAppls; i++) {
		clients->appls[i].refNum = MIDIerrRefNum;
	}
	for (i=0; i<MaxDrivers; i++) {
		clients->drivers[i].refNum = MIDIerrRefNum;
	}
	for (i=0; i<MaxSlots; i++) {
		clients->slots[i].driverRefNum = MIDIerrRefNum;
		clients->slots[i].slotRefNum   = MIDIerrRefNum;
	}
}

static void InitPublicSection (TMSGlobalPtr g, TMSGlobalPublic * pub)
{
	g->pub = pub;
	g->clients.pub = &pub->clients;
	g->clients.lastRef = 0;
	g->clients.lastDrvRef = MidiShareDriverRef;
	g->clients.lastSlot = -1;

	pub(g, time)    = 0;
	pub(g, version) = kMidiShareVersion;
	pub(g, size)    = sizeof(TMSGlobalPublic);
	InitClientPublicSection (&pub->clients);
}
#endif
