/*
  MidiShare Project
  Copyright (C) Grame 1999

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

  modifications history:
   [08-09-99] DF - adaptation to the new memory management

*/

#ifndef __msKernel__
#define __msKernel__

#include "lffifo.h"
#include "msDefs.h"
#include "msAppls.h"
#include "msTypes.h"
#include "msMemory.h"
#include "msSorter.h"

/*------------------------------------------------------------------------*/
/* data types                                                             */
/*------------------------------------------------------------------------*/
typedef struct TMSGlobal *  TMSGlobalPtr;
typedef struct THorloge  *  THorlogePtr;
typedef void*				THost;   /* reserved for platform dependant storage */

/*------------------------------------------------------------------------*/
/* realtime clock */
typedef struct {
	long	sec;
	long	usec;
} TimeInfo, *TimeInfoPtr;

typedef struct THorloge{
	 unsigned long  time;         /* la date sur 32 bits (millisecondes)  */
	 long           reenterCount; /* count of clockHandler reenters       */
	 TimeInfo		rtOffset;     /* offset to real time clock            */
	 long			adjustCount;  /* clocks adjustment count              */
	 long			offset;       /* current real-time clock offset       */
	 char			padding[40];  /* alignment      */
} THorloge;

/*___________________________________*/
typedef struct TMSGlobal {
	fifo          toSched;       /* events to be scheduled              */
	TMSMemory     memory;        /* memory management                   */
	THorloge      currTime;      /* time management                     */
	TClients      clients;       /* clients applications management     */
	TSorter       sorter;        /* sorter specific storage             */
	long          error;		 /* the global system errors            */
	THost         local;         /* for implementation specific purpose */
} TMSGlobal;

/*--------------------------------------------------------------------------*/
/* fields access macros                                                     */
/*--------------------------------------------------------------------------*/
#define SorterList(g)       (&g->toSched)
#define Sorter(g)           (&g->sorter)
#define Clients(g)       	(&g->clients)
#define Memory(g)           (&g->memory)
#define CurrTime(g)         (g->currTime.time)
#define TimeOffset(g)       (g->currTime.rtOffset)
#define Appls(g)       		(g->clients.appls)
#define ActiveAppl(g)       (g->clients.activesAppls)
#define NextActiveAppl(g)   (g->clients.nextActiveAppl)

#define kClockAdjustCount	1000

/*--------------------------------------------------------------------------*/
/* function declaration                                                     */
/*--------------------------------------------------------------------------*/
void ClockHandler (TMSGlobalPtr g);

#endif
