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


#include "msDefs.h"

#define MaxApplNameLen   32            /* maximum application name length */

typedef  char MSName[MaxApplNameLen];  /* A REVOIR */

#define kMidiShareName "MidiShareDev"
#define kMidiShareMajor 151

/* functions index for ioctl */

#define kMidiGetVersion 	1

#define kMidiCountAppls 	2
#define kMidiGetIndAppl		3
#define kMidiGetNamedAppl	4

#define kMidiGetSyncInfo	5
#define kMidiSetSyncMode 	6
#define kMidiGetExtTime     	8   
#define kMidiInt2ExtTime	9
#define kMidiExt2IntTime	10
#define kMidiTime2Smpte		11
#define kMidiSmpte2Time		12
#define kMidiGetTimeAddr	13

#define kMidiOpen 		14
#define kMidiClose 		15

#define kMidiGetName 		16
#define kMidiSetName		17
#define kMidiGetInfo		18
#define kMidiSetInfo		19
#define kMidiGetFilter		20
#define kMidiSetFilter		21
#define kMidiGetRcvAlarm	22
#define kMidiSetRcvAlarm	23
#define kMidiGetApplAlarm	24
#define kMidiSetApplAlarm 	25

#define kMidiConnect		26
#define kMidiIsConnected	27

#define kMidiGetPortState	28
#define kMidiSetPortState	29

#define kMidiFreeSpace		30
#define kMidiTotalSpace		31
#define kMidiGrowSpace		32

#define kMidiNewCell		33
#define kMidiFreeCell		34
#define kMidiNewEv 		35
#define kMidiCopyEv		36
#define kMidiFreeEv		37
#define kMidiSetField		38
#define kMidiGetField		39
#define kMidiAddField		40
#define kMidiCountFields	41

#define kMidiNewSeq 		42
#define kMidiAddSeq 		43
#define kMidiFreeSeq		44
#define kMidiClearSeq		45
#define kMidiApplySeq		46

#define kMidiGetTime 		47

#define kMidiSendIm 		48
#define kMidiSend		49
#define kMidiSendAt			50

#define kMidiCountEvs		51
#define kMidiGetEv			52
#define kMidiAvailEv		53
#define kMidiFlushEvs 		54

#define kMidiReadSync		55
#define kMidiWriteSync		56

#define kMidiCall			57
#define kMidiTask			58
#define kMidiDTask 			59
#define kMidiForgetTask		60
#define kMidiCountDTasks	61
#define kMidiFlushDTasks	62
#define kMidiExec1DTask 	63

#define kMidiShare		64

#define kMidiGetCommand 	65
#define kMidiGetDTask 		66

#define kMidiNewFilter 		67
#define kMidiFreeFilter 	68
#define kMidiAcceptChan 	69
#define kMidiAcceptPort		70
#define kMidiAcceptType		71
#define kMidiIsAcceptedChan 	72
#define kMidiIsAcceptedPort 	73
#define kMidiIsAcceptedType 	74

#define kMidiReset 		75
#define kMidiQuit 		76


#define kMaxMth			77


/* date structures  for ioctl calls */ 

typedef struct SMidiGetVersionArgs 
{
  /* result */
  int version;
  
} TMidiGetVersionArgs;



typedef struct SMidiCountApplsArgs 
{
  /* result */
  int appls;
  
} TMidiCountApplsArgs;



typedef struct SMidiGetIndApplArgs 
{
  /* result */
  int refnumres;
  
  /* input args */
  int refnumarg;
  
} TMidiGetIndApplArgs;


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



typedef struct SMidiCloseArgs 
{
  /* no result */
  
  /* input args */
  int refnum;

} TMidiCloseArgs;


typedef struct SMidiGetNameArgs 
{
  /* inpout args */
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



