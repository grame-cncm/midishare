/*

  Copyright © Grame 1999-2001

  This program is free software; you can redistribute it and modify it under 
  the terms of the GNU General Public License as published by the 
  Free Software Foundation version 2 of the License.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for 
  more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

  Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
  grame@grame.fr

*/

#ifndef __MidiSharePPC_68k__
#define __MidiSharePPC_68k__

#include <MixedMode.h>
#include "MidiShareDefs.h"

/*----------------------------- Alarms prototypes ----------------------------*/
typedef UniversalProcPtr UPPTaskPtr;
typedef UniversalProcPtr UPPRcvAlarmPtr;
typedef UniversalProcPtr UPPApplAlarmPtr;
typedef UniversalProcPtr UPPApplyProcPtr;
typedef UniversalProcPtr UPPWakeupProcPtr;
typedef UniversalProcPtr UPPSleepProcPtr;

enum {
	
	uppTaskPtrProcInfo = kPascalStackBased
		 | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(long)))
		 | STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(short)))
		 | STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(long)))
		 | STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(long)))
		 | STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(long)))
,
		 
	uppRcvAlarmPtrProcInfo = kPascalStackBased
		 | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(short)))
,		 
	
	uppApplAlarmPtrProcInfo = kPascalStackBased
		 | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(short)))
		 | STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(long)))		 
,	 
		 
	uppApplyProcPtrProcInfo = kPascalStackBased
		 | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(MidiEvPtr)))
,	 
		 
	uppWakeupProcPtrProcInfo = kThinkCStackBased
		 | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(short)))
,	 
		 
	uppSleepProcPtrProcInfo = kThinkCStackBased
		 | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(short)))
};

#define NewTaskPtr(userRoutine)		\
		(UPPTaskPtr) NewRoutineDescriptor((ProcPtr)(userRoutine), uppTaskPtrProcInfo, GetCurrentISA())

#define NewRcvAlarmPtr(userRoutine)		\
		(UPPRcvAlarmPtr) NewRoutineDescriptor((ProcPtr)(userRoutine), uppRcvAlarmPtrProcInfo, GetCurrentISA())

#define NewApplAlarmPtr(userRoutine)		\
		(UPPApplAlarmPtr) NewRoutineDescriptor((ProcPtr)(userRoutine), uppApplAlarmPtrProcInfo, GetCurrentISA())

#define NewApplyProcPtr(userRoutine)		\
		(UPPApplyProcPtr) NewRoutineDescriptor((ProcPtr)(userRoutine), uppApplyProcPtrProcInfo, GetCurrentISA())

#define NewWakeupProcPtr(userRoutine)		\
		(UPPWakeupProcPtr) NewRoutineDescriptor((ProcPtr)(userRoutine), uppWakeupProcPtrProcInfo, GetCurrentISA())

#define NewSleepProcPtr(userRoutine)		\
		(UPPSleepProcPtr) NewRoutineDescriptor((ProcPtr)(userRoutine), uppSleepProcPtrProcInfo, GetCurrentISA())

/* use:

definition
----------

pascal void MyTask ( long date, short refNum, long a1,long a2,long a3 );
UPPTaskPtr UPPMyTask = NewTaskPtr(MyTask);


call
-----
MidiTask(UPPMyTask , .....)

*/


/*******************************************************************************
* 								ENTRY POINTS									
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#define RESULT_SIZE_MSH(sizeCode) 	((long)(sizeCode) << kResultSizePhase)


enum {
	uppProcInfoMidiShareSpecialInit = kPascalStackBased
	
,

	uppProcInfoMidiGetSyncInfo = kPascalStackBased
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(SyncInfoPtr)))
,
	
	uppProcInfoMidiGetIndAppl = kPascalStackBased
						| RESULT_SIZE (SIZE_CODE(sizeof(short)))
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(short)))
,
	uppProcInfoMidiTime2Smpte = kPascalStackBased
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(long)))
						| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(short)))
						| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(SmpteLocPtr)))
,
	uppProcInfoMidiSmpte2Time = kPascalStackBased
						| RESULT_SIZE (SIZE_CODE(sizeof(long)))
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(SmpteLocPtr)))
,						
	uppProcInfoMidiGetTimeAddr = kPascalStackBased
						| RESULT_SIZE (SIZE_CODE(sizeof(long*)))
,						
	uppProcInfoMidiGetName = kPascalStackBased
						| RESULT_SIZE (SIZE_CODE(sizeof(MidiName)))
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(short)))
,						
	uppProcInfoMidiSetName = kPascalStackBased
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(short)))
						| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(MidiName)))
,						
	uppProcInfoMidiGetInfo = kPascalStackBased
						| RESULT_SIZE (SIZE_CODE(sizeof(void*)))
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(short)))
,						
	uppProcInfoMidiSetInfo = kPascalStackBased
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(short)))
						| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(void*)))
,						
	uppProcInfoMidiGetFilter = kPascalStackBased
						| RESULT_SIZE (SIZE_CODE(sizeof(MidiFilterPtr)))
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(short)))
,
	uppProcInfoMidiSetFilter = kPascalStackBased
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(short)))
						| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(MidiFilterPtr)))
,						
	uppProcInfoMidiGetRcvAlarm = kPascalStackBased
						| RESULT_SIZE (SIZE_CODE(sizeof(UPPRcvAlarmPtr)))
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(short)))
,
	uppProcInfoMidiSetRcvAlarm = kPascalStackBased
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(short)))
						| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(UPPRcvAlarmPtr)))
,
	uppProcInfoMidiGetApplAlarm = kPascalStackBased
						| RESULT_SIZE (SIZE_CODE(sizeof(UPPApplAlarmPtr)))
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(short)))
,
	uppProcInfoMidiSetApplAlarm = kPascalStackBased
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(short)))
						| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(UPPApplAlarmPtr)))
,	
	uppProcInfoMidiConnect = kPascalStackBased
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(short)))
						| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(short)))
						| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(Boolean)))
,						
	uppProcInfoMidiIsConnected = kPascalStackBased
						| RESULT_SIZE (SIZE_CODE(sizeof(Boolean)))
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(short)))
						| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(short)))
,	
	uppProcInfoMidiGetPortState = kPascalStackBased
						| RESULT_SIZE (SIZE_CODE(sizeof(Boolean)))
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(short)))
						
,	
	uppProcInfoMidiSetPortState = kPascalStackBased
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(short)))
						| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(Boolean)))
,						
	uppProcInfoMidiNewCell = kPascalStackBased
						| RESULT_SIZE (SIZE_CODE(sizeof(MidiEvPtr)))
,
	uppProcInfoMidiNewEv = kPascalStackBased
						| RESULT_SIZE (SIZE_CODE(sizeof(MidiEvPtr)))
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(short)))
,
	uppProcInfoMidiCopyEv = kPascalStackBased
						| RESULT_SIZE (SIZE_CODE(sizeof(MidiEvPtr)))
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(MidiEvPtr)))
,
	uppProcInfoMidiSetField = kPascalStackBased
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(MidiEvPtr)))
						| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(long)))
						| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(long)))
,
	uppProcInfoMidiGetField = kPascalStackBased
						| RESULT_SIZE (SIZE_CODE(sizeof(long)))
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(MidiEvPtr)))
						| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(long)))
,	
	uppProcInfoMidiAddField = kPascalStackBased
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(MidiEvPtr)))
						| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(long)))
,				
	uppProcInfoMidiCountFields = kPascalStackBased
						| RESULT_SIZE (SIZE_CODE(sizeof(long)))
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(MidiEvPtr)))
,
	uppProcInfoMidiNewSeq = kPascalStackBased
						| RESULT_SIZE (SIZE_CODE(sizeof(MidiSeqPtr)))
,						
	uppProcInfoMidiAddSeq = kPascalStackBased
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(MidiSeqPtr)))
						| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(MidiEvPtr)))
,
	uppProcInfoMidiApplySeq = kPascalStackBased
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(MidiSeqPtr)))
						| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(UPPApplyProcPtr)))
,
	uppProcInfoMidiSendAt = kPascalStackBased
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(short)))
						| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(MidiEvPtr)))
						| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(long)))						
,
	uppProcInfoMidiReadSync = kPascalStackBased
						| RESULT_SIZE (SIZE_CODE(sizeof(void*)))
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(void*)))
,						
	uppProcInfoMidiWriteSync = kPascalStackBased
						| RESULT_SIZE (SIZE_CODE(sizeof(void*)))
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(void*)))
						| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(void*)))
,						
	uppProcInfoMidiCall = kPascalStackBased
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(UPPTaskPtr)))
						| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(long)))
						| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(short)))
						| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(long)))
						| STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(long)))
						| STACK_ROUTINE_PARAMETER(6, SIZE_CODE(sizeof(long)))
,						
	uppProcInfoMidiForgetTask = kPascalStackBased
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(MidiEvPtr*)))
						
	
				
};

/* descripteurs utilisŽs par plusieurs fonctions */

enum {

	/* MidiGetVersion , MidiCountAppls */
	
	uppProcInfoType1 = kPascalStackBased
						| RESULT_SIZE (SIZE_CODE(sizeof(short)))
	,					
	/* MidiSetSyncMode , MidiFlushEvs,MidiClose,MidiFlushDTasks,MidiExec1DTask */
	
	uppProcInfoType2 = kPascalStackBased
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(short)))
	,					
	/* MidiGetExtTime, MidiFreeSpace,MidiTotalSpace ,MidiGetTime */
	
	uppProcInfoType3 = kPascalStackBased
						| RESULT_SIZE (SIZE_CODE(sizeof(long)))
	,					
	/* MidiInt2ExtTime, MidiExtInt2Time,MidiGrowSpace  */
	
	uppProcInfoType4 = kPascalStackBased
						| RESULT_SIZE (SIZE_CODE(sizeof(long)))
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(long)))
	,
	/* MidiGetNamedAppl, MidiOpen  */	
				
	uppProcInfoType5 = kPascalStackBased
						| RESULT_SIZE (SIZE_CODE(sizeof(short)))
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(MidiName)))
						
	,
	/* MidiFreeCell, MidiFreeEv  */					
	uppProcInfoType6 = kPascalStackBased
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(MidiEvPtr)))

	,
	/* MidiFreeSeq, MidiClearSeq  */					
	uppProcInfoType7 = kPascalStackBased
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(MidiSeqPtr)))

	,
	/* MidiSendIm, MidiSend  */					
	uppProcInfoType8 = kPascalStackBased
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(short)))
						| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(MidiEvPtr)))

	,
	/* MidiGetEv, MidiAvailEv  */					
	uppProcInfoType9 = kPascalStackBased
						| RESULT_SIZE (SIZE_CODE(sizeof(MidiEvPtr)))
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(short)))
					
	,				
	/* MidiTask, MidiDTask */	
	uppProcInfoType10 = kPascalStackBased
						| RESULT_SIZE (SIZE_CODE(sizeof(MidiEvPtr)))
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(UPPTaskPtr)))
						| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(long)))
						| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(short)))
						| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(long)))
						| STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(long)))
						| STACK_ROUTINE_PARAMETER(6, SIZE_CODE(sizeof(long)))
	,					
	/* MidiCountEvs, MidiCountDTasks  */					
	uppProcInfoType11 = kPascalStackBased
						| RESULT_SIZE (SIZE_CODE(sizeof(long)))
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(short)))
	,					
	/*MidiRegisterDriver  */					
	uppProcInfoType12 = kPascalStackBased
						| RESULT_SIZE (SIZE_CODE(sizeof(short)))
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(TDriverInfos *)))
						| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(TDriverOperation *)))
	,					
	/*MidiUnregisterDriver  */					
	uppProcInfoType13 = kPascalStackBased
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(short)))
	,			
	/*MidiCountDrivers  */					
	uppProcInfoType14 = kPascalStackBased
						| RESULT_SIZE (SIZE_CODE(sizeof(short)))
	,					
	/*MidiGetIndDriver  */					
	uppProcInfoType15 = kPascalStackBased
						| RESULT_SIZE (SIZE_CODE(sizeof(short)))
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(short)))
	,					
	/*MidiGetDriverInfos  */					
	uppProcInfoType16 = kPascalStackBased
						| RESULT_SIZE (SIZE_CODE(sizeof(Boolean)))
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(short)))
						| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(TDriverInfos *)))
	,					
	/* MidiAddSlot  */					
	uppProcInfoType17 = kPascalStackBased
						| RESULT_SIZE (SIZE_CODE(sizeof(SlotRefNum)))
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(short)))
						| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(char *)))
						| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(SlotDirection)))
	,
	/* MidiGetIndSlot  */					
	uppProcInfoType18 = kPascalStackBased
						| RESULT_SIZE (SIZE_CODE(sizeof(SlotRefNum)))
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(short)))
						| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(short)))
	,
	/* MidiRemoveSlot  */					
	uppProcInfoType19 = kPascalStackBased
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(SlotRefNum)))
	,
	/* MidiGetSlotInfos  */					
	uppProcInfoType20 = kPascalStackBased
						| RESULT_SIZE (SIZE_CODE(sizeof(Boolean)))
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(SlotRefNum)))
						| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(TSlotInfos *)))
	,
	/* MidiConnectSlot  */					
	uppProcInfoType21 = kPascalStackBased
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(short)))
						| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(SlotRefNum)))
						| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(Boolean)))
	,
	/* MidiIsSlotConnected  */					
	uppProcInfoType22 = kPascalStackBased
						| RESULT_SIZE (SIZE_CODE(sizeof(Boolean)))
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(short)))
						| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(SlotRefNum)))
	,
	/* MidiSetSlotName  */					
	uppProcInfoType23 = kPascalStackBased
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(SlotRefNum)))
						| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(char *)))
	,					
	/* MidiNewFilter */
	uppProcInfoType24 = kPascalStackBased
						| RESULT_SIZE (SIZE_CODE(sizeof(MidiFilterPtr)))
	,
	/* MidiFreeFilter */
	uppProcInfoType25 = kPascalStackBased
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(MidiFilterPtr)))
	,					
	/* MidiAcceptPort, MidiAcceptChan, MidiAcceptType */
	uppProcInfoType26 = kPascalStackBased
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(MidiFilterPtr)))
						| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(short)))
						| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(Boolean)))

	,					
	/* MidiIsAcceptedPort, MidiIsAcceptedChan, MidiIsAcceptedType */
	uppProcInfoType27 = kPascalStackBased
						| RESULT_SIZE (SIZE_CODE(sizeof(Boolean)))
						| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(MidiFilterPtr)))
						| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(short)))
					
				
};


/*--------------------------- Global MidiShare environment --------------------*/

static short CodeMidiShareSpecialInit[]	={0x7037, 0x2078, 0x00B8, 0x4ED0};
static short CodeMidiGetVersion[]		={0x7000, 0x2078, 0x00B8, 0x4ED0};

static short CodeMidiCountAppls[]		={0x7001, 0x2078, 0x00B8, 0x4ED0};
static short CodeMidiGetIndAppl[] 		={0x7002, 0x2078, 0x00B8, 0x4ED0};		
static short CodeMidiGetNamedAppl[]		={0x7003, 0x2078, 0x00B8, 0x4ED0};
 
/*----------------------------- SMPTE synchronization -------------------------*/

static short CodeMidiGetSyncInfo[]		={0x7038, 0x2078, 0x00B8, 0x4ED0};
static short CodeMidiSetSyncMode[]		={0x7039, 0x2078, 0x00B8, 0x4ED0};
static short CodeMidiGetExtTime[]		={0x703D, 0x2078, 0x00B8, 0x4ED0};						
static short CodeMidiInt2ExtTime[]		={0x703E, 0x2078, 0x00B8, 0x4ED0};						
static short CodeMidiExt2IntTime[]		={0x703F, 0x2078, 0x00B8, 0x4ED0};						
static short CodeMidiTime2Smpte[] 		={0x7040, 0x2078, 0x00B8, 0x4ED0};						
static short CodeMidiSmpte2Time[]		={0x7041, 0x2078, 0x00B8, 0x4ED0};						
static short CodeMidiGetTimeAddr[]		={0x7042, 0x2078, 0x00B8, 0x4ED0};						


/*----------------------------- Open / close application ----------------------*/

static short CodeMidiOpen[] 			={0x7004, 0x2078, 0x00B8, 0x4ED0};
static short CodeMidiClose[] 			={0x7005, 0x2078, 0x00B8, 0x4ED0};	


/*--------------------------- Application configuration -----------------------*/

static short CodeMidiGetName[]			={0x7006, 0x2078, 0x00B8, 0x4ED0};
static short CodeMidiSetName[] 			={0x7007, 0x2078, 0x00B8, 0x4ED0};
static short CodeMidiGetInfo[] 			={0x7008, 0x2078, 0x00B8, 0x4ED0};	
static short CodeMidiSetInfo[]			={0x7009, 0x2078, 0x00B8, 0x4ED0};
static short CodeMidiGetFilter[] 		={0x700A, 0x2078, 0x00B8, 0x4ED0};
static short CodeMidiSetFilter[]		={0x700B, 0x2078, 0x00B8, 0x4ED0};
static short CodeMidiGetRcvAlarm[]		={0x700C, 0x2078, 0x00B8, 0x4ED0};		
static short CodeMidiSetRcvAlarm[] 		={0x700D, 0x2078, 0x00B8, 0x4ED0};	
static short CodeMidiGetApplAlarm[]		={0x700E, 0x2078, 0x00B8, 0x4ED0};		
static short CodeMidiSetApplAlarm[]		={0x700F, 0x2078, 0x00B8, 0x4ED0};


/*------------------------- Inter-Application Connections ---------------------*/

static short CodeMidiConnect[] 			={0x7010, 0x2078, 0x00B8, 0x4ED0};
static short CodeMidiIsConnected[]		={0x7011, 0x2078, 0x00B8, 0x4ED0};


/*-------------------------------- Serial ports -------------------------------*/

static short CodeMidiGetPortState[]		={0x7012, 0x2078, 0x00B8, 0x4ED0};	
static short CodeMidiSetPortState[]		={0x7013, 0x2078, 0x00B8, 0x4ED0};


/*-------------------------- Events and memory managing -----------------------*/

static short CodeMidiFreeSpace[]		={0x7014, 0x2078, 0x00B8, 0x4ED0};						

static short CodeMidiNewCell[]			={0x7033, 0x2078, 0x00B8, 0x4ED0};			
static short CodeMidiFreeCell[] 		={0x7034, 0x2078, 0x00B8, 0x4ED0};					
static short CodeMidiTotalSpace[]		={0x7035, 0x2078, 0x00B8, 0x4ED0};
static short CodeMidiGrowSpace[]		={0x7036, 0x2078, 0x00B8, 0x4ED0};

static short CodeMidiNewEv[]			={0x7015, 0x2078, 0x00B8, 0x4ED0};			
static short CodeMidiCopyEv[]			={0x7016, 0x2078, 0x00B8, 0x4ED0};			
static short CodeMidiFreeEv[]			={0x7017, 0x2078, 0x00B8, 0x4ED0};					


static short CodeMidiSetField[] 		={0x703A, 0x2078, 0x00B8, 0x4ED0};
static short CodeMidiGetField[]			={0x703B, 0x2078, 0x00B8, 0x4ED0};
static short CodeMidiAddField[] 		={0x701A, 0x2078, 0x00B8, 0x4ED0};
static short CodeMidiCountFields[]		={0x703C, 0x2078, 0x00B8, 0x4ED0};


/*------------------------------- Sequence managing ---------------------------*/

static short CodeMidiNewSeq[]			={0x701D, 0x2078, 0x00B8, 0x4ED0};				 			
static short CodeMidiAddSeq[] 			={0x701E, 0x2078, 0x00B8, 0x4ED0};
static short CodeMidiFreeSeq[] 			={0x701F, 0x2078, 0x00B8, 0x4ED0};		
static short CodeMidiClearSeq[]			={0x7020, 0x2078, 0x00B8, 0x4ED0};			
static short CodeMidiApplySeq[]			={0x7021, 0x2078, 0x00B8, 0x4ED0}; 


/*------------------------------------- Time ----------------------------------*/

static short CodeMidiGetTime[]			={0x7022, 0x2078, 0x00B8, 0x4ED0};		


/*------------------------------------ Sending --------------------------------*/

static short CodeMidiSendIm[]			={0x7023, 0x2078, 0x00B8, 0x4ED0};
static short CodeMidiSend[] 			={0x7024, 0x2078, 0x00B8, 0x4ED0};
static short CodeMidiSendAt[]			={0x7025, 0x2078, 0x00B8, 0x4ED0};	


/*------------------------------------ Receving -------------------------------*/

static short CodeMidiCountEvs[]			={0x7026, 0x2078, 0x00B8, 0x4ED0};
static short CodeMidiGetEv[]			={0x7027, 0x2078, 0x00B8, 0x4ED0};
static short CodeMidiAvailEv[]			={0x7028, 0x2078, 0x00B8, 0x4ED0};
static short CodeMidiFlushEvs[]			={0x7029, 0x2078, 0x00B8, 0x4ED0};	


/*----------------------------------- Mail boxes ------------------------------*/

static short CodeMidiReadSync[]			={0x702A, 0x2078, 0x00B8, 0x4ED0};
static short CodeMidiWriteSync[]		={0x702B, 0x2078, 0x00B8, 0x4ED0};


/*---------------------------------- Task Managing ----------------------------*/

static short CodeMidiCall[]				={0x702C, 0x2078, 0x00B8, 0x4ED0};
static short CodeMidiTask[]				={0x702D, 0x2078, 0x00B8, 0x4ED0};
static short CodeMidiDTask[] 			={0x702E, 0x2078, 0x00B8, 0x4ED0};
static short CodeMidiForgetTask[]		={0x702F, 0x2078, 0x00B8, 0x4ED0};
static short CodeMidiCountDTasks[]		={0x7030, 0x2078, 0x00B8, 0x4ED0};
static short CodeMidiFlushDTasks[]		={0x7031, 0x2078, 0x00B8, 0x4ED0};
static short CodeMidiExec1DTask[]		={0x7032, 0x2078, 0x00B8, 0x4ED0};

/*---------------------------------- Filter ----------------------------*/

static short CodeMidiNewFilter[]		={0x7051, 0x2078, 0x00B8, 0x4ED0};
static short CodeMidiFreeFilter[]		={0x7052, 0x2078, 0x00B8, 0x4ED0};
static short CodeMidiAcceptPort[] 		={0x7053, 0x2078, 0x00B8, 0x4ED0};
static short CodeMidiAcceptChan[]		={0x7054, 0x2078, 0x00B8, 0x4ED0};
static short CodeMidiAcceptType[]		={0x7055, 0x2078, 0x00B8, 0x4ED0};
static short CodeMidiIsAcceptedPort[]	={0x7056, 0x2078, 0x00B8, 0x4ED0};
static short CodeMidiIsAcceptedChan[]	={0x7057, 0x2078, 0x00B8, 0x4ED0};
static short CodeMidiIsAcceptedType[]	={0x7058, 0x2078, 0x00B8, 0x4ED0};


/*------------------------------- Drivers management --------------------------*/

static short CodeRegisterDriver[]   	={0x7044, 0x2078, 0x00B8, 0x4ED0};
static short CodeUnregisterDriver[]		={0x7045, 0x2078, 0x00B8, 0x4ED0};
static short CodeCountDrivers[]			={0x7046, 0x2078, 0x00B8, 0x4ED0};
static short CodeGetIndDriver[]			={0x7047, 0x2078, 0x00B8, 0x4ED0};
static short CodeGetDriverInfos[] 		={0x7048, 0x2078, 0x00B8, 0x4ED0};


/*-------------------------------- Slots management ---------------------------*/

static short CodeAddSlot[]				={0x7050, 0x2078, 0x00B8, 0x4ED0};
static short CodeGetIndSlot[]			={0x704A, 0x2078, 0x00B8, 0x4ED0};
static short CodeRemoveSlot[] 			={0x704B, 0x2078, 0x00B8, 0x4ED0};
static short CodeGetSlotInfos[] 		={0x704C, 0x2078, 0x00B8, 0x4ED0};
static short CodeConnectSlot[]			={0x704D, 0x2078, 0x00B8, 0x4ED0};
static short CodeIsSlotConnected[]		={0x704E, 0x2078, 0x00B8, 0x4ED0};
static short CodeSetSlotName[]			={0x704F, 0x2078, 0x00B8, 0x4ED0};


/*-----------------MidiShare -------------------------------*/

#define MidiShare() 	( **((long**)0xB8) == 0xD080D080 )


/*--------------------------- Global MidiShare environment --------------------*/

#define MidiShareSpecialInit()		\
	CallUniversalProc((UniversalProcPtr)CodeMidiShareSpecialInit, uppProcInfoMidiShareSpecialInit)

#define MidiGetVersion()		\
	(short)CallUniversalProc((UniversalProcPtr)CodeMidiGetVersion, uppProcInfoType1)

#define MidiCountAppls()		\
	(short)CallUniversalProc((UniversalProcPtr)CodeMidiCountAppls, uppProcInfoType1)

#define MidiGetIndAppl(ind)		\
	(short)CallUniversalProc((UniversalProcPtr)CodeMidiGetIndAppl, uppProcInfoMidiGetIndAppl,(ind))

#define MidiGetNamedAppl(na)		\
	(short)CallUniversalProc((UniversalProcPtr)CodeMidiGetNamedAppl, uppProcInfoType5,(na))

/*----------------------------- SMPTE synchronization -------------------------*/

#define MidiGetSyncInfo(p)		\
	CallUniversalProc((UniversalProcPtr)CodeMidiGetSyncInfo, uppProcInfoMidiGetSyncInfo,(p))

#define MidiSetSyncMode(mode)		\
	CallUniversalProc((UniversalProcPtr)CodeMidiSetSyncMode, uppProcInfoType2,(mode))

#define MidiGetExtTime()		\
	(long)CallUniversalProc((UniversalProcPtr)CodeMidiGetExtTime, uppProcInfoType3)

#define MidiInt2ExtTime(v)		\
	(long)CallUniversalProc((UniversalProcPtr)CodeMidiInt2ExtTime, uppProcInfoType4,(v))

#define MidiExt2IntTime(v)		\
	(long)CallUniversalProc((UniversalProcPtr)CodeMidiExt2IntTime, uppProcInfoType4,(v))
		
#define MidiTime2Smpte(time,format,loc)		\
	CallUniversalProc((UniversalProcPtr)CodeMidiTime2Smpte, uppProcInfoMidiTime2Smpte,(time),(format),(loc))
	
#define MidiSmpte2Time(loc)		\
	(long)CallUniversalProc((UniversalProcPtr)CodeMidiSmpte2Time, uppProcInfoMidiSmpte2Time,(loc))

/*----------------------------- Open / close application ----------------------*/

#define MidiOpen(na)		\
	(short)CallUniversalProc((UniversalProcPtr)CodeMidiOpen, uppProcInfoType5,(na))

#define MidiClose(r)		\
	CallUniversalProc((UniversalProcPtr)CodeMidiClose, uppProcInfoType2,(r))


/*--------------------------- Application configuration -----------------------*/

#define MidiGetName(r)		\
	(MidiName)CallUniversalProc((UniversalProcPtr)CodeMidiGetName, uppProcInfoMidiGetName,(r))

#define MidiSetName(r,na)		\
	CallUniversalProc((UniversalProcPtr)CodeMidiSetName, uppProcInfoMidiSetName,(r),(na))


#define MidiGetInfo(r)		\
	(void*)CallUniversalProc((UniversalProcPtr)CodeMidiGetInfo, uppProcInfoMidiGetInfo,(r))


#define MidiSetInfo(r,in)		\
	CallUniversalProc((UniversalProcPtr)CodeMidiSetInfo, uppProcInfoMidiSetInfo,(r),(in))

		
#define MidiGetFilter(r)		\
	(MidiFilterPtr)CallUniversalProc((UniversalProcPtr)CodeMidiGetFilter, uppProcInfoMidiGetFilter,(r))
	
	
#define MidiSetFilter(r,f)		\
	CallUniversalProc((UniversalProcPtr)CodeMidiSetFilter, uppProcInfoMidiSetFilter,(r),(f))

#define MidiGetRcvAlarm(r)		\
	(UPPRcvAlarmPtr)CallUniversalProc((UniversalProcPtr)CodeMidiGetRcvAlarm, uppProcInfoMidiGetRcvAlarm,(r))
	
		
#define MidiSetRcvAlarm(r,al)		\
	CallUniversalProc((UniversalProcPtr)CodeMidiSetRcvAlarm, uppProcInfoMidiSetRcvAlarm,(r),(al))

	
#define MidiGetApplAlarm(r)		\
	(UPPApplAlarmPtr)CallUniversalProc((UniversalProcPtr)CodeMidiGetApplAlarm, uppProcInfoMidiGetApplAlarm,(r))
	

#define MidiSetApplAlarm(r,al)		\
	CallUniversalProc((UniversalProcPtr)CodeMidiSetApplAlarm, uppProcInfoMidiSetApplAlarm,(r),(al))


/*------------------------- Inter-Application Connections ---------------------*/

#define MidiConnect(s,d,st)		\
	CallUniversalProc((UniversalProcPtr)CodeMidiConnect, uppProcInfoMidiConnect,(s),(d),(st))

	
#define MidiIsConnected(s,d)		\
	(Boolean)CallUniversalProc((UniversalProcPtr)CodeMidiIsConnected, uppProcInfoMidiIsConnected,(s),(d))
	
 
/*-------------------------------- Serial ports -------------------------------*/

#define MidiGetPortState(p)		\
	(Boolean)CallUniversalProc((UniversalProcPtr)CodeMidiGetPortState, uppProcInfoMidiGetPortState,(p))


#define MidiSetPortState(p,s)		\
	CallUniversalProc((UniversalProcPtr)CodeMidiSetPortState, uppProcInfoMidiSetPortState,(p),(s))


/*-------------------------- Events and memory managing -----------------------*/

#define MidiFreeSpace()		\
	(long)CallUniversalProc((UniversalProcPtr)CodeMidiFreeSpace, uppProcInfoType3)


#define MidiNewCell()		\
	(MidiEvPtr)CallUniversalProc((UniversalProcPtr)CodeMidiNewCell, uppProcInfoMidiNewCell)

#define  MidiFreeCell(e)	\
	CallUniversalProc((UniversalProcPtr)CodeMidiFreeCell,uppProcInfoType6,(e))
	
#define  MidiTotalSpace()	\
	(long)CallUniversalProc((UniversalProcPtr)CodeMidiTotalSpace,uppProcInfoType3)
	
	
#define  MidiGrowSpace(n)	\
	(long)CallUniversalProc((UniversalProcPtr)CodeMidiGrowSpace,uppProcInfoType4,(n))
	
	
#define MidiNewEv(ty)		\
	(MidiEvPtr)CallUniversalProc((UniversalProcPtr)CodeMidiNewEv, uppProcInfoMidiNewEv,(ty))

#define MidiCopyEv(e)		\
	(MidiEvPtr)CallUniversalProc((UniversalProcPtr)CodeMidiCopyEv, uppProcInfoMidiCopyEv,(e))

#define MidiFreeEv(e)		\
	CallUniversalProc((UniversalProcPtr)CodeMidiFreeEv, uppProcInfoType6,(e))


#define MidiSetField(e,f,v)		\
	CallUniversalProc((UniversalProcPtr)CodeMidiSetField, uppProcInfoMidiSetField,(e),(f),(v))


#define MidiGetField(e,f)		\
	CallUniversalProc((UniversalProcPtr)CodeMidiGetField, uppProcInfoMidiGetField,(e),(f))


#define MidiAddField(e,v)		\
	CallUniversalProc((UniversalProcPtr)CodeMidiAddField, uppProcInfoMidiAddField,(e),(v))


#define MidiCountFields(e)		\
	(long)CallUniversalProc((UniversalProcPtr)CodeMidiCountFields, uppProcInfoMidiCountFields,(e))
	

/*------------------------------- Sequence managing ---------------------------*/

#define MidiNewSeq()		\
	(MidiSeqPtr)CallUniversalProc((UniversalProcPtr)CodeMidiNewSeq, uppProcInfoMidiNewSeq)

#define MidiAddSeq(s,e)		\
	CallUniversalProc((UniversalProcPtr)CodeMidiAddSeq, uppProcInfoMidiAddSeq,(s),(e))


#define MidiFreeSeq(s)		\
	CallUniversalProc((UniversalProcPtr)CodeMidiFreeSeq, uppProcInfoType7,(s))


#define MidiClearSeq(s)		\
	CallUniversalProc((UniversalProcPtr)CodeMidiClearSeq, uppProcInfoType7,(s))


#define MidiApplySeq(s,p)		\
	CallUniversalProc((UniversalProcPtr)CodeMidiApplySeq, uppProcInfoMidiApplySeq,(s),(p))


/*------------------------------------- Time ----------------------------------*/

#define MidiGetTime()		\
	(long)CallUniversalProc((UniversalProcPtr)CodeMidiGetTime, uppProcInfoType3)


/*------------------------------------ Sending --------------------------------*/

#define MidiSendIm(r,e)		\
	CallUniversalProc((UniversalProcPtr)CodeMidiSendIm, uppProcInfoType8,(r),(e))

#define MidiSend(r,e)		\
	CallUniversalProc((UniversalProcPtr)CodeMidiSend, uppProcInfoType8,(r),(e))

#define MidiSendAt(r,e,d)		\
	CallUniversalProc((UniversalProcPtr)CodeMidiSendAt, uppProcInfoMidiSendAt,(r),(e),(d))

/*------------------------------------ Receving -------------------------------*/

#define MidiCountEvs(r)		\
	(long)CallUniversalProc((UniversalProcPtr)CodeMidiCountEvs, uppProcInfoType11,(r))


#define MidiGetEv(r)		\
	(MidiEvPtr)CallUniversalProc((UniversalProcPtr)CodeMidiGetEv, uppProcInfoType9,(r))


#define MidiAvailEv(r)		\
	(MidiEvPtr)CallUniversalProc((UniversalProcPtr)CodeMidiAvailEv, uppProcInfoType9,(r))


#define MidiFlushEvs(r)		\
	CallUniversalProc((UniversalProcPtr)CodeMidiFlushEvs, uppProcInfoType2,(r))


/*----------------------------------- Mail boxes ------------------------------*/

#define MidiReadSync(adrMem)		\
	(void*)CallUniversalProc((UniversalProcPtr)CodeMidiReadSync, uppProcInfoMidiReadSync,(adrMem))


#define MidiWriteSync(adrMem,val)		\
	(void*)CallUniversalProc((UniversalProcPtr)CodeMidiWriteSync, uppProcInfoMidiWriteSync,(adrMem),(val))


/*---------------------------------- Task Managing ----------------------------*/

#define MidiCall(ro,d,re,a1,a2,a3)		\
	CallUniversalProc((UniversalProcPtr)CodeMidiCall, uppProcInfoMidiCall,(ro),(d),(re),(a1),(a2),(a3))


#define MidiTask(ro,d,re,a1,a2,a3)		\
	(MidiEvPtr)CallUniversalProc((UniversalProcPtr)CodeMidiTask, uppProcInfoType10,(ro),(d),(re),(a1),(a2),(a3))


#define MidiDTask(ro,d,re,a1,a2,a3)		\
	(MidiEvPtr)CallUniversalProc((UniversalProcPtr)CodeMidiDTask, uppProcInfoType10,(ro),(d),(re),(a1),(a2),(a3))


#define MidiForgetTask(e)		\
	CallUniversalProc((UniversalProcPtr)CodeMidiForgetTask, uppProcInfoMidiForgetTask,(e))


#define MidiCountDTasks(r)		\
	(long)CallUniversalProc((UniversalProcPtr)CodeMidiCountDTasks, uppProcInfoType11,(r))


#define MidiFlushDTasks(r)		\
	CallUniversalProc((UniversalProcPtr)CodeMidiFlushDTasks, uppProcInfoType2,(r))


#define MidiExec1DTask(r)		\
	CallUniversalProc((UniversalProcPtr)CodeMidiExec1DTask, uppProcInfoType2,(r))


/*---------------------------------- Filters ----------------------------*/

#define MidiNewFilter()		\
	(MidiFilterPtr)CallUniversalProc((UniversalProcPtr)CodeMidiNewFilter, uppProcInfoType24)

#define MidiFreeFilter(f)		\
	CallUniversalProc((UniversalProcPtr)CodeMidiFreeFilter, uppProcInfoType25,(f))

#define MidiAcceptPort(f,p,s)		\
	CallUniversalProc((UniversalProcPtr)CodeMidiAcceptPort, uppProcInfoType26,(f),(p),(s))

#define MidiAcceptChan(f,p,s)		\
	CallUniversalProc((UniversalProcPtr)CodeMidiAcceptChan, uppProcInfoType26,(f),(p),(s))
	
#define MidiAcceptType(f,p,s)		\
	CallUniversalProc((UniversalProcPtr)CodeMidiAcceptType, uppProcInfoType26,(f),(p),(s))

#define MidiIsAcceptedPort(f,p)		\
	CallUniversalProc((UniversalProcPtr)CodeMidiIsAcceptedPort, uppProcInfoType27,(f),(p))

#define MidiIsAcceptedChan(f,p)		\
	CallUniversalProc((UniversalProcPtr)CodeMidiIsAcceptedChan, uppProcInfoType27,(f),(p))
	
#define MidiIsAcceptedType(f,p)		\
	CallUniversalProc((UniversalProcPtr)CodeMidiIsAcceptedType, uppProcInfoType27,(f),(p))


/*-------------------------------- Drivers management ---------------------------*/

#define MidiRegisterDriver(info,op)		\
	CallUniversalProc((UniversalProcPtr)CodeRegisterDriver, uppProcInfoType12,(info),(op))

#define MidiUnregisterDriver(r)		\
	CallUniversalProc((UniversalProcPtr)CodeUnregisterDriver, uppProcInfoType13,(r))

#define MidiCountDrivers()		\
	CallUniversalProc((UniversalProcPtr)CodeCountDrivers, uppProcInfoType14)

#define MidiGetIndDriver(index)		\
	CallUniversalProc((UniversalProcPtr)CodeGetIndDriver, uppProcInfoType15,(index))

#define MidiGetDriverInfos(r,infos)		\
	CallUniversalProc((UniversalProcPtr)CodeGetDriverInfos, uppProcInfoType16,(r),infos)


/*-------------------------------- Slots management ---------------------------*/

#define MidiAddSlotAux(r,name,dir)		\
	(long)CallUniversalProc((UniversalProcPtr)CodeAddSlot, uppProcInfoType17,(r),(name),(dir))
	
inline SlotRefNum MidiAddSlot(short refnum, SlotName name, SlotDirection direction)	
{ 
	long s = MidiAddSlotAux(refnum,name,direction);
  	return *(SlotRefNum *)&s;
}

#define MidiGetIndSlotAux(r,index)		\
	CallUniversalProc((UniversalProcPtr)CodeGetIndSlot, uppProcInfoType18,(r),(index))
	  
inline SlotRefNum MidiGetIndSlot(short r, short index)		
{ 
	long s = MidiGetIndSlotAux(r,index);
  	return *(SlotRefNum *)&s;
}

#define MidiRemoveSlot(slot)		\
	CallUniversalProc((UniversalProcPtr)CodeRemoveSlot, uppProcInfoType19,(slot))
	
#define MidiGetSlotInfos(slot ,infos)		\
	CallUniversalProc((UniversalProcPtr)CodeGetSlotInfos, uppProcInfoType20,(slot),(infos))
	
#define MidiConnectSlot(port ,slot,state)		\
	CallUniversalProc((UniversalProcPtr)CodeConnectSlot, uppProcInfoType21,(port),(slot),(state))

#define MidiIsSlotConnected(port ,slot)		\
	CallUniversalProc((UniversalProcPtr)CodeIsSlotConnected, uppProcInfoType22,(port),(slot))

#define MidiSetSlotName(slot,name)		\
	CallUniversalProc((UniversalProcPtr)CodeSetSlotName, uppProcInfoType23,(slot),(name))

#ifdef __cplusplus
}
#endif

#endif