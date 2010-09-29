/*

  Copyright � Grame 1999

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
    TApplContextPtr context;     /* system dependent context     */
    void*			info;        /* user field                   */
    uchar           folder;      /* application folder           */
    uchar           refNum;      /* reference number             */
    uchar           rcvFlag;     /* <> 0 to call rcvAlarm        */
	uchar			unused[sizeof(void*)*2 - 3]; /* for alignment */
    RcvAlarmPtr     rcvAlarm;    /* rcv alarm pointer            */
    ApplAlarmPtr    applAlarm;   /* application alarm pointer    */

    TConnectionPtr  srcList;     /* intput applications list  */
    TConnectionPtr  dstList;     /* output applications list  */

    fifo            rcv;         /* received events fifo      */
    fifo            dTasks;      /* defered tasks fifo        */
 
    MidiFilterPtr   filter;      /* application filter        */
    TDriverPtr		driver;		 /* driver specific storage   */
    
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
