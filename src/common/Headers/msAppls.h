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
   [08-09-99] DF - new fifo management
   [04-08-02] DF - memory organization in private and public sections

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
#define MaxApplNameLen   64             /* maximum application name length */
#define MaxDrivers       64             /* maximum allowed drivers         */
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
typedef FarPtr(void)				TApplContextPtr;
typedef struct FarPtr(TAppl) 		TApplPtr;

#ifdef PascalNames
typedef unsigned char MSName[MaxApplNameLen];
#else
typedef char MSName[MaxApplNameLen];
#endif

/*------------------------------------------------------------------------*/
/* MidiShare public application data structures                           */

typedef struct TConnections {
    char        dst[MaxAppls/8];    /* destination application */
} TConnections, * TConnectionsPtr;

enum { kClientFolder = 0, kDriverFolder = 255 };

typedef struct TApplPublic{
    MSName          name;        /* the application name         */
    uchar           folder;      /* application folder           */
    FarPtr(void)    info;        /* user field                   */
    short           refNum;      /* reference number             */
    short           drvidx;      /* driver specific info index   */
    TConnections    cnx;         /* intput/output connections    */
} TApplPublic, * TApplPublicPtr;

typedef struct TClientsPublic {
	short         nbAppls;                /* current running clients count    */
	short         nbDrivers;              /* current registered drivers count */
	TApplPublic   appls[MaxAppls];        /* client applications list         */
	TDriverPublic drivers[MaxDrivers];    /* drivers specific information     */
	SlotInfosPublic slots[MaxSlots];      /* drivers slots management         */
} TClientsPublic, * TClientsPublicPtr;

#define CheckPublicRefNum( g, r)  ((r>=0) && (r<MaxAppls) && g->appls[r].refNum!=MIDIerrRefNum)

/*--------------------------------------------------------------------------*/
/* public fields access macros                                              */
/*--------------------------------------------------------------------------*/
#define nbAppls(c)      pub(c, nbAppls)
#define nbDrivers(c)    pub(c, nbDrivers)
#define nbSlots(c)      pub(c, nbSlots)
#define folder(c)       pub(c, folder)

#if defined(MSKernel)
typedef struct FarPtr(TClients) 	TClientsPtr;

/*------------------------------------------------------------------------*/
/* MidiShare server private application data structures                   */
typedef struct TAppl{
	TApplPublic *   pub;
    fifo            rcv;         /* received events fifo         */
    fifo            dTasks;      /* defered tasks fifo           */
    MidiFilterPtr   filter;      /* server mapping of the filter */
    RcvAlarmPtr     rcvAlarm;    /* the client receive alarm (used only internaly)     */
    ApplAlarmPtr    applAlarm;   /* the client application alarm (used only internaly) */
    TApplContextPtr context;     /* system dependent context     */
    uchar           rcvFlag;     /* <> 0 to call rcvAlarm        */
    TDriverPtr		driver;      /* driver specific information  */
} TAppl;

typedef struct TClients {
	TClientsPublic * pub;
	short         nbSlots;                /* current registered slots count   */
	short         lastRef;                /* last allocated refNum            */
	short         lastDrvRef;             /* last allocated driver refNum     */
	short         lastSlot;               /* last allocated slot refNum       */
	TApplPtr      appls[MaxAppls];        /* client applications list         */
	TApplPtr      activesAppls[MaxAppls]; /* active client applications list  */
	TApplPtr  *   nextActiveAppl;         /* ptr in active applications list  */
	MSMemoryPtr   memory;                 /* ptr to the kernel memory         */
} TClients;

#define CheckRefNum( g, r)    	((r>=0) && (r<MaxAppls) && g->appls[r])
#define CheckGlobRefNum( g, r)  ((r>=0) && (r<MaxAppls) && Clients(g)->appls[r])
#define CheckClientsCount(g)	((nbAppls(g) + nbDrivers(g)) < MaxAppls - 2)

#define DTasksFifoHead(appl)  	 (MidiEvPtr)(appl->dTasks.head)

/*--------------------------------------------------------------------------*/
/* functions                                                                */
/*--------------------------------------------------------------------------*/
short GetIndClient (short index, short folderRef, TClientsPtr g);

#else

/*------------------------------------------------------------------------*/
/* MidiShare client private application data structures                   */
typedef struct TAppl{
//	TApplPublic *   pub;
	short 			refNum;
    fifo            rcv;         /* received events fifo         */
    fifo            dTasks;      /* defered tasks fifo           */
    MidiFilterPtr   filter;      /* server mapping of the filter */
    RcvAlarmPtr     rcvAlarm;    /* the client receive alarm     */
    ApplAlarmPtr    applAlarm;   /* the client application alarm */
    uchar           rcvFlag;     /* <> 0 to call rcvAlarm        */
} TAppl;

#define CheckGlobRefNum( g, r)   ((r>=0) && (r<MaxAppls) && g->appls[r])

#endif /* MSKernel */

#endif
