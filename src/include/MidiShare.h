#ifndef __MidiShare__
#define __MidiShare__

/*****************************************************************************
 *                                MIDI SHARE									
 *----------------------------------------------------------------------------
 * MidiShare is a multi-tasking, real-time software environment, specially 
 * devised for the developing of MIDI applications with a triple target :
 * 
 * 	´To propose solutions to currently met problems when developing 
 * 	 a real-time MIDI application : memory management, communication management, 
 *	 time management, task management. 
 * 
 * 	´To enable the real-time and multi-tasking functioning of these applications, 
 * 	 i.e. to enable the sharing of all the necessary resources and their 
 * 	 simultaneous access.
 *
 * 	´To make easier cooperation between independent MIDI applications by  proposing 
 * 	 a real-time mechanism of inter-application communications.
 * 
 * This file contains a complete description of all the MidiShare functions and 
 * procedures, as well as all the data structures in use.
 *----------------------------------------------------------------------------
 * 		      c GRAME 1989, 1990, 1991, 1992, 1999 
 *	     [Yann Orlarey, Herve Lequay, Dominique fober, Stephane Letz]						
 *----------------------------------------------------------------------------
 *
 *	version 1.20 
 *	version Lightspeed C version 3.01 G. Bloch Aout 1989 (modif YO 9-9-89)
 *	version Think C 4.0 : YO [13-11-89]
 *  	extensions taches differees : YO [05-12-89]
 *	version MPW C++ 3.1 : YO [09-03-90]
 *	harmonisation prototypes: YO [12-05-90]
 *	harmonisation MPW & TC: YO [12-05-90]
 *	implementation de 'MidiShare()' sous forme de macro: YO [15-03-91]
 *  	english version : YO [29-01-92]
 *  	changes for TC5 : YO [03-02-93]
 *	Linux version : YO [29-07-99]
******************************************************************************/

/*******************************************************************************
 *						MIDISHARE EVENTS									
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
		
#define typePrivate		19		/*19..127 Private events for applications internal use*/
#define typeProcess		128		/* used by MidiShare for MidiCall and MidiTask	*/
#define typeDProcess	129		/* used by MidiShare for MidiDTask				*/
#define typeQuarterFrame 130 	/* Midi time code quarter frame					*/

#define typeCtrl14b		131	
#define typeNonRegParam	132
#define typeRegParam	133

#define typeSeqNum		134		/* MidiFile sequence number				*/
#define typeTextual		135		/* MidiFile text event					*/
#define typeCopyright	136		/* MidiFile copyright message			*/
#define typeSeqName		137		/* MidiFile sequence or track name		*/
#define typeInstrName	138		/* MidiFile instrument name				*/
#define typeLyric		139		/* MidiFile lyrics						*/
#define typeMarker		140		/* MidiFile marker						*/
#define typeCuePoint	141		/* MidiFile cue point					*/
#define typeChanPrefix	142		/* MidiFile Midi Channel Prefix			*/
#define typeEndTrack	143		/* MidiFile end of track				*/
#define typeTempo		144		/* MidiFile tempo change				*/
#define typeSMPTEOffset	145		/* MidiFile smpte offset				*/

#define typeTimeSign	146		/* MidiFile time signature				*/
#define typeKeySign		147		/* MidiFile key signature				*/
#define typeSpecific	148		/* MidiFile specific meta event			*/
#define typePortPrefix  149   	/* MidiFile Midi Port Prefix			*/
#define typeRcvAlarm    150   	/* RcvAlam         						*/
#define typeApplAlarm   151   	/* ApplAlam        						*/

#define typeMidiOpen       152
#define typeMidiOpenRes    153   /* server reply to MidiOpen */
#define typeMidiClose      154
#define typeMidiConnect    155
#define typeMidiSetName    156
#define typeMidiSetInfo    157
#define typeMidiSetFilter  158

#define typeReserved       159   /*159..254 reserved for future extensions */

#define typeDead           255   /* dead Task or DTask                     */

#define typeMidiCommSync	typeContinue

		
#define true 1
#define false 0

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
*						SERIAL PORTS									
*------------------------------------------------------------------------------
* The Modem and printer serial ports ID numbers.							
*******************************************************************************/
			
#define ModemPort	0
#define PrinterPort	1
		

/******************************************************************************
* 					ERROR CODES								
*------------------------------------------------------------------------------
* List of the error codes returned by some MidiShare functions.									
*******************************************************************************/
		
#define MIDIerrSpace    -1   /* no space left in the freeList */
#define MIDIerrRefNum   -2   /* bad reference number          */
#define MIDIerrBadType  -3   /* bad event type                */
#define MIDIerrIndex    -4   /* bad access index (events)     */
#define MIDIerrEv       -5   /* event argument is nil         */
#define MIDIerrUndef    -6   /* event argument is undef       */
#define MIDIerrComm     -7   /* client / server communication error */
	

/******************************************************************************
* 					SYNCHRONISATION CODES								
*------------------------------------------------------------------------------
* List of the error codes returned by some MidiShare functions.									
*******************************************************************************/
		
#define MIDISyncExternal 	0x8000		/* bit-15 for external synchronisation */
#define MIDISyncAnyPort		0x4000		/* bit-14 for synchronisation on any port */


/******************************************************************************
* 						CHANGE CODES							
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
		
enum{   MIDIOpenAppl=1,
		MIDICloseAppl,
		MIDIChgName,
		MIDIChgConnect,
		MIDIOpenModem,		// now obsolete
		MIDICloseModem,		// now obsolete
		MIDIOpenPrinter,	// now obsolete
		MIDIClosePrinter,	// now obsolete
        MIDISyncStart,
        MIDISyncStop,
        MIDIChangeSync,
        MIDIOpenDriver,
        MIDICloseDriver,
        MIDIAddSlot,
        MIDIRemoveSlot,
        MIDIChgSlotConnect,
        MIDIChgSlotName
};
	
		
/******************************************************************************
* 							    EVENTS STRUCTURES							
*------------------------------------------------------------------------------
* All events are built using one or several fixed size cells (16 bytes)
* Most events use  one cell. Some other like SysEx events use several linked cells.
*******************************************************************************/

	typedef unsigned char Byte;
	typedef void * Ptr;
	typedef int Boolean;

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
	#if 0
		Ptr ptr1;							/* 4 32-bits fields 				*/
		Ptr ptr2;							
		Ptr ptr3;
		Ptr ptr4;	
	#endif
		long val[4];					
	}	TMidiST;						
		

/*------------------------- Common Event Structure ----------------------*/

	typedef struct TMidiEv *MidiEvPtr;		
	typedef struct TMidiEv
	{
		MidiEvPtr link;				/* link to next event				*/
		unsigned long date;			/* event date (in ms)		 		*/
		Byte evType;				/* event type		 				*/
		Byte refNum;				/* sender reference number			*/
		Byte port;					/* Midi port 						*/
		Byte chan;					/* Midi channel						*/
		union {						/* info depending of event type :	*/
			struct {				/* for notes						*/
				Byte pitch;			/* pitch  							*/
				Byte vel;			/* velocity 						*/
				unsigned short dur;	/* duration 						*/
			} note;

			struct {				/* for MidiFile time signature  */
				Byte numerator;		/* numerator				*/
				Byte denominator;	/* denominator as neg power	*/
									/* of 2. (2= quarter note)	*/
				Byte nClocks;		/* number of Midi clocks in */
									/* a metronome click		*/
				Byte n32nd;			/* number of 32nd notes in	*/
									/* a Midi quarter note		*/
			} timeSign;

			struct {				/* for MidiFile key signature	*/
				char ton;			/* 0: key of C, 1: 1 sharp	*/
									/* -1: 1 flat etc...		*/
				Byte mode;			/* 0: major 1: minor		*/
				Byte unused[2];
			} keySign;
            
	    		struct {            	/* for paramchg & 14-bits ctrl  */
            			short num;      /* param or ctrl num            */
            			short val;      /* 14-bits value                */
            		} param;


			struct {				/* for MidiFile sequence number */
				unsigned short number;
				short unused;
			} seqNum;
			
			short shortFields[2];	/* for 14-bits controlers		*/
            long longField;

			long tempo;				/* MidiFile tempo in		*/
									/* microsec/Midi quarter note	*/
			Byte data[4];			/* for other small events	*/
			MidiSEXPtr linkSE;		/* link to last sysex extension	*/
			MidiSTPtr linkST;		/* link to private extension	*/
		} info;
	} TMidiEv;		

	
/*------------------------------ sequence header ---------------------------*/

	typedef struct TMidiSeq *MidiSeqPtr;	
	typedef struct TMidiSeq
	{
		MidiEvPtr first;		/* first event pointer 	*/
		MidiEvPtr last;			/* last event pointer 	*/
		Ptr undef1;
		Ptr undef2;
	}	TMidiSeq;
	

/*-------------------------------- input Filter -------------------------------*/

	typedef struct TFilter *MidiFilterPtr;
	typedef struct TFilter
	{
		char port[32];			/* 256 bits */
		char evType[32];		/* 256 bits */
		char channel[2];		/*  16 bits */
		char unused[2];			/*  16 bits */
	} TFilter; 
	

/*------------------------ MidiShare application name ------------------------*/

	#define DrvNameLen 32
	typedef char* MidiName;
	typedef char  DriverName[DrvNameLen];
	typedef DriverName SlotName;


/*----------------------- drivers and slots information -----------------------*/

	typedef struct {
		short	drvRef;
		short 	slotRef;
	} SlotRefNum;
	#define Slot(ref) 			((ref).slotRef)

	typedef enum { MidiInputSlot=1, MidiOutputSlot, MidiInputOutputSlot } SlotDirection;
	typedef struct TSlotInfos {
		SlotName 		name;
		SlotDirection 	direction;
		char 			cnx[32];	// bit field : 256 ports connection state
		long			reserved[2];
	} TSlotInfos;

	typedef void (* WakeupPtr) 	(short refnum);
	typedef void (* SleepPtr) 	(short refnum);
	typedef struct TDriverOperation {
		WakeupPtr wakeup;
		SleepPtr sleep;
		long	reserved[3];
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
 		short		format;			// (0:24f/s, 1:25f/s, 2:30DFf/s, 3:30f/s)
 		short		hours;			// 0..23
 		short		minutes;		// 0..59
 		short		seconds;		// 0..59
 		short		frames;			// 0..30 (according to format)
 		short		fracs;			// 0..99 (1/100 of frames)
	} TSmpteLocation; 
	
	
/*----------------------------- Alarms prototypes ----------------------------*/

	typedef void (* TaskPtr)( long date, short refNum, long a1,long a2,long a3 );
	typedef void (* RcvAlarmPtr)( short refNum );
	typedef void (* ApplAlarmPtr)( short refNum, long code );
	typedef void (* ApplyProcPtr)( MidiEvPtr e );


/******************************************************************************
* 							   FIELD MACROS							
*------------------------------------------------------------------------------
* Somes macros to read and write event's fields					 
*******************************************************************************/

 	#define Link(e)       ( (e)->link )
    #define Date(e)       ( (e)->date )
    #define EvType(e)     ( (e)->evType )
    #define RefNum(e)     ( (e)->refNum )
    #define Port(e)       ( (e)->port )
    #define Canal(e)      ( (e)->chan )
    #define Chan(e)       ( (e)->chan )
    #define Pitch(e)      ( (e)->info.note.pitch )
    #define Vel(e)        ( (e)->info.note.vel )
    #define Dur(e)        ( (e)->info.note.dur )
    #define Data(e)       ( (e)->info.data )
    #define LinkSE(e)     ( (e)->info.linkSE )
    #define LinkST(e)     ( (e)->info.linkST )

    #define TSNum(e)      ( (e)->info.timeSign.numerator )
    #define TSDenom(e)    ( (e)->info.timeSign.denominator )
    #define TSClocks(e)   ( (e)->info.timeSign.nClocks )
    #define TS32nd(e)     ( (e)->info.timeSign.n32nd )

    #define KSTon(e)      ( (e)->info.keySign.ton )
    #define KSMode(e)     ( (e)->info.keySign.mode )

    #define Tempo(e)      ( (e)->info.tempo )
    #define SeqNum(e)     ( (e)->info.seqNum.number )
    #define ChanPrefix(e) ( (e)->info.data[0] )
    #define PortPrefix(e) ( (e)->info.data[0] )

    #define First(e)      ( (e)->first )
    #define Last(e)       ( (e)->last )
    #define FirstEv(e)    ( (e)->first )
    #define LastEv(e)     ( (e)->last )


/*******************************************************************************
* 						MidiShare API									
*------------------------------------------------------------------------------
* 		
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------- MidiShare -------------------------------*/

int 		MidiShare();
 
/*--------------------------- Global MidiShare environment --------------------*/

short 		MidiGetVersion		(void);

short 		MidiCountAppls		(void);
short 		MidiGetIndAppl 		(short index);		
short 		MidiGetNamedAppl	(MidiName name);
 
/*----------------------------- SMPTE synchronization -------------------------*/

void 		MidiGetSyncInfo		(SyncInfoPtr p);
void 		MidiSetSyncMode		(unsigned short mode);
long 		MidiGetExtTime		(void);						
long 		MidiInt2ExtTime		(long);						
long 		MidiExt2IntTime		(long);						
void 		MidiTime2Smpte		(long time, short format, SmpteLocPtr loc);						
long 		MidiSmpte2Time		(SmpteLocPtr loc);						

/*----------------------------- Open / close application ----------------------*/

short 		MidiOpen 			(MidiName applName);
void 		MidiClose 			(short refNum);	


/*--------------------------- Application configuration -----------------------*/

MidiName 	MidiGetName 		(short refNum);
void 		MidiSetName 		(short refNum, MidiName applName);
void* 		MidiGetInfo 		(short refNum);	
void 		MidiSetInfo 		(short refNum, void* InfoZone);
MidiFilterPtr 	MidiGetFilter 		(short refNum);
void 			MidiSetFilter 		(short refNum, MidiFilterPtr f);
RcvAlarmPtr 	MidiGetRcvAlarm 	(short refNum);		
void 			MidiSetRcvAlarm		(short refNum, RcvAlarmPtr alarm);	
ApplAlarmPtr 	MidiGetApplAlarm 	(short refNum);		
void 			MidiSetApplAlarm 	(short refNum, ApplAlarmPtr alarm);


/*------------------------------- Drivers management --------------------------*/

short 	MidiRegisterDriver   (TDriverInfos * infos, TDriverOperation *op);
void 	MidiUnregisterDriver (short refnum);
short	MidiCountDrivers	();
short	MidiGetIndDriver	(short index);
Boolean	MidiGetDriverInfos 	(short refnum, TDriverInfos * infos);


/*-------------------------------- Slots management ---------------------------*/

SlotRefNum	MidiAddSlot 		(short refnum, MidiName name, SlotDirection direction);
SlotRefNum	MidiGetIndSlot		(short refnum, short index);
void		MidiRemoveSlot 		(SlotRefNum slot);
void		MidiSetSlotName 	(SlotRefNum slot, MidiName name);
Boolean		MidiGetSlotInfos 	(SlotRefNum slot, TSlotInfos * infos);
void		MidiConnectSlot		(short port, SlotRefNum slot, Boolean state);
Boolean		MidiIsSlotConnected	(short port, SlotRefNum slot);


/*------------------------- Inter-Application Connections ---------------------*/

void 		MidiConnect 		(short src, short dest , Boolean state);
Boolean 	MidiIsConnected		(short src, short dest);


/*-------------------------------- Serial ports -------------------------------*/

Boolean 	MidiGetPortState 	(short port);	
void 		MidiSetPortState 	(short port, Boolean state);


/*-------------------------- Events and memory managing -----------------------*/

long 		MidiFreeSpace		(void);						

MidiEvPtr 	MidiNewCell 		(void);			
void 	  	MidiFreeCell 		(MidiEvPtr e);					
long 	  	MidiTotalSpace 		(void);
long 	  	MidiGrowSpace 		(long n);

MidiEvPtr 	MidiNewEv 			(short typeNum);			
MidiEvPtr 	MidiCopyEv 			(MidiEvPtr e);			
void 	  	MidiFreeEv 			(MidiEvPtr e);					

void 		MidiSetField 		(MidiEvPtr e, long f, long v);
long		MidiGetField 		(MidiEvPtr e, long f);
void 		MidiAddField 		(MidiEvPtr e, long v);
long 		MidiCountFields 	(MidiEvPtr e);


/*------------------------------- Sequence managing ---------------------------*/

MidiSeqPtr 	MidiNewSeq			(void);				 			
void 		MidiAddSeq 			(MidiSeqPtr s, MidiEvPtr e);
void 		MidiFreeSeq 		(MidiSeqPtr s);		
void 		MidiClearSeq 		(MidiSeqPtr s);			
void 		MidiApplySeq 		(MidiSeqPtr s, ApplyProcPtr proc); 


/*------------------------------------- Time ----------------------------------*/

long 		MidiGetTime		(void);		


/*------------------------------------ Sending --------------------------------*/

void 		MidiSendIm 		(short refNum, MidiEvPtr e);
void 		MidiSend 		(short refNum, MidiEvPtr e);
void 		MidiSendAt 		(short refNum, MidiEvPtr e, long d);	

	

/*------------------------------------ Receving -------------------------------*/

long 		MidiCountEvs 	(short refNum);
MidiEvPtr 	MidiGetEv 		(short refNum);
MidiEvPtr 	MidiAvailEv 	(short refNum);
void 		MidiFlushEvs 	(short refNum);	


/*----------------------------------- Mail boxes ------------------------------*/

void* 		MidiReadSync 	(void* adrMem);
void* 		MidiWriteSync 	(void* adrMem, void* val);


/*---------------------------------- Task Managing ----------------------------*/

void		MidiCall 	(TaskPtr routine, long date, short refNum, long a1,long a2,long a3);
MidiEvPtr	MidiTask 	(TaskPtr routine, long date, short refNum, long a1,long a2,long a3);
MidiEvPtr	MidiDTask 	(TaskPtr routine, long date, short refNum, long a1,long a2,long a3);
void		MidiForgetTask	(MidiEvPtr *e);
long		MidiCountDTasks (short refnum);
void		MidiFlushDTasks (short refnum);
void		MidiExec1DTask 	(short refnum);

/*---------------------------------- Filter Managing ----------------------------*/

MidiFilterPtr MidiNewFilter  (void);
void          MidiFreeFilter (MidiFilterPtr filter);

void MidiAcceptPort(MidiFilterPtr f, short port, Boolean state);
void MidiAcceptChan(MidiFilterPtr f, short chan, Boolean state);
void MidiAcceptType(MidiFilterPtr f, short type, Boolean state);

Boolean MidiIsAcceptedPort(MidiFilterPtr f, short port);
Boolean MidiIsAcceptedChan(MidiFilterPtr f, short chan);
Boolean MidiIsAcceptedType(MidiFilterPtr f, short type);

short MidiGetLibVersion ();

#ifdef __cplusplus
}
#endif

#endif
