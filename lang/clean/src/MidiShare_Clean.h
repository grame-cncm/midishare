/*****************************************************************************
 *                                MIDI SHARE									
 *----------------------------------------------------------------------------
 * MidiShare is a multi-tasking, real-time software environment, specially 
 * devised for the developing of MIDI applications with a triple target :
 * 
 * 	-To propose solutions to currently met problems when developing 
 * 	 a real-time MIDI application : memory management, communication management, 
 *	 time management, task management. 
 * 
 * 	-To enable the real-time and multi-tasking functioning of these applications, 
 * 	 i.e. to enable the sharing of all the necessary resources and their 
 * 	 simultaneous access.
 *
 * 	-To make easier cooperation between independent MIDI applications by  proposing 
 * 	 a real-time mechanism of inter-application communications.
 * 
 * This file contains a complete description of all the MidiShare functions and 
 * procedures, as well as all the data structures in use.
 *----------------------------------------------------------------------------
 * 		      c GRAME 1989, 2001
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
 *				MIDISHARE EVENTS									
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

	
 #define 	typeNote	0 	/* note with pitch, velocity and duration	*/	
 #define 	typeKeyOn	1 	/* Note On with pitch, velocity 				*/
 #define 	typeKeyOff	2 	/* Note Off with pitch, velocity 				*/
 #define  	typeKeyPress 	3 	/* Poly Key Pressure with pitch and pressure	*/
 #define  	typeCtrlChange	4 	/* Control Change with controller ID and value	*/
 #define  	typeProgChange	5 	/* Program Change with program ID number	*/
 #define  	typeChanPress	6 	/* Channel Pressure with pressure value		*/
 #define  	typePitchWheel	7 	/* Pitch Bend Change with LSB and MSB values	*/
		
 #define  	typeSongPos	8 	/* Song Position Pointer with LSB and MSB values*/
 #define  	typeSongSel	9 	/* Song Select with song ID number				*/
 #define  	typeClock	10 	/* Timing Clock				*/
 #define  	typeStart	11 	/* Start				*/
 #define  	typeContinue	12 	/* Continue				*/
 #define  	typeStop	13	/* Stop					*/
		
 #define   	typeTune	14 	/* Tune Request				*/
 #define   	typeActiveSens	15 	/* Active Sensing			*/
 #define   	typeReset	16	/* System Reset									*/
	
 #define  	typeSysEx	17 	/* System Exclusive (only data bytes)		*/
 #define   	typeStream	18 	/* arbitrary midi bytes (data and status codes)	*/
		
 #define   	typePrivate	19	/* 19..127 Application's private events		*/
 #define   	typeProcess	128	/* used by MidiShare for MidiCall and MidiTask	*/
 #define   	typeDProcess	129	/* used by MidiShare for MidiDTask		*/
 #define   	typeQuarterFrame  130 	/* Midi time code quarter frame			*/
 #define   	typeCtrl14b	 131	
 #define  	typeNonRegParam	 132
 #define   	typeRegParam	 133

 #define  typeSeqNum		 134		/* MidiFile sequence number			*/
 #define  typeText		 135			/* MidiFile text event				*/
 #define  typeTextual		 135
 #define  typeCopyright		 136		/* MidiFile copyright message		*/
 #define  typeSeqName		 137		/* MidiFile sequence or track name	*/
 #define  typeInstrName		 138		/* MidiFile nom d'instrument		*/
 #define  typeLyric		 		139		/* MidiFile lyrics					*/
 #define  typeMarker		 140		/* MidiFile marker					*/
 #define  typeCuePoint		 141		/* MidiFile cue point				*/
 #define  typeChanPrefix	 142		/* MidiFile Midi Channel Prefix		*/
 #define  typeEndTrack		 143		/* MidiFile end of track			*/
 #define  typeTempo		 	144			/* MidiFile tempo change			*/
 #define  typeSMPTEOffset	 145		/* MidiFile smpte offset			*/

 #define   typeTimeSign		 146		/* MidiFile time signature			*/
 #define   typeKeySign		 147		/* MidiFile key signature			*/
 #define   typeSpecific		 148		/* MidiFile sepcific meta event		*/
 #define   typePortPrefix	 149		/* MidiFile Midi Port Prefix		*/

 #define   typeRcvAlarm    	 150     	/* RcvAlarm         				*/
 #define   typeApplAlarm   	 151     	/* ApplAlarm       					*/


 #define  typeReserved     	 152    	/*152..254 reserved for future extensions 	*/
 #define  typeLastReserved 	 254		/*152..254 reserved for future extensions	*/
		
 #define   typeDead		 255			/* dead Task or DTask				*/


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
		
 #define MIDIOpenAppl 		1
 #define MIDICloseAppl 		2
 #define MIDIChgName		3
 #define MIDIChgConnect		4
 #define MIDIOpenModem		5   // now obsolete
 #define MIDICloseModem		6 	// now obsolete
 #define MIDIOpenPrinter	7 	// now obsolete
 #define MIDIClosePrinter	8	// now obsolete
 #define MIDISyncStart		9
 #define MIDISyncStop		10
 #define MIDIChangeSync		11
 #define MIDIOpenDriver		12
 #define MIDICloseDriver	13
 #define MIDIAddSlot		14
 #define MIDIRemoveSlot		15
 #define MIDIChgSlotConnect	16
 #define MIDIChgSlotName	17
 
/*------------------------ MidiShare application name ------------------------*/

 typedef char* MidiName;
 typedef int Boolean;

Clean (
	:: *State :== Int;
	:: RefNum :==  Int ;
	:: MidiEvPtr :==  Int;
	:: MidiSeqPtr :==  Int;
	:: EvType :==  Int;
	:: MidiName :== String
)

/*----------------------------------- MidiShare -------------------------------*/

int Clean_MidiShare	();
Clean (Clean_MidiShare :: State -> (Bool,State))

/*--------------------------- Global MidiShare environment --------------------*/

int Clean_MidiGetVersion ();
Clean (Clean_MidiGetVersion :: Int)

int Clean_MidiCountAppls ();
Clean (Clean_MidiCountAppls :: State -> (Int,State))

int Clean_MidiGetIndAppl (int index);	
Clean (Clean_MidiGetIndAppl :: Int State  -> (Int,State))
	
int Clean_MidiGetNamedAppl (CleanString name);
Clean (Clean_MidiGetNamedAppl :: String State  -> (Int,State))

/*----------------------------- SMPTE synchronization -------------------------*/
// Not defined
/*----------------------------- Open/Close application ----------------------*/

int Clean_MidiOpen (CleanString);
Clean (Clean_MidiOpen :: String State -> (RefNum,State))

void Clean_MidiClose (int ref);
Clean (Clean_MidiClose :: RefNum State -> State)

/*--------------------------- Application configuration -----------------------*/

void Clean_MidiGetName (int refNum, CleanString*);
Clean (Clean_MidiGetName :: RefNum  State -> (String,State))

void Clean_MidiSetName (int refNum, CleanString applName);
Clean (Clean_MidiSetName :: RefNum String State -> State)

void Clean_MidiSetFilter (int refNum, int filter);
Clean (Clean_MidiSetFilter :: RefNum Int State -> State)

int Clean_MidiGetFilter (int refNum);
Clean (Clean_MidiGetFilter :: RefNum State -> (Int,State))

/* Not defined
void* 			MidiGetInfo 		(short refNum);	
void 			MidiSetInfo 		(short refNum, void* InfoZone);
RcvAlarmPtr 		MidiGetRcvAlarm 	(short refNum);		
void 			MidiSetRcvAlarm		(short refNum, RcvAlarmPtr alarm);	
ApplAlarmPtr 		MidiGetApplAlarm 	(short refNum);		
void 			MidiSetApplAlarm 	(short refNum, ApplAlarmPtr alarm);
*/

/*------------------------------- Drivers management --------------------------*/
// Not defined
/*-------------------------------- Slots management ---------------------------*/
// Not defined
/*------------------------- Inter-Application Connections ---------------------*/

void Clean_MidiConnect 	(int src, int dest , Boolean state);
Clean (Clean_MidiConnect :: Int Int Bool State -> State)

int Clean_MidiIsConnected (int src, int dest);
Clean (Clean_MidiIsConnected :: Int Int State -> (Bool,State))

/*-------------------------------- Serial ports -------------------------------*/
// Not defined
/*-------------------------- Events and memory managing -----------------------*/

int Clean_MidiFreeSpace();
Clean (Clean_MidiFreeSpace :: State -> (Int,State))

int Clean_MidiDesiredSpace();
Clean (Clean_MidiDesiredSpace :: State -> (Int,State))

int Clean_MidiNewCell();
Clean (Clean_MidiNewCell :: State -> (MidiEvPtr,State))

void Clean_MidiFreeCell(int cell);
Clean (Clean_MidiFreeCell :: MidiEvPtr State -> State)

int Clean_MidiTotalSpace();
Clean (Clean_MidiTotalSpace :: State -> (Int,State))

int Clean_MidiGrowSpace(int space);
Clean (Clean_MidiGrowSpace :: Int State -> (Int,State))

int Clean_MidiNewEv(int type);
Clean (Clean_MidiNewEv :: Int State -> (MidiEvPtr,State))

int Clean_MidiCopyEv(int ev);
Clean (Clean_MidiCopyEv :: MidiEvPtr State -> (MidiEvPtr,State))

void Clean_MidiFreeEv(int ev);
Clean (Clean_MidiFreeEv :: MidiEvPtr State -> State)

void Clean_MidiSetField(int ev, int index, int value);
Clean (Clean_MidiSetField :: MidiEvPtr Int Int State ->  State)

int Clean_MidiGetField(int ev, int index);
Clean (Clean_MidiGetField :: MidiEvPtr Int State -> (Int,State))

void Clean_MidiAddField(int ev, int val);
Clean (Clean_MidiAddField :: MidiEvPtr Int State -> State)

int Clean_MidiCountFields(int ev);
Clean (Clean_MidiCountFields :: MidiEvPtr State -> (Int,State))

/*------------------------------- Sequence managing ---------------------------*/

int Clean_MidiNewSeq();
Clean (Clean_MidiNewSeq :: State -> (MidiSeqPtr,State))

void Clean_MidiAddSeq(int seq, int ev);
Clean (Clean_MidiAddSeq :: MidiSeqPtr MidiEvPtr State -> State)

void Clean_MidiFreeSeq(int seq);
Clean (Clean_MidiFreeSeq :: MidiSeqPtr State -> State)

void Clean_MidiClearSeq(int seq);
Clean (Clean_MidiClearSeq :: MidiSeqPtr State -> State)

//void MidiApplySeq (MidiSeqPtr s, ApplyProcPtr proc);	

/*------------------------------------- Time ----------------------------------*/

int Clean_MidiGetTime();
Clean (Clean_MidiGetTime ::  State -> (Int,State))

/*------------------------------------ Sending --------------------------------*/

void Clean_MidiSendIm(int ref, int ev);
Clean (Clean_MidiSendIm :: RefNum MidiEvPtr State -> State)

void Clean_MidiSendAt(int ref, int ev, int date);
Clean (Clean_MidiSendAt :: RefNum MidiEvPtr Int State -> State)

void Clean_MidiSend(int ref, int ev);
Clean (Clean_MidiSend :: RefNum MidiEvPtr State -> State)

/*------------------------------------ Receving -------------------------------*/

int Clean_MidiCountEvs(int ref);
Clean (Clean_MidiCountEvs :: RefNum State -> (Int,State))

int Clean_MidiGetEv(int ref);
Clean (Clean_MidiGetEv :: RefNum State -> (Int,State))

void Clean_MidiFlushEvs(int ref);
Clean (Clean_MidiFlushEvs :: RefNum State -> State)

int Clean_MidiAvailEv(int ref);
Clean (Clean_MidiAvailEv :: RefNum State -> (Int,State))

/*----------------------------------- Mail boxes ------------------------------*/
// Not defined
/*---------------------------------- Task Managing ----------------------------*/
// Not defined
/*---------------------------------- Filters ----------------------------------*/

int Clean_MidiNewFilter();
Clean (Clean_MidiNewFilter :: State -> (Int,State))

void Clean_MidiFreeFilter(int filter);
Clean (Clean_MidiFreeFilter :: Int State -> State)

void Clean_MidiAcceptPort(int filter, int port,  Boolean state);
Clean (Clean_MidiAcceptPort :: Int Int Bool State -> State)

void Clean_MidiAcceptChan(int filter, int chan,  Boolean state);
Clean (Clean_MidiAcceptChan :: Int Int Bool State -> State)

void Clean_MidiAcceptType(int filter, int type,  Boolean state);
Clean (Clean_MidiAcceptType :: Int Int Bool State -> State)

int Clean_MidiIsAcceptedPort(int filter, int port);
Clean (Clean_MidiIsAcceptedPort :: Int Int State -> (Int,State))

int Clean_MidiIsAcceptedChan(int filter, int chan);
Clean (Clean_MidiIsAcceptedChan :: Int Int State -> (Int,State))

int Clean_MidiIsAcceptedType(int filter, int type);
Clean (Clean_MidiIsAcceptedType :: Int Int State -> (Int,State))


/*---------------------------------- Fields ----------------------------------*/

void Clean_MidiSetDate(int ev, int date);
Clean (Clean_MidiSetDate :: MidiEvPtr Int State -> State)

int Clean_MidiGetDate(int ev);
Clean (Clean_MidiGetDate :: MidiEvPtr State -> (Int,State))

void Clean_MidiSetType(int ev, int type);
Clean (Clean_MidiSetType :: MidiEvPtr Int State -> MidiEvPtr)

int Clean_MidiGetType(int ev);
Clean (Clean_MidiGetType :: MidiEvPtr State -> (Int,State))

void Clean_MidiSetChan(int ev, int chan);
Clean (Clean_MidiSetChan :: MidiEvPtr Int State -> MidiEvPtr)

int Clean_MidiGetChan(int ev);
Clean (Clean_MidiGetChan :: MidiEvPtr State -> (Int,State))

void Clean_MidiSetPort(int ev, int port);
Clean (Clean_MidiSetPort :: MidiEvPtr Int State -> MidiEvPtr)

int Clean_MidiGetPort(int ev);
Clean (Clean_MidiGetPort :: MidiEvPtr State -> (Int,State))

void Clean_MidiSetRefnum(int ev, int ref);
Clean (Clean_MidiSetRefnum :: MidiEvPtr Int State -> MidiEvPtr)

int Clean_MidiGetRefnum(int ev);
Clean (Clean_MidiGetRefnum :: MidiEvPtr State -> (Int,State))

void Clean_MidiSetLink(int ev, int link);
Clean (Clean_MidiSetLink :: MidiEvPtr Int State -> MidiEvPtr)

int Clean_MidiGetLink(int ev);
Clean (Clean_MidiGetLink :: MidiEvPtr State -> (Int,State))

void Clean_MidiSetFirstEv(int seq, int link);
Clean (Clean_MidiSetFirstEv :: MidiEvPtr Int State -> MidiEvPtr)

int Clean_MidiGetFirstEv(int seq);
Clean (Clean_MidiGetFirstEv :: MidiEvPtr State -> (Int,State))

void Clean_MidiSetLastEv(int seq, int link);
Clean (Clean_MidiSetLastEv :: MidiEvPtr Int State -> MidiEvPtr)

int Clean_MidiGetLastEv(int seq);
Clean (Clean_MidiGetLastEv :: MidiEvPtr State -> (Int,State))

/*---------------------------------- Special ----------------------------------*/

int Clean_MidiGetEvBlock(int ref);
Clean (Clean_MidiGetEvBlock :: RefNum -> MidiEvPtr)

int Clean_MidiWait(int ref, int d);
Clean (Clean_MidiWait :: RefNum Int  -> Int)

int Clean_MidiWaitTimeOut(int ref, int d);
Clean (Clean_MidiWaitTimeOut :: RefNum Int  -> MidiEvPtr)
