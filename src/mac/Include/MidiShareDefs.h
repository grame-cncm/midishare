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

#ifndef __MidiShareDefs__
#define __MidiShareDefs__

#ifndef __TYPES__
#include <Types.h>
#endif

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
 * 		            © GRAME 1989, 1990, 1991, 1992 
 *	          [Yann Orlarey, HervŽ Lequay, Dominique fober]						
 *----------------------------------------------------------------------------
 *
 *	version 1.20 
 *	version Lightspeed C version 3.01 G. Bloch Aout 1989 (modif YO 9-9-89)
 *	version Think C 4.0 : YO [13-11-89]
 *  extensions t‰ches diffŽrŽes : YO [05-12-89]
 *	version MPW C++ 3.1 : YO [09-03-90]
 *	harmonisation prototypes: YO [12-05-90]
 *	harmonisation MPW & TC: YO [12-05-90]
 *	implŽmentation de 'MidiShare()' sous forme de macro: YO [15-03-91]
 *  english version : YO [29-01-92]
 *  changes for TC5 : YO [03-02-93]
******************************************************************************/

/*******************************************************************************
 * 							MIDISHARE EVENTS									
 *------------------------------------------------------------------------------
 * 	The listing below presents the different types of MidiShare handled events. 
 * 	This typology contains the whole of the standard Midi messages, plus specific 
 * 	messages such as typeNote corresponding to a note with its duration;  
 * 	or typeStream corresponding to a series of arbitrary bytes, possibly including 
 * 	data and status codes, sent directly without any processing; or typePrivate 
 * 	that are application private messages.
 * 	
 * 	All these codes may be used in the MidiNewEv function to allocate an event
 * 	of the desirable type and are accessible in an event evType field.
 *******************************************************************************/
			
#define typeNote		0 	/* note with pitch, velocity and duration		*/	
		
#define typeKeyOn		1 	/* Note On with pitch, velocity 				*/
#define typeKeyOff		2 	/* Note Off with pitch, velocity 				*/
#define typeKeyPress 	3 	/* Poly Key Pressure with pitch and pressure	*/
#define typeCtrlChange	4 	/* Control Change with controller ID and value	*/
#define typeProgChange	5 	/* Program Change with program ID number		*/
#define typeChanPress	6 	/* Channel Pressure with pressure value			*/
#define typePitchWheel	7 	/* Pitch Bend Change with LSB and MSB values	*/
		
#define typeSongPos		8 	/* Song Position Pointer with LSB and MSB values*/
#define typeSongSel		9 	/* Song Select with song ID number				*/
#define typeClock		10 	/* Timing Clock									*/
#define typeStart		11 	/* Start										*/
#define typeContinue	12 	/* Continue										*/
#define typeStop		13	/* Stop											*/
		
#define typeTune		14 	/* Tune Request									*/
#define typeActiveSens	15 	/* Active Sensing								*/
#define typeReset		16	/* System Reset									*/
	
#define typeSysEx		17 	/* System Exclusive (only data bytes)			*/
#define typeStream		18 	/* arbitrary midi bytes (data and status codes)	*/
		
#define typePrivate		19	/*19..127 Private events for applications internal use*/
#define typeProcess		128	/* used by MidiShare for MidiCall and MidiTask	*/
#define typeDProcess	129	/* used by MidiShare for MidiDTask				*/
#define typeQuarterFrame 130 /* Midi time code quarter frame				*/

#define typeCtrl14b		131	
#define typeNonRegParam	132
#define typeRegParam	133

#define typeSeqNum		134		/* numŽro de sŽquence					*/
#define typeTextual		135		/* ŽvŽnement texte						*/
#define typeCopyright	136		/* message copyright					*/
#define typeSeqName		137		/* nom de sŽquence ou de piste			*/
#define typeInstrName	138		/* nom d'instrument						*/
#define typeLyric		139		/* paroles dŽstinŽes ˆ etre chantŽes	*/
#define typeMarker		140		/* marqueur								*/
#define typeCuePoint	141		/* cue point							*/
#define typeChanPrefix	142		/* Midi Channel Prefix					*/
#define typeEndTrack	143		/* fin de piste							*/
#define typeTempo		144		/* changement de tempo					*/
#define typeSMPTEOffset	145		/* offset smpte							*/

#define typeTimeSign	146		/* indication de mesure					*/
#define typeKeySign		147		/* signature tonale						*/
#define typeSpecific	148		/* mŽta evt spŽcifique ˆ un sŽquenceur	*/

#define typeReserved	149	/*149..254	reserved for future extensions		*/
		
#define typeDead		255	/* dead Task or DTask							*/
		


/******************************************************************************
* 								 MIDI STATUS CODE								
*******************************************************************************/

#define NoteOff		0x80	
#define NoteOn		0x90
#define PolyTouch	0xa0
#define ControlChg	0xb0
#define ProgramChg	0xc0
#define AfterTouch	0xd0
#define PitchBend	0xe0
#define SysRealTime	0xf0
#define SysEx		0xf0
#define QFrame		0xf1
#define SongPos		0xf2
#define SongSel		0xf3
#define UnDef2		0xf4
#define UnDef3		0xf5
#define Tune		0xf6
#define EndSysX		0xf7
#define MClock		0xf8
#define UnDef4		0xf9
#define MStart		0xfa
#define MCont		0xfb
#define MStop		0xfc
#define UnDef5		0xfd
#define ActSense	0xfe
#define MReset		0xff


/*******************************************************************************
* 									SERIAL PORTS									
*------------------------------------------------------------------------------
* The Modem and printer serial ports ID numbers.							
*******************************************************************************/
			
#define ModemPort	0
#define PrinterPort	1
		

/******************************************************************************
* 									ERROR CODES								
*------------------------------------------------------------------------------
* List of the error codes returned by some MidiShare functions.																	
*******************************************************************************/
		
#define MIDIerrSpace 	-1		/* plus de place dans la freeList */
#define MIDIerrRefNum	-2		/* mauvais numero de reference */
#define MIDIerrBadType	-3		/* mauvais type d'ŽvŽnement */
#define MIDIerrIndex	-4		/* mauvais index d'acces ˆ un ŽvŽnement */
		

/******************************************************************************
* 								SYNCHRONISATION CODES								
*------------------------------------------------------------------------------
* List of the error codes returned by some MidiShare functions.																	
*******************************************************************************/
		
#define MIDISyncExternal 	0x8000		/* bit-15 for external synchronisation */
#define MIDISyncAnyPort		0x4000		/* bit-14 for synchronisation on any port */


/******************************************************************************
* 								  CHANGE CODES							
*------------------------------------------------------------------------------
When an application need to know about context modifications like opening and
closing of applications, opening and closing of midi ports, changes in 
connections between applications, it can install an ApplAlarm (with 
MidiSetApplAlarm). This ApplAlarm is then called by MidiShare every time a 
context modification happens with a 32-bits code describing the modification. 
The hi 16-bits part of this code is the refNum of the application involved in 
the context modification, the low 16-bits part describe the type of change as
listed here.
*******************************************************************************/
		
enum{	MIDIOpenAppl=1,
		MIDICloseAppl,
		MIDIChgName,
		MIDIChgConnect,
		MIDIOpenModem,
		MIDICloseModem,
		MIDIOpenPrinter,
		MIDIClosePrinter,
		MIDISyncStart,
		MIDISyncStop,
		MIDIChangeSync,
        MIDIOpenDriver,
        MIDICloseDriver,
        MIDIAddSlot,
        MIDIRemoveSlot,
        MIDIChgSlotConnect
};
	
		
/******************************************************************************
* 							    EVENTS STRUCTURES							
*------------------------------------------------------------------------------
* Tous les ŽvŽnements sont construits ˆ l'aide d'une ou plusieurs cellules de	
* taille fixe (16 octets). La plupart nŽcessitent une seule cellule. D'autres	
* comme les System Exclusive nŽcessitent plusieurs cellules cha”nŽes.			
*******************************************************************************/


/*------------------------ System Exclusive extension cell ----------------------*/

	typedef struct TMidiSEX *MidiSEXPtr;	
	typedef struct TMidiSEX
	{
		MidiSEXPtr link;					/* link to next cell				*/
		Byte data[12];						/* 12 data bytes					*/
	}	TMidiSEX;						
	

/*------------------------------ Private extension cell -------------------------*/

	typedef struct TMidiST *MidiSTPtr;		
	typedef struct TMidiST					
	{
		Ptr ptr1;							/* 4 32-bits fields 				*/
		Ptr ptr2;							
		Ptr ptr3;
		Ptr ptr4;						
	}	TMidiST;						
		

/*------------------------- Common Event Structure ----------------------*/

	typedef struct TMidiEv *MidiEvPtr;		
	typedef struct TMidiEv
	{
		MidiEvPtr link;						/* link to next event				*/
		unsigned long date;					/* event date (in ms)		 		*/
		Byte evType;						/* event type		 				*/
		Byte refNum;						/* sender reference number		 	*/
		Byte port;							/* Midi port 						*/
		Byte chan;							/* Midi channel					 	*/
		union {								/* info depending of event type :	*/
			struct {							/* for notes					*/
				Byte pitch;							/* pitch  					*/
				Byte vel;							/* velocity 				*/
				unsigned short dur;					/* duration 				*/
			} note;

			struct {							/* for MidiFile time signature  */
				Byte numerator;						/* numerator				*/
				Byte denominator;					/* denominator as neg power	*/
													/* of 2. (2= quarter note)	*/
				Byte nClocks;						/* number of Midi clocks in */
													/* a metronome click		*/
				Byte n32nd;							/* number of 32nd notes in	*/
													/* a Midi quarter note		*/
			} timeSign;

			struct {							/* for MidiFile key signature	*/
				char ton;							/* 0: key of C, 1: 1 sharp	*/
													/* -1: 1 flat etc...		*/
				Byte mode;							/* 0: major 1: minor		*/
				Byte unused[2];
			} keySign;

			struct {							/* for paramchg & 14-bits ctrl	*/
				short num;						/* param or ctrl num			*/
				short val;						/* 14-bits value				*/
			} param;

			struct {							/* for MidiFile sequence number */
				unsigned short number;
				short unused;
			} seqNum;
			long tempo;							/* MidiFile tempo in			*/
												/* microsec/Midi quarter note	*/
			Byte data[4];						/* for other small events	 	*/
			MidiSEXPtr linkSE;					/* link to last sysex extension	*/
			MidiSTPtr linkST;					/* link to private extension	*/
		} info;
	} TMidiEv;		

	
/*------------------------------ sequence header ---------------------------*/

	typedef struct TMidiSeq *MidiSeqPtr;	
	typedef struct TMidiSeq
	{
		MidiEvPtr first;					/* first event pointer 				*/
		MidiEvPtr last;						/* last event pointer 				*/
		Ptr undef1;
		Ptr undef2;
	}	TMidiSeq;
	

/*-------------------------------- input Filter -------------------------------*/

	typedef struct TFilter *MidiFilterPtr;
	typedef struct TFilter
	{
		char port[32];						/* 256 bits	*/
		char evType[32];					/* 256 bits */
		char channel[2];					/*  16 bits	*/
		char unused[2];						/*  16 bits */
	} TFilter; 
	

/*------------------------ MidiShare application name ------------------------*/

	#define DrvNameLen 32
	typedef unsigned char * MidiName;
	typedef unsigned char	DriverName[DrvNameLen];
	typedef DriverName SlotName;
	

/*----------------------- drivers and slots information -----------------------*/

	typedef long SlotRefNum;
	#define Slot(ref) 			((ref) & 0xffff)

	typedef enum { MidiInputSlot=1, MidiOutputSlot, MidiInputOutputSlot } SlotDirection;
	typedef struct TSlotInfos {
		SlotName 		name;
		SlotDirection 	direction;
		char 			cnx[32];	// bit field : 256 ports connection state
		long			reserved[2];
	} TSlotInfos;

	typedef struct TDriverOperation {
		void 		(* wakeup) (short refNum);
		void 		(* sleep)  (short refNum);
		Boolean 	(* slotInfo)  (SlotRefNum slot, TSlotInfos * infos);
		long		reserved[2];
	} TDriverOperation;

	typedef struct TDriverInfos {
		DriverName  name;
		short 		version;
		short 		slots;			// slots count - ignored at register time
		long		reserved[2];
	} TDriverInfos;


/*------------------------ Synchronisation informations -----------------------*/

	typedef struct TSyncInfo *SyncInfoPtr;
	typedef struct TSyncInfo
	{
 		long		time;
 		long		reenter;
 		unsigned short	syncMode;
 		Byte		syncLocked; 
 		Byte		syncPort;
		long		syncStart;
		long		syncStop;
		long		syncOffset;
		long		syncSpeed;
		long		syncBreaks;
		short		syncFormat;
	} TSyncInfo; 

	typedef struct TSmpteLocation *SmpteLocPtr;
	typedef struct TSmpteLocation
	{
 		short		format;			/* (0:24f/s, 1:25f/s, 2:30DFf/s, 3:30f/s) */
 		short		hours;			/* 0..23 */
 		short		minutes;		/* 0..59 */
 		short		seconds;		/* 0..59 */
 		short		frames;			/* 0..30 (according to format) */
 		short		fracs;			/* 0..99 (1/100 of frames) */
	} TSmpteLocation; 
	
	
/*----------------------------- Alarms prototypes ----------------------------*/

	typedef pascal void (* TaskPtr)( long date, short refNum, long a1,long a2,long a3 );
	typedef pascal void (* RcvAlarmPtr)( short refNum );
	typedef pascal void (* ApplAlarmPtr)( short refNum, long code );
	typedef pascal void (* ApplyProcPtr)( MidiEvPtr e );


/******************************************************************************
* 							   FIELD MACROS							
*------------------------------------------------------------------------------
* Somes macros to read and write event's fields					 
*******************************************************************************/

	#define Link(e) 	( (e)->link )
	#define Date(e) 	( (e)->date )
	#define EvType(e) 	( (e)->evType )
	#define RefNum(e) 	( (e)->refNum )	
	#define Port(e) 	( (e)->port )
	#define Canal(e)	( (e)->chan )
	#define Chan(e)		( (e)->chan )
	#define Pitch(e) 	( (e)->info.note.pitch ) 
	#define Vel(e) 		( (e)->info.note.vel ) 
	#define Dur(e) 		( (e)->info.note.dur )
	#define Data(e) 	( (e)->info.data )
	#define LinkSE(e) 	( (e)->info.linkSE )
	#define LinkST(e) 	( (e)->info.linkST )

	#define TSNum(e)	( (e)->info.timeSign.numerator )
	#define TSDenom(e)	( (e)->info.timeSign.denominator )
	#define TSClocks(e)	( (e)->info.timeSign.nClocks )
	#define TS32nd(e)	( (e)->info.timeSign.n32nd )

	#define KSTon(e)	( (e)->info.keySign.ton )
	#define KSMode(e)	( (e)->info.keySign.mode )

	#define Tempo(e)	( (e)->info.tempo )
	#define SeqNum(e)	( (e)->info.seqNum.number )
	#define ChanPrefix(e) ((e)->info.data[0] )

	#define First(e)	( (e)->first )
	#define Last(e)		( (e)->last )
	#define FirstEv(e)	( (e)->first )
	#define LastEv(e)	( (e)->last )

/******************************************************************************
* 							  FILTER MACROS							
*------------------------------------------------------------------------------
* somes macros to set and reset filter's bits. 
*******************************************************************************/

#ifdef __cplusplus
inline void 	AcceptBit( char* a, Byte n) 		{ (a)[(n)>>3] |=  (1<<((n)&7)); }
inline void 	RejectBit( char* a, Byte n) 		{ (a)[(n)>>3] &= ~(1<<((n)&7)); }
inline void 	InvertBit( char* a, Byte n) 		{ (a)[(n)>>3] ^=  (1<<((n)&7)); }

inline Boolean 	IsAcceptedBit( char* a, Byte n) 	{ return (a)[(n)>>3] & (1<<((n)&7)); }

#else

	#define AcceptBit(a,n) 			( ((char*) (a))[(n)>>3] |=   (1<<((n)&7)) )
	#define RejectBit(a,n) 			( ((char*) (a))[(n)>>3] &=  ~(1<<((n)&7)) )
	#define InvertBit(a,n) 			( ((char*) (a))[(n)>>3] ^=   (1<<((n)&7)) )

	#define IsAcceptedBit(a,n) 		( ((char*) (a))[(n)>>3]  &   (1<<((n)&7)) )
#endif


#endif
