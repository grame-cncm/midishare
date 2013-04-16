/*
  MidiShare Project
  Copyright (C) Grame 1999-2006

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/


/*****************************************************************************/
/* Basic Midi Class : native methods call to MidiShare functions 
/* are implemented here
/*
/* All functions are implemented in the JNI interface, but only those which are 
/* usable in Java are declared public.
/*
/* Versions
/*
/* 123: Adaptation for JDK 1.1 (use of the String constructor in MidiFileStream.java)
/* 124: Correction of a ForgetTask bug  (test of the scheduler variable)
/* 125: Adaptation for the Player libaray 2.0 : no more kEventSync mode.
/* 126: Correction of a bug in MidiFile saving.
/* 127: On Macintosh MRJ, use of a CompletionRoutineNotifier for better incoming 
/* event management.
/* 128: Linux version 
/* 129: Add IsAcceptedPort, IsAcceptedChan, IsAcceptedType functions
/* 130: Functions for drivers management
/* 131: New PortPrefix event type
/* 132: New RemoveEv function
/* 133: Direct call of Java code for MacOSX
/* 134: Correct Midi.Share semantic : now the JMidi interface is loaded in Midi.Share 
/*      thus clients *must* use Midi.Share as the first call to the MidiShare API
/* 135: Use of  standard JNI string functions
/*****************************************************************************/


package grame.midishare;

import grame.midishare.*;
import java.lang.*;
import java.util.*;
import java.io.*;

/**
The basic interface between MidiShare and Java. MidiShare entry points are implemented as static methods. 
All functions are not available because of some compatibility problems between Java and C. 
For perfomances reasons, MidiShare events, sequences and filters are manipulated at <b>int</b> type in Java. 
*/

public final class Midi {
	private  static  boolean interfaceLoaded = false;
	public   static  int  Version =  135;

	/* Don't let anyone instantiate this class.*/
	private Midi() {}
	
  	/* event types */
  	/**
  	A note with pitch, velocity and duration. When a Note event is
  	sent to external Midi devices, actually a NoteOn message is
  	first sent followed, after a delay specified by the duration, by a
  	NoteOn with a velocity of 0 to end the note. 
	<br><br>
	Note events have 3 fields numbered from 0 to 2 :
	<br><br>
	 0 : Pitch, a note number from 0 to 127. (Field size : 1 byte) 
	<br>
	1 : Vel, a note velocity from 0 to 127. (Field size : 1 byte) 
	<br>
	2 : Dur, a note duration from 0 to 215-1. (Field size : 2 bytes) 
	*/
  
	public static final int typeNote    		= 0;
	
	/**
  	A Note On message with pitch and velocity. 
  	<br><br>

  	KeyOn events have 2 fields numbered from 0 to 1 :
  	<br><br>

	0 : Pitch, a note number from 0 to 127. (Field size : 1 byte) 
	<br>
	1 : Vel, a note velocity from 0 to 127. (Field size : 1 byte) 
  	*/
	
	public static final int typeKeyOn   		= 1;
	
	/**
  	A Note Off message with pitch and velocity. 
  	<br><br>

  	KeyOff events have 2 fields numbered from 0 to 1 
  	<br><br>

	0 : Pitch, a note number from 0 to 127. (Field size : 1 byte) 
	<br>
	1 : Vel, a note velocity from 0 to 127. (Field size : 1 byte) 
  	*/

	public static final int typeKeyOff  		= 2;
	
	/**
 	 A Polyphonic Key Pressure message with pitch and pressure. 
  	<br><br>

  	KeyPress events have 2 fields numbered from 0 to 1 :
  	<br><br>

	0 : Pitch, a note number from 0 to 127. (Field size : 1 byte) 
	<br>
	1 : Press, a key pressure from 0 to 127. (Field size : 1 byte)  
	*/

	public static final int typeKeyPress   		= 3;
	
	/**
 	A Control Change message with controller and value.
  	<br><br>

  	CtrlChange events have 2 fields numbered from 0 to 1 :
  	<br><br>

	0 : A control number from 0 to 127. (Field size : 1 byte) 
	<br>
	1 : A control value from 0 to 127. (Field size : 1 byte) 
	*/

	public static final int typeCtrlChange  	= 4;
	
	/**
  	A Program Change message with a program number. 
   	<br><br>

  	ProgChange events have 1 field numbered 0 :
    <br><br>

	0 : A program number from 0 to 127. (Field size : 1 byte) 
	*/

	public static final int typeProgChange  	= 5;
	
	/**
  	A Channel pressure message with pressure value. 
  	<br><br>

  	ChanPress events have 1 field numbered 0 :
  	<br><br>

	0 : A channel pressure value from 0 to 127. (Field size : 1
          byte) 	
  	*/

	public static final int typeChanPress  		= 6;
	
	/**
  	A Pitch Bender message with a 14 bits resolution. 
    <br><br>

  	PitchWheel events have 2 fields numbered from 0 to 1 :
    <br><br>

	0 : LS 7-Bits of 14-bits pitch swing, from 0 to 127. (Field
          size : 1 byte) 
	<br>
	1 : MS 7-Bits of 14-bits pitch swing, from 0 to 127. (Field
          size : 1 byte) 
  	*/

	public static final int typePitchWheel 		= 7;
	
	/**
  	A Song Position Pointer message with a 14 bit location (unit : 6
    Midi Clocks).    
  	<br><br>

  	SongPos events have 2 fields numbered from 0 to 1 :
  	<br><br>

	0 : LS 7-Bits of 14-bits location, from 0 to 127. (Field size
          : 1 byte) 	<br>
	1 : MS 7-Bits of 14-bits location, from 0 to 127. (Field size
          : 1 byte)  
  	*/

	public static final int typeSongPos			= 8;
	
	/**
  	A Song Select message with a song number.   
  	<br><br>

  	SongSel events have 1 field numbered 0 :
    <br><br>

	0 : A song number from 0 to 127. (Field size : 1 byte)   
	*/

	public static final int typeSongSel 		= 9;
	
	/**
  	A Real Time Clock message.  
  	<br><br>

  	Clock events have no field.    
	*/
	public static final int typeClock			= 10;	
	
	/**
 	 A Real Time Start message.  
  	<br><br>

  	Start events have no field.    
	*/
	public static final int typeStart		 	= 11;
	
	/**
 	A Real Time Continue message.  
  	<br><br>

 	 Continue events have no field.    
	*/

	public static final int typeContinue 		= 12;
	
	/**
 	A Real Time Stop message.  
  	<br><br>

  	Stop events have no field.    
	*/

	public static final int typeStop			= 13;
	
	/**
 	A Tune message. 
    <br><br>

  	Tune events have no field.    
	*/

	public static final int typeTune		 	= 14;
	
	/**
 	A Real Time ActiveSens message. 
    <br><br>

 	ActiveSens events have no field.    
	*/

	public static final int typeActiveSens 		= 15;
	
	/**
 	A Real Time Reset message. 
    <br><br>

  	Reset events have no field.   
	*/

	public static final int typeReset			= 16;
	
	/**
 	A System Exclusive message.  
    <br><br>

  	SysEx events have a variable number of fields. The leading F0
    and tailing F7 codes MUST NOT be included. They are automatically 
    supplied by MidiShare. The channel field of the event is OR'ed with 
    the first data byte after the manufacturer
    ID. This works for setting the channel of many system exclusive
    messages.  
	*/

	public static final int typeSysEx			= 17;
	
	/**
 	Stream messages are arbitrary streams of bytes sent by the
    MidiShare driver without any processing. 
    <br><br>
	Stream events have a variable number of fields. 
	*/
	
	public static final int typeStream			= 18;
	
	/**
 	A private event with 4 fields which can be freely used by the
    application. 
    <br><br>
	Private events have 4 fields numbered from 0 to 3.
	<br><br>

     Fields size : 4 bytes 
	*/

	public static final int typePrivate		 	= 19; /* 19 .... 127 */
	
	static final int typeProcess			= 128;
	static final int typeDProcess		= 129;
	 
	/**
 	A Midi time code quarter frame message with message type
    and value. These two fields are automatically assembled by
    MidiShare into one byte when the message is sent.  
  	<br><br>
	QuarterFrame events have 2 fields numbered from 0 to 1 :
	<br><br>

    0: A message type from 0=Frame count LSB nibble to
          7=Hours count MS nibble. (Field size : 1 byte) 
   	<br>
	1 : A count nibble from 0 to 15. (Field size : 1 byte) 
	*/

	public static final int typeQFrame			= 130;
	
	/**
 	A Control Change event with a controller number from 0 to 31
    and a 14-bits value. When a typeCtrl14b event is sent to
    external Midi devices, actually two control change messages
    are sent, the first one for the MSB part of the value and the
    second one for the LSB part of the value. The message for the
    LSB part is sent only when the LSB part of the value is
     different from 0. 
    <br><br>
	Ctrl14b events have 2 fields numbered from 0 to 1 :
	<br><br>

   	0: A control number from 0 to 31. (Field size : 2 byte) 
    <br>
	1 : A control value from 0 to 16383. (Field size : 2 byte) 		
	*/

	public static final int typeCtrl14b			= 131;
	
	/**
 	A Non Registered Parameter event with a 14-bit parameter
    number and a 14-bit parameter value. When a
    typeNonRegParam event is sent to external Midi devices,
    actually four control change messages are sent, two to select
    the non-registered parameter number, and two for the
    parameter value using the 14-bits data-entry controller. 
    <br><br>
	
	NonRegParam events have 2 fields numbered from 0 to 1 :		
	<br><br>

   	0: A Non Registered Parameter number from 0 to 16383.
    (Field size : 2 byte)  
    <br>
	1 : A parameter value from 0 to 16383. (Field size : 2 byte)		
	*/

	public static final int typeNonRegParam		= 132;
	
	/**
 	A Registered Parameter event with a 14-bit parameter number
    and a 14-bit parameter value. When a typeRegParam event is
    sent to external Midi devices, actually four control change
    messages are sent, two to select the registered parameter
    number, and two for the parameter value using the 14-bits
    data-entry controller. 
    <br><br>
	
	RegParam events have 2 fields numbered from 0 to 1 :		
	<br><br>

   	0: A Registered Parameter number from 0 to 16383.
          (Field size : 2 byte) 
    <br>
	1 : A Registered Parameter value from 0 to 16383. (Field
    size : 2 byte) 		
	*/
	
	public static final int typeRegParam		= 133;
	
	/**
 	A Sequence number event (from the Midi File 1.0
    specification). This event cannot be sent to external Midi
    devices. 
    <br><br>
	
	SeqNum events have 1 field :	
	<br><br>

    0: Sequence number form 0 to 65535 (2-bytes field)	
	*/

	public static final int typeSeqNum			= 134;
	
	/**
 	A text event (from the Midi File 1.0 specification). This event
    cannot be sent to external Midi devices. 
    <br><br>
	
	Text events have a variable number of character fields.		
	*/

	public static final int typeText			= 135;
	
	/**
 	A copyright event (from the Midi File 1.0 specification). This
    event cannot be sent to external Midi devices. 
    <br><br>
	
	Copyrigth events have a variable number of character
    fields. 	
	*/
	
	public static final int typeCopyright		= 136;
	
	/**
 	A sequence name event (from the Midi File 1.0 specification).
     This event cannot be sent to external Midi devices. 
     <br><br>
	
	SeqName events have a variable number of character
    fields. 			
	*/

	public static final int typeSeqName			= 137;
	
	/**
 	An instrument name event (from the Midi File 1.0
    specification). This event cannot be sent to external Midi
    devices.      <br><br>
	
	InstrName events have a variable number of character
    fields. 			
	*/

	public static final int typeInstrName		= 138;
	
	/**
 	A lyric event (from the Midi File 1.0 specification). This event
    cannot be sent to external Midi devices.    <br><br>
	
	Lyric events have a variable number of character
    fields. 			
	*/

	public static final int typeLyric			= 139;
	
	/**
	A marker event (from the Midi File 1.0 specification). This
    event cannot be sent to external Midi devices.  <br><br>
	
	Marker events have a variable number of character
    fields. 	
	*/

	public static final int typeMarker			= 140;
	
	/**
	A cue point event (from the Midi File 1.0 specification). This
    event cannot be sent to external Midi devices.   <br><br>
	
	Cuepoint events have a variable number of character
    fields. 	
	*/

	public static final int typeCuePoint		= 141;
	
	/**
	A channel prefix event (from the Midi File 1.0 specification).
    This event cannot be sent to external Midi devices.   <br><br>
	
	ChanPrefix events have one field. <br><br>
	0 : A channel prefix number from 0 to 15. (Field size : 1
    byte) 
	*/
	
	public static final int typeChanPrefix		= 142;
	
	/**
	An end of track event (from the Midi File 1.0 specification).
    This event cannot be sent to external Midi devices. 
    <br><br>
	
	EndTrack events have no field.			
	*/
	
	public static final int typeEndTrack		= 143;
	
	/**
	A tempo event (from the Midi File 1.0 specification). This
    event cannot be sent to external Midi devices.   <br><br>
	
	Tempo events have one field. <br><br>
	0 : A tempo value in microseconds/Midi quarter-note 0 to
    127. (Field size : 4 bytes) 			
	*/

	public static final int typeTempo			= 144;
	
	/**
	A SMPTE Offset event (from the Midi File 1.0 specification).
    This event cannot be sent to external Midi devices.    <br><br>
	
	SMPTEOffset events have 6 fields : <br><br>
	0 : Format : 0..3 (0 = 24 f/s, 1 = 25 f/s, 2 = 29 drop frame,
          3 = 30 f/s). <br><br>

  	1 : Hours : 0..23.  <br><br>

  
  	2 : minutes : 0..59.  <br><br>

  
  	3 : seconds : 0..59.  <br><br>

  
  	4 : Frames : 0..29.  <br><br>

  
  	5 : 100ths of a frame : 0..99.		
	*/
	
	public static final int typeSMPTEOffset		= 145;
	
	/**
	A Time Signature event (form the Midi File 1.0 specification).
    This event cannot be sent to external Midi devices.   <br><br>
	
	TimeSign events have 4 fields :<br><br>
	0 : Numerator (8-bits field)  <br><br>
  	1 : denominator in power of two (8-bits field)  <br><br>

	2 : Midi Clocks per metronome clicks (8-bits field)  <br><br>

	3 : notated 32th of note per quarter-note (8-bits field) 
	*/
	
	public static final int typeTimeSign		= 146;
	
	/**
    A Key Signature event (form the Midi File 1.0 specification).
    This event cannot be sent to external Midi devices. <br><br>
	
	KeySign events have 2 fields :<br><br>
	0 : from -7 (7 flats) to 7 (7 sharps), (8-bits field)  <br><br>
  	1 : form 0 (major key) to 1 (minor key), (8-bits field)  
	*/	
	
	public static final int typeKeySign			= 147;
	
	/**
  	A sequencer specific event (from the Midi File 1.0
    specification). This event cannot be sent to external Midi
    devices. <br><br>
	Specific events have a variable number of 8-bits fields.   
	*/
	
	public static final int typeSpecific		= 148;	
	
	/**
	A port prefix event (from the Midi File 1.0 specification).
    This event cannot be sent to external Midi devices.   <br><br>
	
	PortPrefix events have one field. <br><br>
	0 : A port prefix number from 0 to 255. (Field size : 1
    byte) 
	*/
	
	public static final int typePortPrefix		= 149;
	
	/**
	For internal use
	*/
	
	public static final int typeRcvAlarm		= 150;
	
	/**
	For internal use
	*/
	
	public static final int typeApplAlarm		= 151;

	/**
  	These events are reserved for future use.   
	*/
	
	public static final int typeReserved		= 152; /* 152 .... 254 */
	static final int typeDead					= 255;	
	
	/** 	Midi status code.	*/							
	public static final int NoteOff		= 0x80;	
	/** 	Midi status code.	*/
	public static final int NoteOn		= 0x90;
	/** 	Midi status code.	*/
	public static final int PolyTouch	= 0xa0;
	/** 	Midi status code.	*/
	public static final int ControlChg	= 0xb0;
	/** 	Midi status code.	*/
	public static final int ProgramChg	= 0xc0;
	/** 	Midi status code.	*/
	public static final int AfterTouch	= 0xd0;
	/** 	Midi status code.	*/
	public static final int PitchBend	= 0xe0;
	/** 	Midi status code.	*/
	public static final int SysRealTime	= 0xf0;
	/** 	Midi status code.	*/
	public static final int SysEx		= 0xf0;
	/** 	Midi status code.	*/
	public static final int QFrame		= 0xf1;
	/** 	Midi status code.	*/
	public static final int SongPos		= 0xf2;
	/** 	Midi status code.	*/
	public static final int SongSel		= 0xf3;
	/** 	Midi status code.	*/
	public static final int UnDef2		= 0xf4;
	/** 	Midi status code.	*/
	public static final int UnDef3		= 0xf5;
	/** 	Midi status code.	*/
	public static final int Tune		= 0xf6;
	/** 	Midi status code.	*/
	public static final int EndSysX		= 0xf7;
	/** 	Midi status code.	*/
	public static final int MClock		= 0xf8;
	/** 	Midi status code.	*/
	public static final int UnDef4		= 0xf9;
	/** 	Midi status code.	*/
	public static final int MStart		= 0xfa;
	/** 	Midi status code.	*/
	public static final int MCont		= 0xfb;
	/** 	Midi status code.	*/
	public static final int MStop		= 0xfc;
	/** 	Midi status code.	*/
	public static final int UnDef5		= 0xfd;
	/** 	Midi status code.	*/
	public static final int ActSense	= 0xfe;
	/** 	Midi status code.	*/
	public static final int MReset		= 0xff;

	/** Midi error code. */
	public static final int errSpace		= -1;
	/** Midi error code. */
	public static final int errRefNum		= -2;
	/** Midi error code. */
	public static final int errBadType		= -3;
	/** Midi error code. */
	public static final int errIndex	   	= -4;	
	
	/** Synchronisation code : bit-15 for external synchronisation.*/							
	public static final	int SyncExternal =	0x8000;		
	/** Synchronisation code : bit-14 for synchronisation on any port. */	
	public static final int SyncAnyPort	 =	0x4000;	

	/** Midi change code: an application was opened.
 	*/
	public static final int OpenAppl		= 1;
	/** Midi change code: an application was closed.
 	*/
	public static final int CloseAppl		= 2;
	/** Midi change code: an application name has changed.
 	*/
	public static final int ChgName			= 3;
	/** Midi change code:  a connection has changed. */
	public static final int ChgConnect		= 4;
	/** 
	*@deprecated see new drivers management functions.
	*@see grame.midishare.Midi#CountDrivers
 	*/
	public static final int OpenModem		= 5;
	/** 
	*@deprecated see new drivers management functions.
	*@see grame.midishare.Midi#CountDrivers
 	*/
	public static final int CloseModem		= 6;
	/**  
	*@deprecated see new drivers management functions.
	*@see grame.midishare.Midi#CountDrivers
	*/
	public static final int OpenPrinter		= 7;
	/** 
	*@deprecated see new drivers management functions.
	*@see grame.midishare.Midi#CountDrivers
 	*/	
 	public static final int ClosePrinter	= 8;
	/** Midi change code: MidiShare is now locked to SMPTE sync.
	*/
	public static final int SyncStart		= 9;
	/** Midi change code: SMPTE sync disappeard, MidiShare is now unlocked.
 	*/
	public static final int SyncStop		= 10;
	/** Midi change code: the synchronisation mode has changed.
 	*/
	public static final int ChangeSync	   	= 11;
	/** Midi change code: a drivers was opened
 	*/
	public static final int OpenDriver	   	= 12;
	/** Midi change code: a drivers was closed
 	*/
	public static final int CloseDriver		= 13;
	/** Midi change code: a slot is added to the system
 	*/
    public static final int AddSlot			= 14;
    /** Midi change code: a slot is removed from the system 
 	*/
    public static final int RemoveSlot		= 15;
    /** Midi change code: the slots connections changed
 	*/
    public static final int ChgSlotConnect	= 16;
    /** Midi change code: a slot name has changed
 	*/
    public static final int ChgSlotName		= 17;
	
	/**
  	Adds a field at the tail of an event of variable length (for
    example a System Exclusive or a Stream) and assigns to it the
    value transmitted as a parameter.    
	<br>

	*@param event  is the event to be modified. 
    *@param val   it is the value of the field to be added.
    This value is always a long for a purpose of uniformity,
    but it is internally translate to the right size (a byte in this
    case). The value of v is actually between 0 and 127 for a
    System Exclusive and between 0 and 255 for a Stream. 
	*/
	
	public  static native final void AddField(long event, int val);
	
	/**
  	Inserts an event in to a sequence while maintaining the dates in
    time order.    
		<br>

	*@param seq is the sequence to be modified. 
    *@param event is the event to be added. 			
	*/
	
	public  static native final void AddSeq(long seq, long event);
	  		static native final void ApplySeq(long seq, long event);
	  			
	/**
  	Gives a pointer to the first event at the head of the reception
    FIFO, without extracting it. AvailEv can be used if an
    application wants to test the first event in its reception FIFO,
    without processing it. 		<br>

	*@param ref    is the reference number of the application.
    *@return   The result is a pointer to the first event in the
    reception FIFO, or 0 if the reception FIFO is empty. 		
	*/

	public  static native final long AvailEv(int ref);
	 		static native final void Call(long proc, long date, int ref, int a1, int a2, int a3);
	 				
	/**
  	Frees the content of a sequence. ClearSeq de-allocates all
    the events of the given sequence, consequently this sequence
    becomes empty. 		<br>

	*@param seq is a pointer on a sequence whose eventsare to be freed.		
	*/
 				
	public  static native final void ClearSeq(long seq);
	
	/**
  	This is used for closing of a MidiShare application. Every
    opening of MidiShare with Open must be matched by a
    call to Close, so that MidiShare can keep track of active
    applications and release the corresponding internal data
    structures. All the MidiShare applications owning a "context
    alarm" will be informed of this closing. 	<br>

	*@param ref is the reference number of the
    application, given by the corresponding Open. 		
	*/

	public  static native final void Close(int ref);
	
	/**
  	Connects or disconnects two applications. The Connect
    function allows the switching on or off of a connection between
    a source application and a destination application. There is no
    restrictions in the establishing of these connections, an
    application can be the source or destination of as many other
    applications as you wish. Loops are permitted. 
     
	*@param src  is the reference number of the source application. 
    *@param dst is the reference number of the destination application. 	

	*@param state indicates if a connection must be switched on (1 = True) or off (0 = False).	
	*/
	
	public  static native final void Connect(int src, int dst, int state);
	
	/**
  	Duplicates an event, taking into account the structure of the
    event. It can be used to copy any type of events, from simple
    notes to large system exclusives. 		

	*@param event is a pointer to the event to be copied.
    *@return   The result is a  pointer to the copy if the operation
     was successful. The result is 0 if MidiShare was not able to
     allocate enough memory space for the copy. 		
	*/
	
	public  static native final long CopyEv(long event);
	
	/**
  	Gives the number of Midi applications currently active. 
  
	 *@return   The result is  the number of currently opened
     Midi applications. 		
	*/

	public  static native final int CountAppls();
	  		static native final int CountDTasks(int ref);
	  		
	/**
  	Gives the number of events waiting into the application reception FIFO. 
	*@param ref is the reference number of the application.
  
	*@return The result is the number of waiting events in
    the reception FIFO. 		
	*/

	public  static native final int CountEvs(int ref);
	
	/**
  	Gives the number of fields of an event.  
   	*@param event is a pointer to the concerned event.   
	*@return   The result is the number of fields of the event. 		
	*/

	public  static native final int CountFields(long event);
	 		static native final long DTask(long proc, long date, int ref, int a1, int a2, int a3);
			static native final void Exec1DTask(int ref);
			
	/**
    Converts an external time in millisecond to the value of an
    internal time. The convertion is made by subtracting the current
    offset between internal and external time. 

   	*@param time is in milliseconds.    
	*@return The result is the corresponding internal time in milliseconds.		
	*/

	public  static native final long Ext2IntTime(long time);
			static native final void FlushDTasks(int ref);
			
	/**
    Flushes all the waiting events in the reception FIFO of the
    application. 
    *@param ref  is the reference number of the application. 		
	*/

	public  static native final void FlushEvs(int ref);
			static native final void ForgetTask(long ref);
			
	/**
    Frees a cell allocated by NewCell function. This is the
    lowest level command for accessing the MidiShare Memory
    Manager. One must be sure to use FreeCell on an
    individual cell allocated with NewCell and not on complete
    MidiShare events. Not doing so may result in the lose of cells. 
    *@param cell is a pointer to a basic cell of 16 bytes.	
	*/

	public  static native final void FreeCell(long cell);
	
	/**
  	Frees a MidiShare event allocated with NewEv.
    FreeEv takes into account the event structure by checking
    the events type. For this reason, FreeEv must not be used
    on cell allocated with NewCell.     
    *@param event is a pointer to a basic cell of 16 bytes.	
	*/

	public  static native final void FreeEv(long event);
	
	/**
  	Frees a sequence and its content. FreeSeq first
    de-allocates all the events of the given sequence and then the
    sequence header itself.    
    *@param seq is a pointer on a sequence to be freed. 	
	*/
	public  static native final void FreeSeq(long seq);
	
	/**
  	Returns the available free MidiShare event space.
    FreeSpace allows to know at any time the number of cells
    remaining available from the MidiShare memory manager. 
    *@return The result is the number of available free cells
    in the MidiShare memory manager.
    */

	public  static native final int FreeSpace();
			static native final long GetApplAlarm(int ref);
			
	/**
	Extracts the first event on in the reception FIFO. The received
	events, stored automatically by MidiShare in the application
	reception FIFO, can be picked up by successive calls to
	GetEv function. 
	*@param ref is the reference number of the application. 
	*@return The result is a pointer to the first event in the reception
	FIFO, or 0 if the FIFO is empty. The event is extracted
	from the reception FIFO.     
	*/

	public  static native final long GetEv(int ref);
	
	/**
    Gives the current external time i.e. the position of the tape
    converted in milliseconds.    
     
    *@return The result is the external time.
    */
	public  static native final long GetExtTime();
	
	/**
    Gives the index field value of an event. Field index start from 0.
    Depending of the event type and field nature, the field format
    can be 8, 16 or 32-bit. GetField deals with all the format
    conversion and the result is always a 32-bit integer. 
     
    *@param event is a pointer to the event to be accessed. 
    *@param field is the field number to be read
    (numbered from 0).   
    *@return The result is the value of the field. Fields are
    considered as unsigned. 
    */
	
	public  static native final int GetField (long event, int field);
	
	/**
    Gives the associated filter of an application. Each application
    can select the events to be received by using a filter. The
    filtering process is local to the application and has no influence
    on the events received by other applications. 
          
    *@param ref is the reference number of the application. 
    *@return The result is  a pointer to the filter associated to the
    application, or 0 if there is no such filter (in this case the
    application accepts any events).     
    */
	
	public  static native final long GetFilter(int ref);
	
	/**
  	Gives the reference of number of an application from is order
    number. The GetIndAppl function allows to know the
    reference number of any application by giving its order number
    (a number between 1 and CountAppls()). 
               
    *@param index is the index number of an application.
    *@return The result is an application reference number or errIndex
    if the index is out of range.     
    */
	public  static native final int GetIndAppl(int index);
			static native final long GetInfo(int ref);
	
	/**
  	Gives the name of an application. Knowing an application
    reference number, it is possible to find its name using the
    GetName function. It is also possible to find the reference
    number of an application via its name using the
    GetNamedAppl function. 
               
    *@param ref is the reference number of the application.       
    *@return The result is a string representing the application name.     
    */
		
	public  static native final String GetName(int ref);		

	/**
  	Returns the reference number of an application. Knowing an
    application name, it is possible to find its reference number
    using the GetNamedAppl function. It is also possible to
    find the name of an application via its reference number using
    the GetName function. 
     
    *@param midiname the application name.      
    *@return The result is the reference number of the application.
    */
	
	public  static native final int GetNamedAppl(String midiname);

	/**
  	*@deprecated see new drivers management functions.
	*@see grame.midishare.Midi#CountDrivers
    */
	
	public  static native final int GetPortState(int port);
			static native final long GetRcvAlarm(int ref);
	
	/**
  	Fills a SyncInfo object with information about the current
    state of the MTC synchronisation. 
     
                    
    *@param info  a SyncInfo object.     
    */

	public  static native final void GetSyncInfo(SyncInfo info);
	
	/**
  	Gives in milliseconds the time elapsed since the starting up of
    MidiShare.    
      
    *@return The result is a 32-bit integer, being the elapsed time in
    milliseconds since the starting up of MidiShare.        
    */
    
	public  static native final long GetTime();
	
	/**
  	Gives the version number of MidiShare.                  
    *@return The result is  the MidiShare version number. A
    result of 161 means "version 1.61".      
    */

	public  static native final int GetVersion();
		
	/**
  	Tries to increase the memory space of MidiShare.                     
    *@param space  the number of cells to increase the MidiShare memory space.  
    *@return The result is a 32-bit integer, the number of new cells allocated.   
    */

	public  static native final int GrowSpace(int space);
	
	/**
  	Convert an internal time in millisecond to an external time. The
    convertion is made by adding the current offset between
    internal and external time.                      
    *@param time in milliseconds        
    *@return The result is the corresponding external time,a 32-bits value in milliseconds.        
    */

	public  static native final long Int2ExtTime (long time);
	
	/**
    Gives the state of a connection between two MidiShare
    applications. Connections allow real-time communications of
    midi events between applications. 
                        
    *@param src  is the reference number of a source application.
    *@param dest  dest is the reference number of a destination application .       
    *@return The result is 1 (= True) when a connection exist between the source
    and the destination, and 0 (= False) otherwise.         
    */

	public  static native final int IsConnected (int src, int dest);
	
	/**
    Allocates a simple memory cell from the MidiShare memory
    manager. For some special application, it may be useful to have
    access to the basic functions of the memory manager. All the
    events managed by MidiShare are implemented from
    fixed-sized cells (16 bytes). 
                             
    *@return The result is a pointer to a memory cell, or 0 when memory space is exhausted.         
    */

	public  static native final long NewCell ();
	
	/**
    Allocates a new event of desirable type. 
             
    *@param type the type of event to be allocated.                      
    *@return The result is a pointer to a MidiShare event of the
    desired type, or 0 if the MidiShare memory space is
    exhausted.      
    */

	public  static native final long NewEv(int type);
	
	/**
    Allocation of a new empty sequence.              
    *@return The result is a pointer to an empty sequence.  
    */

	public  static native final long NewSeq();
	
	/**
    Opening of MidiShare. Open allows the recording of
    some information relative to the application context (its name,
    the value of the global data register, etc.), to allocate a
    reception FIFO and to attribute a unique reference number to
    the application. In counterpart to any Open call, the
    application must call the Close function before leaving, by
    giving its reference number as an argument. MidiShare can thus
    be aware of the precise number of active Midi applications.  
             
    *@param midiname the name of the application.                      
    *@return The result is a unique reference number identifying the
    application.  
    */
    
	public 	static  native final int Open(String midiname); 		

			static native final long ReadSync(long adr);
			
	/**
    Sends an event. A copy of the event is sent to all the
    application destinations. The date field of the event is used to
    specify when the destinations will actually receive the event. 
             
    *@param ref is the reference number of the application.                       
    *@param  event  is a pointer to the event to send.     
    */
		
			
	public  static native final void Send(int ref, long event);
	
	/**
    Sends an event. A copy of the event is sent to all the
    application destinations. The date argument is used to specify
    when destinations will actually receive the event. 
                  
    *@param ref is the reference number of the application.                       
    *@param  event  is a pointer to the event to send. 
    *@param  date ia a 32-bit integer, the date when destinations will receive the event.    
    */

	public  static native final void SendAt(int ref, long event, long date);
	
	/**
    Immediately sends an event. A copy of the event is sent to all
    the application destinations. 
     
    *@param ref is the reference number of the application.                       
    *@param  event  is a pointer to the event to send.     
    */		
	
	public  static native final void SendIm(int ref, long event);
			static native final void SetApplAlarm(int ref, long alarm);
			
	/**
    Attributes a value to a field of an event. The access to the
    compulsory fields of the event is done directly. But the access
    to the variables fields is achieved through the SetField and
    GetField functions. The function deals with the conversion
    of this value into the concerned field format (8, 16 or 32-bit). 
          
    *@param event is a pointer to the event to be modified.                       
    *@param field is a 32-bit integer, the index number of the field to modify (from 0 to CountFields(e) -1).    
    *@param val is 32-bit value to put in the field. This value will beconverted to the right size (8, 16 or 32-bit).  
    */		
			
	public  static native final void SetField(long event, int field, int val);
		
	/**
  	Associates a filter to an application. Each application can select
    the events to be received by using a filter. The filtering process
    is local to the application and has no influence on the events
    received by the other applications. The implementation of these
    filters is achieved by two routines : SetFilter and
    GetFilter.      
          
    *@param ref is the reference number of the application.                  
    *@param filter is a pointer to the application filter. 
    */
	
	public  static native final void SetFilter(int ref, long filter);
			static native final void SetInfo(int ref, long info);
		
	/**
  	Changes the name of an application.  
          
    *@param ref is the reference number of the application.                  
    *@param midiname, the new application name.     
    */
  
  	public  static  native final void SetName(int ref, String midiname);

	/**
	*@deprecated see new drivers management functions.
	*@see grame.midishare.Midi#CountDrivers
    */

	public  static native final void SetPortState(int port,int state);
			static native final void SetRcvAlarm(int ref,long alarm);
			
	/**
 	Set the synchronisation mode of MidiShare.              
    *@param mode  an unsigned 16-bits word of structure 'xarrrrrrpppppppp'. 
    x, b15 is used to choose between internal synchronisation (x=0) and external synchronisation (x=1) 
    a, b14 is used to choose between synchronisation on port p (a=0) and synchronisation on any port (a=1) 
    r, b13_b8  is reserved for future use and must be set to 0. 
    p, b7_b0 is the synchronisation port to be used when x=1 and a=0. 
    When a=1 the port number is ignored, the first port with incoming MTC is used.     
    */
			
	public  static native final int SetSyncMode(int mode);
	
	/**
  	Test if MidiShare is available on the Machine. The function actually tries to load the JMidi native library
  	and return a result according to the loading process result. 
  	Applications *must* call this function first, otherwise next calls to the MidiShare API will fail.
             
    *@return The result is true (= 1) if MidiShare is available, false (= 0) otherwise.
    */
	
	public static final int Share()
	{
		if (interfaceLoaded){
			return 1;
		}else{
			try {
				String arch = System.getProperty("os.arch");
				if (arch.equals("x86"))
					System.loadLibrary("JMidi32");
				else
					System.loadLibrary("JMidi64");
				interfaceLoaded = true;
				return 1;
	      	} catch (UnsatisfiedLinkError e) {
				e.printStackTrace();
				return 0;
		  	}
		}	
	 }	
			
	/**
  	Convert an SMPTE location to a time in millisecond.  
             
    *@param loc is a pointer to a SmpteLoc object to be converted in milliseconds.              
    *@return The result is a 32-bits time in milliseconds 
    */
    
	public  static native final int Smpte2Time(SmpteLoc loc);
			static native final long Task(long proc, long date, int ref, int a1,int a2,int a3);
			
	/**
  	Convert a time in millisecond to an SMPTE location.              
    
    *@param time is a 32-bits time in milliseconds to convert in an SMPTE location. 
    *@param format is
          a 16-bits integer, the SMPTE format to be used : (0 : 24
          f/s, 1 : 25 f/s, 2 : 30DF f/s, 3 : 30 f/s). 
    *@param loc is
          a pointer to a SmpteLoc object to be filled with
          the resulting SMPTE location. 
    */

	public  static native final void Time2Smpte(long time, int format, SmpteLoc loc);
	
	/**
  	Gives the total number of cells allocated to MidiShare.
    TotalSpace allows an application to know at any time the
    total number of cells allocated by the MidiShare memory
    manager at startup.               
    
    *@return The result is a 32-bit integer, the total number of cells in the MidiShare memory manager. 
  	*/
     
	public  static native final int TotalSpace();
			static native final long WriteSync(long adr, long val);


	/* for common fieds management */

	/**
	Return the link field of a Midi event (Midi events are linked together in a Midi sequence for instance).
		
	*@param event is a pointer to the event.
    *@return The result is a pointer to the event link field.  
    */

	public   static native final long GetLink(long event);
	
	/**
	Set the link field of a Midi event with a new event.		
		 
	*@param event is a pointer to the event.
    *@param link is a pointer to the new event.
  	*/

	public   static native final void SetLink(long event,long link);

	/**
	Return the date field of a Midi event.
		
	*@param event is a pointer to the event.
    *@return The result is a 32-bits date in milliseconds. 
    */

	public   static native final long GetDate (long event);
	
	/**
	 Set the date field of a Midi event with a new date.		
		 
	*@param event is a pointer to the event.
    *@param date is the new date.
    */

	public   static native final void SetDate(long event,long date);

	/**
	Return the refnum field of a Midi event.
		
	*@param event is a pointer to the event.
    *@return The result is the event refnum
    */

	public   static native final int GetRefnum(long event);
	
	/**
	Set the refnum field of a Midi event with a new refnum.		
		 
	*@param event is a pointer to the event.
    *@param ref is the new refnum.
  	*/

	public   static native final void SetRefnum(long event,int ref);

	/**
	Return the type field of a Midi event.
		
	*@param event is a pointer to the event.
    *@return The result is the event type
    */

	public   static native final int GetType(long event);
	
	/**
	Set the type field of a Midi event with a new type. The type
	of an event should be change with care since it is related to the event
	structure. 
		 
	*@param event is a pointer to the event.
    *@param type is the new type.
   */

	public   static native final void SetType(long event,int type);

	/**
	Return the channel field of a Midi event.
		
	*@param event is a pointer to the event.
    *@return The result is the event channel.
    */

	public   static native final int GetChan(long event);
	
	/**
	Set the channel field of a Midi event with a new channel. 
		 
	*@param event is a pointer to the event.
    *@param chan is the new channel.
    */

	public   static native final void SetChan(long event,int chan);

	/**
	Return the port field of a Midi event.
		
	*@param event is a pointer to the event.
    *@return The result is the event port.
    */

	public   static native final int GetPort(long event);
	
	/**
	Set the port field of a Midi event with a new port. 
		 
	*@param event is a pointer to the event.
    *@param port is the new port.
 	*/

	public   static native final void SetPort(long event,int port);	
	
	public   static native final int GetData0(long event);
	public   static native final int GetData1(long event);
	public   static native final int GetData2(long event);
	public   static native final int GetData3(long event);
	
	public   static native final void SetData0(long event,int data);
	public   static native final void SetData1(long event,int data);
	public   static native final void SetData2(long event,int data);
	public   static native final void SetData3(long event,int data);
	
	/* for event with text */

	/**
	Return the text field of a Midi event. This function must be used
	only for "text events" like typeText, typeSeqName... etc..
		
	*@param event is a pointer to the event.
    *@return The result is a string. 
    */

	public   static  native final String GetText(long event); 		

	/**
	Set the text field of a Midi event. This function must be used
	only for "text events" like typeText, typeSeqName... etc..
 
		 
	*@param event is a pointer to the event.
    *@param text is the new text.
  	*/

	public   static  native final void SetText(long event, String text); 

	/* for sequences */

	/**
	Return the first event  of a Midi sequence.
		
	*@param seq is a pointer to the sequence.
    *@return The result is the first event of the sequence.
    */

	public   static native final long GetFirstEv(long seq);
	
	/**
	Set the first event of a Midi sequence. 
		 
	*@param seq is a pointer to the sequence.
    *@param event is the new event
	*/

	public   static native final void SetFirstEv(long seq, long event);

	/**
	Return the last event  of a Midi sequence.
		
	*@param seq is a pointer to the sequence.
    *@return The result is the last event of the sequence.
    */

	public   static native final long GetLastEv(long seq);
	
	/**
	Set the last event of a Midi sequence. 
		 
	*@param seq is a pointer to the sequence.
    *@param event is the new event
     */

	public   static native final void SetLastEv(long seq, long event);

	/* for filters */
	
	/**
  	Allocate a new a application filter. The implementation of the
    filters is achieved by two routines : SetFilter and
    GetFilter.      
          
    *@return  The result is a pointer to the new filter.
    */

	public  static native final long NewFilter();
	
	/**
  	Free an application filter. 
          
    *@param filter is a pointer to the filter.
   	*/

	public  static native  final void FreeFilter(long filter);
	
	/**
  	Modify the application filter by accessing the filter bits.  
          
    *@param filter is a pointer to the filter.
    *@param port  is a port number to be accessed in the filter (0 to  255).
    *@param val  if set to 1, the event on this port will be accepted,
    	if set to 0, the event on this port will be rejected.
  	*/

	public  static native final void AcceptPort(long filter, int port, int val);
	
	/**
  	Modify the application filter by accessing the filter bits.  
          
    *@param filter is a pointer to the filter.
    *@param chan  is a channel number to be accessed in the filter (0 to 15).
    *@param val   if set to 1, the event on this channel will be accepted,
    	if set to 0, the event on this channel will be rejected.
  	*/

	public  static native final void AcceptChan(long filter, int chan, int val);
	
	/**
  	Modify the application filter by accessing the filter bits.  
          
    *@param filter is a pointer to the filter.
    *@param type  is a type number to be accessed in the filter (0 to  255).
    *@param val   if set to 1, the event of this type will be accepted,
    	if set to 0, the event of this type will be rejected.
  	*/
    
	public  static native final void AcceptType(long filter, int type, int val);
	
	/**
  	Test the filter port state.  
          
    *@param filter is a pointer to the filter.
    *@param port  is a port number to be accessed in the filter (0 to  255).
   	*/

	public  static native final int IsAcceptedPort(long filter, int port);
	
	/**
  	Test the filter channel state.    
          
    *@param filter is a pointer to the filter.
    *@param chan  is a channel number to be accessed in the filter (0 to 15).
  	*/

	public  static native final int IsAcceptedChan(long filter, int chan);
	
	/**
  	Test the filter type state.   
          
    *@param filter is a pointer to the filter.
    *@param type  is a type number to be accessed in the filter (0 to  255).
   	*/
    
	public  static native final int IsAcceptedType(long filter, int type);	
	
	/** Slot direction code: input slot
 	*/
	public static final int InputSlot		= 1;
	/** Slot direction code: output slot
 	*/
	public static final int OutputSlot		= 2;
	/** Slot direction code: input/output slot
 	*/
	public static final int InputOutputSlot	= 3;
	
	/**
  	Gives the number of MidiShare drivers currently registered. 
    *@return The result is a 16-bit integer, the number of currently registered drivers
   	*/
		
	public  static native final int CountDrivers();
	
	/**
  	Gives the reference of number of a driver from its order number. The GetIndDriver function
     allows to know the reference number of any driver by giving its order number (a number between
    1 and CountDrivers()). 
	*@return The result is a 16-bit integer, the index number of a driver between 1 and CountDrivers(). 
   	*/
   	
	public  static native final int	GetIndDriver(int index);
	
	/**
  	Gives information about a driver: it includes the driver name, its version number and its slots
    count.
   	*@param  ref a 16-bit integer, it is the reference number of the driver. 
    *@param  infos a DriverInfos structure to be filled with the driver characteristics. 
    *@return The result is a Boolean value which indicates whether MidiShare has been able to get information
              about the driver or not.

   	*/
	public  static native final int GetDriverInfos(int refnum, DriverInfos infos);

	
	/**
  	Add a new slot to a driver. MidiShare applications owning a "context alarm" will be informed
    of the new slot with the AddSlot alarm code.
   	*@param drvRefNum the MidiShare reference number of the owner driver. 
   	*@param  name the slot name. 
    *@param  direction the slot direction which may be in InputSlot, OutputSlot or InputOutputSlot 
    *@return The result is a 32-bit integer, a MidiShare unique slot reference number.
   	*/
		
	public  static native final int AddSlot (int refnum, String slotname, int direction);
	
	/**
  	Gives the reference of number of a driver slot from its order number. The GetIndSlot
    function allows to know the reference number of all the slots declared by a driver by giving its
    order number. 
    *@param ref a 16-bits integer, it is a driver reference number. 
    *@param index a 16-bits integer, it is the index number of the slot, between 1 and the DriverInfos.slots field. 
    *@return  The result is a 32-bit integer, the slot reference number, or errRefNum if the reference
              number is incorrect, or errIndex if the index is out of range. 
   	*/
   	
	public  static native final int	GetIndSlot(int refnum, int index);
			
	/**
  	Removes a slot from the system. MidiShare applications owning a "context alarm" will be
    informed of the change using the RemoveSlot alarm code. 
    *@param ref, a slot reference number. 
   	*/
		
	public  static native final	void RemoveSlot(int slot);
		
	/**
  	Gives information about a slot: it includes the slot name, the slot direction and its connection set to
    the MidiShare ports
    *@param  ref a 32-bit integer, the slot reference number. 
    *@param  infos  a SlotInfos structure to be filled with the slot characteristics. 
    *@return The result is a Boolean value which indicates whether MidiShare has been able to get information
              about the slot or not.
   	*/
 
	public  static native final int GetSlotInfos(int slot, SlotInfos infos); 

	/**
 	 Make or remove a connection between a slot and a MidiShare logical port. MidiShare
     applications owning a "context alarm" will be informed of the connection change using the
     ChgSlotConnect alarm code.
     
     *@param port a 16-bit integer, it is a MidiShare port number. 
     *@param slot a 32-bit integer, it is the slot reference number. 
     *@param state a Boolean value to specify whether the connection should be done (true) or removed (false). 
   	*/
   	   	
	public  static native final	void ConnectSlot(int port, int slot, int state);
	
	/**
  	Gives the state of a connection between a MidiShare port and a driver slot. 
  	*@param port  a 16-bits integer, it is a MidiShare port number 
    *@param ref the slot reference number 
    *@return The result is true when a connection exist between the port and the slot and false otherwise.  
   	*/
	public  static native final	int	IsSlotConnected(int port, int slot);
	
	/**
  	Changes the name of a slot.  
          
    *@param slot the slot reference number .                  
    *@param slotname the new slot name.     
    */
	public  static native final	void SetSlotName(int slot, String slotname);
	
	/* linearisation functions */
	
	/** For internal use only. */
	public  static native final int WriteEv(WriteEvInfo info);
	/** For internal use only. */
	public  static native final int ReadEv(ReadEvInfo info);	
 }
