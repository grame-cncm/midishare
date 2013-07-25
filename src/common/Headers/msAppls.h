/*
  MidiShare Project
  Copyright (C) Grame 1999

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

  modifications history:
   [08-09-99] DF - new fifo management

*/

#ifndef __msAppls__
#define __msAppls__

#include "lffifo.h"
#include "msMemory.h"
#include "msTypes.h"
#include "msDefs.h"
#include "msDriver.h"

/*------------------------------------------------------------------------*/
/* constants definitions                                                  */
/*------------------------------------------------------------------------*/
#define kNoRcvFlag       -1
#define MaxAppls         128            /* maximum allowed clients         */
#define MaxApplNameLen   32             /* maximum application name length */
#define MidiShareRef     0              /* MidiShare refnum at client side */
#define MidiShareDriverRef  MaxAppls-1  /* MidiShare refnum at driver side */

#ifdef PascalNames
# define kMidiShareName    "\pMidiShare"
#else
# define kMidiShareName     "MidiShare"
#endif

/*------------------------------------------------------------------------*/
/* data types                                                             */
/*------------------------------------------------------------------------*/

typedef void*					TApplContextPtr;

typedef struct TAppl*			TApplPtr;
typedef struct TClients*		TClientsPtr;
typedef struct TConnection* 	TConnectionPtr;


#ifdef PascalNames
typedef unsigned char MSName[MaxApplNameLen];
#else
typedef char MSName[MaxApplNameLen];
#endif

/*------------------------------------------------------------------------*/
/* inter-applications connections representation                          */
typedef struct TConnection{
    TApplPtr        itsSrc;         /* source application       */
    TApplPtr        itsDst;         /* destination application  */
    TConnectionPtr  nextSrc;        /* next source       */
    TConnectionPtr  nextDst;        /* next destination  */
} TConnection;

/*------------------------------------------------------------------------*/
/* MidiShare application internal data structures                         */
enum { kClientFolder = 0, kDriverFolder = 255 };

typedef struct TAppl{
    MSName          name;        /* the application name         */
    RcvAlarmPtr     rcvAlarm;    /* rcv alarm pointer            */
    ApplAlarmPtr    applAlarm;   /* application alarm pointer    */

    TConnectionPtr  srcList;     /* intput applications list  */
    TConnectionPtr  dstList;     /* output applications list  */

    fifo            rcv;         /* received events fifo      */
    fifo            dTasks;      /* defered tasks fifo        */

	TApplContextPtr context;     /* system dependent context     */
    void*			info;        /* user field                   */
    uchar           folder;      /* application folder           */
    uchar           refNum;      /* reference number             */
    uchar           rcvFlag;     /* <> 0 to call rcvAlarm        */
	uchar			unused[sizeof(void*)*2 - 3]; /* for alignment */
 
    MidiFilterPtr   filter;      /* application filter        */
    TDriverPtr		driver;		 /* driver specific storage   */
	void*			mem;
    
} TAppl;

/*___________________________________*/
typedef struct TClients {
	short         nbAppls;                /* current running clients count    */
	short         nbDrivers;              /* current registered drivers count */
	TApplPtr      appls[MaxAppls];        /* client applications list         */
	TApplPtr      activesAppls[MaxAppls]; /* active client applications list  */
	TApplPtr  *   nextActiveAppl;         /* ptr in active applications list  */
	MSMemoryPtr   memory;
} TClients;


/*--------------------------------------------------------------------------*/
/* macros                                                                   */
/*--------------------------------------------------------------------------*/
#define CheckRefNum( g, r)    	((r>=0) && (r<MaxAppls) && g->appls[r])
#define CheckClientsCount(g)	((g->nbAppls + g->nbDrivers) < MaxAppls - 2)

#define DTasksFifoHead(appl)  (MidiEvPtr)(appl->dTasks.head)

/*--------------------------------------------------------------------------*/
/* functions                                                                */
/*--------------------------------------------------------------------------*/
short GetIndClient (short index, short folderRef, TClientsPtr g);


#endif
