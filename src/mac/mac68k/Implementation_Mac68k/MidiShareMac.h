#ifndef __MidiShare__
#define __MidiShare__

/*****************************************************************************
 *                                MIDI SHARE									
 *----------------------------------------------------------------------------
 * MidiShare is a multi-tasking, real-time software environment, specially 
 * devised for the developing of MIDI applications with a triple target :
 * 
 * 	¥To propose solutions to currently met problems when developing 
 * 	 a real-time MIDI application : memory management, communication management, 
 *	 time management, task management. 
 * 
 * 	¥To enable the real-time and multi-tasking functioning of these applications, 
 * 	 i.e. to enable the sharing of all the necessary resources and their 
 * 	 simultaneous access.
 *
 * 	¥To make easier cooperation between independent MIDI applications by  proposing 
 * 	 a real-time mechanism of inter-application communications.
 * 
 * This file contains a complete description of all the MidiShare functions and 
 * procedures, as well as all the data structures in use.
 *----------------------------------------------------------------------------
 * 		            © GRAME 1989, 1990, 1991, 1992, 1999 
 *	          [Yann Orlarey, Herve Lequay, Dominique Fober, Stephane Letz]						
 *----------------------------------------------------------------------------
******************************************************************************/

#include "msDefs.h"

/*******************************************************************************
* 								ENTRY POINTS									
*------------------------------------------------------------------------------
* All the MidiShare entry point are implemented as below :
*			moveq	#nn, d0	;move entry point ID number into reg d0													
*			move.l	$B8, a0	;read address of MS dispatch routine from vector $B8													
*			jmp		(a0)	;jump to dispatch,													
*																				
* Vector $B8 is initialy set by MidiShare Init at boot time.		
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

pascal	void 	OldMidiSetField 	(MidiEvPtr e, short f, long v) 		={0x7018, 0x2078, 0x00B8, 0x4E90};
pascal	long	OldMidiGetField 	(MidiEvPtr e, short f) 				={0x7019, 0x2078, 0x00B8, 0x4E90};
pascal	short 	OldMidiCountFields (MidiEvPtr e) 						={0x701B, 0x2078, 0x00B8, 0x4E90};

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

pascal	void		MidiCall 	(TaskPtr routine, long date, short refNum, long a1,long a2,long a3)
																	 	={0x702C, 0x2078, 0x00B8, 0x4E90};
pascal	MidiEvPtr	MidiTask 	(TaskPtr routine, long date, short refNum, long a1,long a2,long a3)
																	 	={0x702D, 0x2078, 0x00B8, 0x4E90};
pascal	MidiEvPtr	MidiDTask 	(TaskPtr routine, long date, short refNum, long a1,long a2,long a3)
																	 	={0x702E, 0x2078, 0x00B8, 0x4E90};
pascal	void		MidiForgetTask (MidiEvPtr *e)					 	={0x702F, 0x2078, 0x00B8, 0x4E90};
pascal	long		MidiCountDTasks (short refnum)					 	={0x7030, 0x2078, 0x00B8, 0x4E90};
pascal	void		MidiFlushDTasks (short refnum)					 	={0x7031, 0x2078, 0x00B8, 0x4E90};
pascal	void		MidiExec1DTask 	(short refnum)					 	={0x7032, 0x2078, 0x00B8, 0x4E90};


/*----------------------------------- MidiShare -------------------------------*/

#define MidiShare() 	( **((long**)0xB8) == 0xD080D080 )


#ifdef __cplusplus
}
#endif

#endif
