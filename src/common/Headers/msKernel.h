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
  grame@rd.grame.fr

  modifications history:
   [08-09-99] DF - adaptation to the new memory management
   [04-08-02] DF - memory organization in private and public sections

*/

#ifndef __msKernel__
#define __msKernel__

#include "lffifo.h"
#include "msDefs.h"
#include "msAppls.h"
#include "msDriver.h"
#include "msTypes.h"
#include "msMemory.h"
#include "msSorter.h"

/*------------------------------------------------------------------------*/
/* data types                                                             */
/*------------------------------------------------------------------------*/
typedef struct TMSGlobal FAR *  TMSGlobalPtr;
typedef struct THorloge  FAR *  THorlogePtr;
typedef FarPtr(void)			THost;   /* reserved for platform dependant
                                            storage */

/*------------------------------------------------------------------------*/
/* realtime clock */
typedef struct {
	long	sec;
	long	usec;
} TimeInfo, *TimeInfoPtr;

typedef struct THorloge{          /* time management (no public section) */
	 long           reenterCount; /* count of clockHandler reenters       */
	 TimeInfo		rtOffset;     /* offset to real time clock            */
	 long			adjustCount;  /* clocks adjustment count              */
	 long			offset;       /* current real-time clock offset       */
} THorloge;

/*___________________________________*/
typedef struct TMSGlobalPublic {
	unsigned long  	time;         /* the current 32 bits (milliseconds) date  */
	short 			version;      /* the current kernel version               */
	unsigned long  	size;         /* the current public segment size          */
	TClientsPublic 	clients;      /* clients applications information         */
} TMSGlobalPublic;

/*___________________________________*/
typedef struct TMSGlobal {
	TMSGlobalPublic * pub;
	THorloge      clock;         /* time management (no public section) */
	TMSMemory     memory;        /* kernel memory management            */
	TClients      clients;       /* clients applications management     */
	TSorter       sorter;        /* sorter specific storage             */
	fifo          toSched;       /* events to be scheduled              */
	THost         local;         /* for implementation specific purpose */
} TMSGlobal;

/*--------------------------------------------------------------------------*/
/* public fields access macros                                              */
/*--------------------------------------------------------------------------*/
#define CurrTime(g)         pub(g, time)
#define Version(g)          pub(g, version)
#define Size(g)             pub(g, size)

/*--------------------------------------------------------------------------*/
/* fields access macros                                                     */
/*--------------------------------------------------------------------------*/
#define SorterList(g)       (&g->toSched)
#define Sorter(g)           (&g->sorter)
#define Clients(g)       	(&g->clients)
#define Memory(g)           (&g->memory)
#define TimeOffset(g)       (g->clock.rtOffset)
#define Appls(g)       		(g->clients.appls)
#define ActiveAppl(g)       (g->clients.activesAppls)
#define NextActiveAppl(g)   (g->clients.nextActiveAppl)

#define kClockAdjustCount	1000

/*--------------------------------------------------------------------------*/
/* function declaration                                                     */
/*--------------------------------------------------------------------------*/
void ClockHandler (TMSGlobalPtr g);

#endif
