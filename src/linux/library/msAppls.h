/*

  Copyright © Grame 1999

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

*/


#include <pthread.h>

#include "msTypes.h"  
#include "msDefs.h"  
#include "msMemory.h"  

#define MaxAppls         128		/* maximum allowed applications    */
#define MaxApplNameLen   32            /* maximum application name length */

typedef  char MSName[MaxApplNameLen];  /* A REVOIR */


/* MidiShare application internal data structures                    	*/
typedef struct TAppl{
   	FarPtr(void)    info;        /* user field                   	*/
    	uchar           refNum;      /* reference number             	*/
 	RcvAlarmPtr     rcvAlarm;    /* rcv alarm pointer            	*/
    	ApplAlarmPtr    applAlarm;   /* application alarm pointer    	*/
	pthread_t       rcvThread;   /* real time rcv thread 		*/    
} TAppl;

typedef struct FarPtr(TAppl) 		TApplPtr;

/*___________________________________*/
typedef struct TClients {
	short         	nbAppls;                /* current running clients count   */
	TApplPtr      	appls[MaxAppls];        /* client applications list        */
	TMSMemory     	memory;			/* shared event memory */
	MSName		nameTable[MaxAppls];    /* array of string (for application name management)*/
	pthread_mutex_t mutex;			/* open/close sychronisation mutex */
	int             msfd;			/* the MidiShare Device file number */
} TClients;



typedef struct FarPtr(TClients) 		TClientsPtr;


/*--------------------------------------------------------------------------*/
/* macros                                                                   */
/*--------------------------------------------------------------------------*/
#define CheckRefNum( g, r)    ((r>=0) && (r<MaxAppls) && g->appls[r])
#define Memory(g)             (&g->memory)
#define Device(g)             (g->msfd)
#define Mutex(g)              (&g->mutex)
#define Appls(g)              (g->appls)
#define ApplsCount(g)         (g->nbAppls)
