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

#ifndef __msIOCtl__
#define __msIOCtl__

#include "msDefs.h"

enum { kUserMode, kKernelMode };

#define kMidiShareMajor 151

/* functions index for ioctl */

enum { 	kMidiGetVersion = 1,
		kMidiCountAppls,
		kMidiGetIndAppl,
		kMidiGetNamedAppl,
		kMidiGetSyncInfo,
		kMidiSetSyncMode,
		kMidiGetExtTime,   
		kMidiInt2ExtTime,
		kMidiExt2IntTime,
		kMidiTime2Smpte,
		kMidiSmpte2Time,
		kMidiGetTimeAddr,

		kMidiOpen,
		kMidiClose,

		kMidiGetName,
		kMidiSetName,
		kMidiGetInfo,
		kMidiSetInfo,
		kMidiGetFilter,
		kMidiSetFilter,
		kMidiGetRcvAlarm,
		kMidiSetRcvAlarm,
		kMidiGetApplAlarm,
		kMidiSetApplAlarm,

		kMidiConnect,
		kMidiIsConnected,

		kMidiGetPortState,
		kMidiSetPortState,

		kMidiFreeSpace,
		kMidiTotalSpace,
		kMidiGrowSpace,

		kMidiNewCell,
		kMidiFreeCell,
		kMidiNewEv,
		kMidiCopyEv,
		kMidiFreeEv,
		kMidiSetField,
		kMidiGetField,
		kMidiAddField,
		kMidiCountFields,

		kMidiNewSeq,
		kMidiAddSeq,
		kMidiFreeSeq,
		kMidiClearSeq,
		kMidiApplySeq,

		kMidiGetTime,

		kMidiSendIm,
		kMidiSend,
		kMidiSendAt,

		kMidiCountEvs,
		kMidiGetEv,
		kMidiAvailEv,
		kMidiFlushEvs,

		kMidiReadSync,
		kMidiWriteSync,

		kMidiCall,
		kMidiTask,
		kMidiDTask,
		kMidiForgetTask,
		kMidiCountDTasks,
		kMidiFlushDTasks,
		kMidiExec1DTask,

		kMidiShare,

		kMidiGetCommand,
		kMidiGetDTask,

		kMidiNewFilter,
		kMidiFreeFilter,
		kMidiAcceptChan,
		kMidiAcceptPort,
		kMidiAcceptType,
		kMidiIsAcceptedChan,
		kMidiIsAcceptedPort,
		kMidiIsAcceptedType,

		kMidiReset,
	
		/* release 1.80 */		
		kMidiRegisterDriver,
		kMidiUnregisterDriver,
		kMidiCountDrivers,
		kMidiGetIndDriver,
		kMidiGetDriverInfos,
		kMidiAddSlot,
		kMidiGetIndSlot,
		kMidiRemoveSlot,
		kMidiGetSlotInfos,
		kMidiConnectSlot,
		kMidiIsSlotConnected,

		/* release 1.82 */		
		kMidiSetSlotName,

		kMaxMth
};


/* date structures  for ioctl calls */ 

typedef struct SMidiGetVersionArgs 
{
  /* result */
  int version;
  
} TMidiGetVersionArgs;



typedef struct SMidiCountApplsArgs 
{
  /* result */
  int count;
  
} TMidiCountApplsArgs;


typedef struct SMidiGetIndApplArgs 
{
  /* result */
  int refnum;
  
  /* input args */
  int index;
  
} TMidiGetIndApplArgs;
typedef TMidiGetIndApplArgs TMidiGetIndDriverArgs;

typedef struct SMidiGetNamedApplArgs 
{
  /* result */
  int refnum;
  
  /* input args */
  MidiName name;
   
} TMidiGetNamedApplArgs;


typedef struct  TSyncInfo TMidiGetSyncInfoArgs;


typedef struct SMidiSetSyncModeArgs 
{
  
  /* input args */
  unsigned short mode;
  
} TMidiSetSyncModeArgs;


typedef struct SMidiGetExtTimeArgs 
{
  
  /* result */
  long time;
  
}TMidiGetExtTimeArgs;


typedef struct SMidiInt2ExtTimeArgs 
{
  /* input args */
  long datein;
  
  /* result */
  long dateout;
  
}TMidiInt2ExtTimeArgs;


typedef TMidiInt2ExtTimeArgs TMidiExt2IntTimeArgs;


typedef struct SMidiTime2SmpteArgs 
{
  /* input args */
  long time;
  short format;
  SmpteLocPtr loc;
  
  
}TMidiTime2SmpteArgs;


typedef struct SMidiSmpte2TimeArgs 
{
  /* input args */
   SmpteLocPtr loc;
  
  /* result */
  long time;
  
  
}TMidiSmpte2TimeArgs;



typedef struct SMidiGetTimeAddrArgs 
{
  /* result */
  long* timeadr;

} TMidiGetTimeAddrArgs;



typedef struct SMidiOpenArgs 
{
  /* result */
  int refnum;
  
  /* input args */
  MidiName name;
} TMidiOpenArgs;


typedef struct SMidiRegisterDriver 
{
  /* result */
  short refnum;
  /* input args */
  TDriverInfos  infos;
  TDriverOperation op;
} TMidiRegisterDriverArgs;



typedef struct SMidiCloseArgs 
{
  /* no result */
  
  /* input args */
  int refnum;

} TMidiCloseArgs;


typedef struct SMidiGetDriverInfosArgs 
{
  /* result */
  Boolean 		result;
  /* input args */
  short 		refnum;
  TDriverInfos	infos;

} TMidiGetDriverInfosArgs;


typedef struct SMidiAddSlotArgs 
{
  /* result */
  SlotRefNum slotRef;
  /* input args */
  short refnum;
  MidiName name;
  SlotDirection direction;

} TMidiAddSlotArgs;


typedef struct SMidiSetSlotNameArgs 
{
  /* result */
  /* input args */
  SlotRefNum slotRef;
  MidiName name;

} TMidiSetSlotNameArgs;


typedef struct SMidiGetIndSlotArgs 
{
  /* result */
  SlotRefNum slotRef;
  /* input args */
  short refnum;
  short index;

} TMidiGetIndSlotArgs;


typedef struct SMidiGetSlotInfosArgs 
{
  /* result */
  Boolean result;
  /* input args */
  SlotRefNum slotRef;
  TSlotInfos infos;

} TMidiGetSlotInfosArgs;


typedef struct SMidiConnectSlotArgs 
{
  /* result */
  /* input args */
  SlotRefNum slotRef;
  short 	port;
  Boolean 	state;

} TMidiConnectSlotArgs;


typedef struct SMidiIsSlotConnectedArgs 
{
  /* result */
  Boolean 	result;
  /* input args */
  SlotRefNum slotRef;
  short 	port;

} TMidiIsSlotConnectedArgs;


typedef struct SMidiGetNameArgs 
{
  /* input args */
  int refnum;
  
  /* res */
  MidiName name;
  //MSName  name;

} TMidiGetNameArgs;

typedef TMidiOpenArgs TMidiSetNameArgs;

typedef struct SMidiGetInfoArgs 
{
  /* inpout args */
  int refnum;
  
  /* res */
  void* info;

} TMidiGetInfoArgs;

typedef TMidiGetInfoArgs TMidiSetInfoArgs;


typedef struct SMidiGetFilterArgs 
{
  /* input args */
  int refnum;
  
  /* res */
  MidiFilterPtr filter;

} TMidiGetFilterArgs;


typedef TMidiGetFilterArgs  TMidiSetFilterArgs;


typedef struct SMidiGetRcvAlarmArgs 
{
  /* input args */
  int refnum;
  
  /* result */
  RcvAlarmPtr alarm;

} TMidiGetRcvAlarmArgs;


typedef TMidiGetRcvAlarmArgs   TMidiSetRcvAlarmArgs ;


typedef struct SMidiGetApplAlarmArgs 
{
  /* input args */
  int refnum;
  
  /* res */
  ApplAlarmPtr alarm;

} TMidiGetApplAlarmArgs;

typedef TMidiGetApplAlarmArgs   TMidiSetApplAlarmArgs ;


typedef struct SMidiConnectArgs 
{
  /* input args */
  int src;
  int dest;
  Boolean state;

} TMidiConnectArgs;

typedef TMidiConnectArgs TMidiIsConnectedArgs;


typedef struct SMidiGetPortStateArgs 
{
  /* input args */
  int port;
 /* result */
  Boolean state;

} TMidiGetPortStateArgs;

typedef TMidiGetPortStateArgs TMidiSetPortStateArgs;


typedef struct SMidiGetTimeArgs 
{
  /* result */
  unsigned long time;

} TMidiGetTimeArgs;



typedef struct SMidiSendImArgs 
{
   /* input args */
  int ref;
  MidiEvPtr ev;
 
} TMidiSendImArgs;


typedef TMidiSendImArgs  TMidiSendArgs; 
typedef TMidiSendImArgs  TMidiSendAtArgs; 



typedef struct SMidiCountEvsArgs 
{
   /* input args */
  int refnum;
  /* result */
  long evs;
 
} TMidiCountEvsArgs;



typedef struct SMidiGetEvArgs 
{
  long		r;	/* -- reference number					*/
  long 		d;	/* -- number of available cells 		*/
  MidiEvPtr	l;	/* -- list of availables cells			*/
  MidiEvPtr	e;	/* -- read event						*/
  long		u;	/* -- number of used cells				*/
} TMidiGetEvArgs;


typedef TMidiGetEvArgs TMidiAvailEvArgs;

typedef TMidiCloseArgs TMidiFlushEvsArgs;

typedef struct SMidiReadSyncArgs 
{
   /* input args */
  void* adr;
  /* result */
  void* res;
  
 
} TMidiReadSyncArgs;


typedef struct SMidiWriteSyncArgs 
{
   /* input args */
  void* adr;
  void* val;
  /* result */
  void* res;
  
 
} TMidiWriteSyncArgs;


typedef struct SMidiTaskArgs 
{
  /* input args */
  int refnum;
  MidiEvPtr utask;
  /* result */
  MidiEvPtr ktask;
  
} TMidiTaskArgs;

typedef TMidiTaskArgs TMidiDTaskArgs;

typedef struct SMidiForgetTaskArgs 
{
  /* input args */
  MidiEvPtr* taskptr;
   /* result */
  MidiEvPtr utask;
   
} TMidiForgetTaskArgs;


typedef struct SMidiCountDTasksArgs 
{
  /* input args */
  int refnum;
  /* result */
  long res;
  
} TMidiCountDTasksArgs;


typedef TMidiCloseArgs TMidiFlushDTasksArgs ;
typedef TMidiCloseArgs TMidiExec1DTaskArgs ;


typedef struct SMidiNewFilterArgs 
{
  /* input args */
 
  /* result */
  MidiFilterPtr filter;
  
} TMidiNewFilterArgs;

typedef TMidiNewFilterArgs TMidiFreeFilterArgs ;



typedef struct SMidiFilterGenericArgs 
{
  /* input args */
  int refnum;
  MidiFilterPtr filter;
  int generic;
  Boolean state;
  
  /* result */
   
} TMidiFilterGenericArgs;

typedef TMidiFilterGenericArgs TMidiAcceptChanArgs ;
typedef TMidiFilterGenericArgs TMidiAcceptPortArgs ;
typedef TMidiFilterGenericArgs TMidiAcceptTypeArgs ;

typedef TMidiFilterGenericArgs TMidiIsAcceptedChanArgs ;
typedef TMidiFilterGenericArgs TMidiIsAcceptedPortArgs ;
typedef TMidiFilterGenericArgs TMidiIsAcceptedTypeArgs ;

#endif
