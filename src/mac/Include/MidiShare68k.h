/*

  Copyright © Grame 1999-2000

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

#ifndef __MidiShare68k__
#define __MidiShare68k__

#include "MidiShareDefs.h"

/*******************************************************************************
* 								ENTRY POINTS									
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*--------------------------- Global MidiShare environment --------------------*/

pascal	void 		MidiShareSpecialInit	(void) 						={0x7037, 0x2078, 0x00B8, 0x4E90};
pascal	short 		MidiGetVersion	(void) 								={0x7000, 0x2078, 0x00B8, 0x4E90};

pascal	short 		MidiCountAppls	(void) 								={0x7001, 0x2078, 0x00B8, 0x4E90};
pascal	short 		MidiGetIndAppl 	(short index) 						={0x7002, 0x2078, 0x00B8, 0x4E90};		
pascal	short 		MidiGetNamedAppl(MidiName name) 					={0x7003, 0x2078, 0x00B8, 0x4E90};
 
/*----------------------------- SMPTE synchronization -------------------------*/

pascal	void 		MidiGetSyncInfo	(SyncInfoPtr p) 					={0x7038, 0x2078, 0x00B8, 0x4E90};
pascal	void 		MidiSetSyncMode	(unsigned short mode) 				={0x7039, 0x2078, 0x00B8, 0x4E90};
pascal	long 		MidiGetExtTime	(void) 								={0x703D, 0x2078, 0x00B8, 0x4E90};						
pascal	long 		MidiInt2ExtTime	(long) 								={0x703E, 0x2078, 0x00B8, 0x4E90};						
pascal	long 		MidiExt2IntTime	(long) 								={0x703F, 0x2078, 0x00B8, 0x4E90};						
pascal	void 		MidiTime2Smpte	(long time, short format, SmpteLocPtr loc) ={0x7040, 0x2078, 0x00B8, 0x4E90};						
pascal	long 		MidiSmpte2Time	(SmpteLocPtr loc) 					={0x7041, 0x2078, 0x00B8, 0x4E90};						

pascal 	long*		MidiGetTimeAddr	()									={0x7042, 0x2078, 0x00B8, 0x4E90};

/*----------------------------- Open / close application ----------------------*/

pascal short 		MidiOpen 		(MidiName applName) 				={0x7004, 0x2078, 0x00B8, 0x4E90};
pascal void 		MidiClose 		(short refNum) 						={0x7005, 0x2078, 0x00B8, 0x4E90};	


/*--------------------------- Application configuration -----------------------*/

pascal	MidiName 	MidiGetName 	(short refNum) 						={0x7006, 0x2078, 0x00B8, 0x4E90};
pascal	void 		MidiSetName 	(short refNum, MidiName applName) 	={0x7007, 0x2078, 0x00B8, 0x4E90};
pascal	void* 		MidiGetInfo 	(short refNum) 						={0x7008, 0x2078, 0x00B8, 0x4E90};	
pascal	void 		MidiSetInfo 	(short refNum, void* InfoZone) 		={0x7009, 0x2078, 0x00B8, 0x4E90};
pascal	MidiFilterPtr MidiGetFilter (short refNum) 						={0x700A, 0x2078, 0x00B8, 0x4E90};
pascal	void 		MidiSetFilter 	(short refNum, MidiFilterPtr f) 	={0x700B, 0x2078, 0x00B8, 0x4E90};
pascal	RcvAlarmPtr MidiGetRcvAlarm (short refNum) 						={0x700C, 0x2078, 0x00B8, 0x4E90};		
pascal	void 		MidiSetRcvAlarm (short refNum, RcvAlarmPtr alarm) 	={0x700D, 0x2078, 0x00B8, 0x4E90};	
pascal	ApplAlarmPtr MidiGetApplAlarm (short refNum) 					={0x700E, 0x2078, 0x00B8, 0x4E90};		
pascal	void 		MidiSetApplAlarm (short refNum, ApplAlarmPtr alarm) ={0x700F, 0x2078, 0x00B8, 0x4E90};


/*------------------------------- Drivers management --------------------------*/

pascal	short 	MidiRegisterDriver   (TDriverInfos * infos, TDriverOperation *op) 	={0x7044, 0x2078, 0x00B8, 0x4E90};
pascal	void 	MidiUnregisterDriver (short refnum)									={0x7045, 0x2078, 0x00B8, 0x4E90};
pascal	short	MidiCountDrivers	()												={0x7046, 0x2078, 0x00B8, 0x4E90};
pascal	short	MidiGetIndDriver	(short index)									={0x7047, 0x2078, 0x00B8, 0x4E90};
pascal	Boolean	MidiGetDriverInfos 	(short refnum, TDriverInfos * infos)			={0x7048, 0x2078, 0x00B8, 0x4E90};


/*-------------------------------- Slots management ---------------------------*/

pascal	SlotRefNum	MidiAddSlot 		(short refnum, SlotName name, SlotDirection direction)
																					={0x7050, 0x2078, 0x00B8, 0x4E90};
pascal	SlotRefNum	MidiGetIndSlot		(short refnum, short index)					={0x704A, 0x2078, 0x00B8, 0x4E90};
pascal	void		MidiRemoveSlot 		(SlotRefNum slot)							={0x704B, 0x2078, 0x00B8, 0x4E90};
pascal	Boolean		MidiGetSlotInfos 	(SlotRefNum slot, TSlotInfos * infos)		={0x704C, 0x2078, 0x00B8, 0x4E90};
pascal	void		MidiConnectSlot		(short port, SlotRefNum slot, Boolean state)={0x704D, 0x2078, 0x00B8, 0x4E90};
pascal	Boolean		MidiIsSlotConnected	(short port, SlotRefNum slot)				={0x704E, 0x2078, 0x00B8, 0x4E90};
pascal	void		MidiSetSlotName		(SlotRefNum slot, SlotName name)			={0x704F, 0x2078, 0x00B8, 0x4E90};


/*------------------------- Inter-Application Connections ---------------------*/

pascal	void 	MidiConnect 	(short src, short dest , Boolean state) ={0x7010, 0x2078, 0x00B8, 0x4E90};
pascal	Boolean MidiIsConnected (short src, short dest) 				={0x7011, 0x2078, 0x00B8, 0x4E90};


/*-------------------------------- Serial ports -------------------------------*/

pascal	Boolean MidiGetPortState (short port) 							={0x7012, 0x2078, 0x00B8, 0x4E90};	
pascal	void 	MidiSetPortState (short port, Boolean state) 			={0x7013, 0x2078, 0x00B8, 0x4E90};


/*-------------------------- Events and memory managing -----------------------*/

pascal	long 	MidiFreeSpace	(void) 									={0x7014, 0x2078, 0x00B8, 0x4E90};						
pascal 	long	MidiDesiredSpace ()										={0x7043, 0x2078, 0x00B8, 0x4E90};

pascal	MidiEvPtr MidiNewCell 	(void) 									={0x7033, 0x2078, 0x00B8, 0x4E90};			
pascal	void 	  MidiFreeCell 	(MidiEvPtr e) 							={0x7034, 0x2078, 0x00B8, 0x4E90};					
pascal	long 	  MidiTotalSpace (void)									={0x7035, 0x2078, 0x00B8, 0x4E90};
pascal	long 	  MidiGrowSpace (long n)								={0x7036, 0x2078, 0x00B8, 0x4E90};

pascal	MidiEvPtr MidiNewEv 	(short typeNum) 						={0x7015, 0x2078, 0x00B8, 0x4E90};			
pascal	MidiEvPtr MidiCopyEv 	(MidiEvPtr e) 							={0x7016, 0x2078, 0x00B8, 0x4E90};			
pascal	void 	  MidiFreeEv 	(MidiEvPtr e) 							={0x7017, 0x2078, 0x00B8, 0x4E90};

pascal  void    OldMidiSetField  (MidiEvPtr e, short f, long v)         ={0x7018, 0x2078, 0x00B8, 0x4E90};
pascal  long    OldMidiGetField  (MidiEvPtr e, short f)                 ={0x7019, 0x2078, 0x00B8, 0x4E90};
pascal  short   OldMidiCountFields (MidiEvPtr e)                        ={0x701B, 0x2078, 0x00B8, 0x4E90};

pascal	void 	MidiSetField 	(MidiEvPtr e, long f, long v) 			={0x703A, 0x2078, 0x00B8, 0x4E90};
pascal	long	MidiGetField 	(MidiEvPtr e, long f) 					={0x703B, 0x2078, 0x00B8, 0x4E90};
pascal	void 	MidiAddField 	(MidiEvPtr e, long v) 					={0x701A, 0x2078, 0x00B8, 0x4E90};
pascal	long 	MidiCountFields (MidiEvPtr e) 							={0x703C, 0x2078, 0x00B8, 0x4E90};


/*------------------------------- Sequence managing ---------------------------*/

pascal	MidiSeqPtr MidiNewSeq	(void) 									={0x701D, 0x2078, 0x00B8, 0x4E90};				 			
pascal	void 	MidiAddSeq 		(MidiSeqPtr s, MidiEvPtr e) 			={0x701E, 0x2078, 0x00B8, 0x4E90};
pascal	void 	MidiFreeSeq 	(MidiSeqPtr s) 							={0x701F, 0x2078, 0x00B8, 0x4E90};		
pascal	void 	MidiClearSeq 	(MidiSeqPtr s) 							={0x7020, 0x2078, 0x00B8, 0x4E90};			
pascal	void 	MidiApplySeq 	(MidiSeqPtr s, ApplyProcPtr proc)		={0x7021, 0x2078, 0x00B8, 0x4E90}; 


/*------------------------------------- Time ----------------------------------*/

pascal	long 	MidiGetTime		(void) 									={0x7022, 0x2078, 0x00B8, 0x4E90};		


/*------------------------------------ Sending --------------------------------*/

pascal	void 	MidiSendIm 		(short refNum, MidiEvPtr e) 			={0x7023, 0x2078, 0x00B8, 0x4E90};
pascal	void 	MidiSend 		(short refNum, MidiEvPtr e) 			={0x7024, 0x2078, 0x00B8, 0x4E90};
pascal	void 	MidiSendAt 		(short refNum, MidiEvPtr e, long d) 	={0x7025, 0x2078, 0x00B8, 0x4E90};	


/*------------------------------------ Receving -------------------------------*/

pascal	long 	MidiCountEvs 	(short refNum)						 	={0x7026, 0x2078, 0x00B8, 0x4E90};
pascal	MidiEvPtr MidiGetEv 	(short refNum)						 	={0x7027, 0x2078, 0x00B8, 0x4E90};
pascal	MidiEvPtr MidiAvailEv 	(short refNum)						 	={0x7028, 0x2078, 0x00B8, 0x4E90};
pascal	void 	MidiFlushEvs 	(short refNum)						 	={0x7029, 0x2078, 0x00B8, 0x4E90};	


/*----------------------------------- Mail boxes ------------------------------*/

pascal	void* 	MidiReadSync 	(void* adrMem)						 	={0x702A, 0x2078, 0x00B8, 0x4E90};
pascal	void* 	MidiWriteSync 	(void* adrMem, void* val)			 	={0x702B, 0x2078, 0x00B8, 0x4E90};


/*---------------------------------- Task Managing ----------------------------*/

pascal	void	MidiCall (TaskPtr routine, long date, short refNum, long a1,long a2,long a3)
																	 	={0x702C, 0x2078, 0x00B8, 0x4E90};

pascal	MidiEvPtr	MidiTask 	(TaskPtr routine, long date, short refNum, long a1,long a2,long a3)
																	 	={0x702D, 0x2078, 0x00B8, 0x4E90};
pascal	MidiEvPtr	MidiDTask 	(TaskPtr routine, long date, short refNum, long a1,long a2,long a3)
																	 	={0x702E, 0x2078, 0x00B8, 0x4E90};

pascal	void		MidiForgetTask (MidiEvPtr *e)					 	={0x702F, 0x2078, 0x00B8, 0x4E90};

pascal	long		MidiCountDTasks (short refnum)					 	={0x7030, 0x2078, 0x00B8, 0x4E90};

pascal	void		MidiFlushDTasks (short refnum)					 	={0x7031, 0x2078, 0x00B8, 0x4E90};

pascal	void		MidiExec1DTask 	(short refnum)					 	={0x7032, 0x2078, 0x00B8, 0x4E90};


/*---------------------------------- Filters ----------------------------------*/

pascal	MidiFilterPtr MidiNewFilter  (void)						={0x7051, 0x2078, 0x00B8, 0x4E90};
pascal	void		MidiFreeFilter (MidiFilterPtr)				={0x7052, 0x2078, 0x00B8, 0x4E90};

pascal	void		MidiAcceptPort(MidiFilterPtr f, short port, Boolean state)	={0x7053, 0x2078, 0x00B8, 0x4E90};
pascal	void		MidiAcceptChan(MidiFilterPtr f, short chan, Boolean state)	={0x7054, 0x2078, 0x00B8, 0x4E90};
pascal	void		MidiAcceptType(MidiFilterPtr f, short type, Boolean state)	={0x7055, 0x2078, 0x00B8, 0x4E90};

pascal	Boolean 	MidiIsAcceptedPort(MidiFilterPtr f, short port)	={0x7056, 0x2078, 0x00B8, 0x4E90};
pascal	Boolean 	MidiIsAcceptedChan(MidiFilterPtr f, short chan)	={0x7057, 0x2078, 0x00B8, 0x4E90};
pascal	Boolean 	MidiIsAcceptedType(MidiFilterPtr f, short type)	={0x7058, 0x2078, 0x00B8, 0x4E90};

/*----------------------------------- MidiShare -------------------------------*/

#define MidiShare() 	( **((long**)0xB8) == 0xD080D080 )

#ifdef __cplusplus
}
#endif

#endif
