; /*****************************************************************************
;  *                                MIDI SHARE
;  *----------------------------------------------------------------------------
;  * MidiShare is a multi-tasking, real-time software environment, specially
;  * devised for the developing of MIDI applications with a triple target :
;  *
;  * 	-To propose solutions to currently met problems when developing
;  * 	 a real-time MIDI application : memory management, communication management,
;  *	 time management, task management.
;  *
;  * 	-To enable the real-time and multi-tasking functioning of these applications,
;  * 	 i.e. to enable the sharing of all the necessary resources and their
;  * 	 simultaneous access.
;  *
;  * 	-To make easier cooperation between independent MIDI applications by  proposing
;  * 	 a real-time mechanism of inter-application communications.
;  *
;  * This file contains a complete description of all the MidiShare functions and
;  * procedures, as well as all the data structures in use.
;  *----------------------------------------------------------------------------
;  * 		                    c GRAME 1989-1997
;  *	          [Yann Orlarey, Herv‚ Lequay, Dominique fober]
;  *----------------------------------------------------------------------------
;  ******************************************************************************/

; /*******************************************************************************
;  * MIDISHARE EVENTS
;  *------------------------------------------------------------------------------
;  * 	The listing below presents the different types of MidiShare handled events.
;  * 	This typology contains the whole of the standard Midi messages, plus specific
;  * 	messages such as typeNote corresponding to a note with its duration;
;  * 	or typeStream corresponding to a series of arbitrary bytes, possibly including
;  * 	data and status codes, sent directly without any processing; or typePrivate
;  * 	that are application private messages.
;  *
;  * 	All these codes may be used in the MidiNewEv function to allocate an event
;  * 	of the desirable type and are accessible in an event evType field.
;  *******************************************************************************/

;    From Jacques.Herry@wanadoo.fr 
;            Jacques Herry 
;           35, resid Corlay 
;           22300 Lannion 
;            France 
;  	Grame  MidiShare for schemers. 
;  "Welcome to Grame MidiShare(c)extension player, version 1.6.22 Copyright(c)2001 Jacques Herry" 


;;******** NOTE I keep  symbols names from C for your convenience *******/

(define typeNote		0); 	/* note with pitch, velocity and duration	*/

(define typeKeyOn		1); 	/* Note On with pitch, velocity 	*/
(define typeKeyOff		2); 	/* Note Off with pitch, velocity 	*/
(define typeKeyPress 	3); 	/* Poly Key Pressure with pitch and pressure	*/
(define typeCtrlChange	4); 	/* Control Change with controller ID and value	*/
(define typeProgChange	5); 	/* Program Change with program ID number	*/
(define typeChanPress	6); 	/* Channel Pressure with pressure value		*/
(define typePitchWheel	7); 	/* Pitch Bend Change with LSB and MSB values	*/

(define typeSongPos		8); 	/* Song Position Pointer with LSB and MSB values*/
(define typeSongSel		9); 	/* Song Select with song ID number	*/
(define typeClock		10); 	/* Timing Clock		*/
(define typeStart		11); 	/* Start		*/
(define typeContinue	12); 	/* Continue */
(define typeStop		13);	/* Stop		*/
(define typeTune		14);	/* Tune Request	*/
(define typeActiveSens	15);	/* Active Sensing      */
(define typeReset		16);	/* System Reset	*/

(define typeSysEx		17);	/* System Exclusive (only data bytes)	*/
(define typeStream		18);	/* arbitrary midi bytes (data and status codes)	*/
(define typePrivate		19);	/*19..127 Private events for applications internal use*/

(define typeProcess			128);	/* used by MidiShare for MidiCall and MidiTask	*/
(define typeDProcess		129);	/* used by MidiShare for MidiDTask	*/
(define typeQuarterFrame 	130);	/* Midi time code quarter frame		*/

(define typeCtrl14b		131);
(define typeNonRegParam	132);
(define typeRegParam	133);

(define typeSeqNum		134);	/* sequence number	*/
(define typeTextual		135);	/* text event		*/
(define typeCopyright	136);	/* message copyright message	*/
(define typeSeqName		137);	/* sequence or track name*/
(define typeInstrName	138);	/* instrument	name	*/
(define typeLyric		139);	/* lyrics	*/
(define typeMarker		140);	/* marker	*/
(define typeCuePoint	141);	/* cue point	*/
(define typeChanPrefix	142);	/* Midi Channel Prefix	   */
(define typeEndTrack	143);	/* end of a track	*/
(define typeTempo		144);	/* changement de tempo	*/
(define typeSMPTEOffset	145);	/* smpte offset	*/

(define typeTimeSign	146);	/* time signature	*/
(define typeKeySign		147);	/* signature tonale	*/
(define typeSpecific	148);	/* sequencer specific meta event	*/
(define typePortPrefix	149);	/* sequencer specific meta event	*/
(define typeRcvAlarm	150);	/* RcvAlarm			*/
(define typeReserved	151);	/* ApplAlarm		*/

(define typeReserved	152);	/*152..254	reserved for future extensions	*/

(define typeDead	255);	/* dead Task or DTask	*/



; /******************************************************************************
; * MIDI STATUS CODE
; *******************************************************************************/

(define NoteOff		#x80)
(define NoteOn		#x90)
(define PolyTouch	#xa0)
(define ControlChg	#xb0)
(define ProgramChg	#xc0)
(define AfterTouch	#xd0)
(define PitchBend	#xe0)
(define SysRealTime	#xf0)
(define SysEx		#xf0)
(define QFrame		#xf1)
(define SongPos		#xf2)
(define SongSel		#xf3)
(define UnDef2		#xf4)
(define UnDef3		#xf5)
(define Tune		#xf6)
(define EndSysX		#xf7)
(define MClock		#xf8)
(define UnDef4		#xf9)
(define MStart		#xfa)
(define MCont		#xfb)
(define MStop		#xfc)
(define UnDef5		#xfd)
(define ActSense	#xfe)
(define MReset		#xff)


; /*******************************************************************************
; * MIDI PORT
; *------------------------------------------------------------------------------
; * my config
;*******************************************************************************/

(define sb-port	0)
(define yamaha-port	1)


; /******************************************************************************
; * ERROR CODES
; *------------------------------------------------------------------------------
; * List of the error codes returned by some MidiShare functions.
; *******************************************************************************/

(define MIDIerrSpace 	-1);		/* no space left in the freeList */
(define MIDIerrRefNum	-2);		/* bad reference number */
(define MIDIerrBadType	-3);		/* bad event type */
(define MIDIerrIndex	-4);		/* bad access index (events) */
(define MIDIerrEv 		-5);		/* event argument is nil*/
(define MIDIerrUndef 	-6);		/* event argument is undef 	*/


; /******************************************************************************
; * SYNCHRONISATION CODES
; *------------------------------------------------------------------------------
; * List of the error codes returned by some MidiShare functions.
; *******************************************************************************/

(define MIDISyncExternal 	#x8000);		/* bit-15 for external synchronisation */
(define MIDISyncAnyPort		#x4000);		/* bit-14 for synchronisation on any port */

;/** scheme specific code */
(define-macro while (lambda(test . Lcorps)
  (let ((loop (gensym)))
    `(letrec ((,loop (lambda ()
		       (if ,test
			     (begin ,@Lcorps (,loop))))))
	      (,loop)))))

;; list of possibles chan for use with apply 1 to 16
(define *all-chans* '(0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15))



