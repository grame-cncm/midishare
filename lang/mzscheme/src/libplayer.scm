;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;;  mzPlayer.scm
;;
;;  
;;   Interface to the Player Library
;;
;;   
;;   6/6/2001 : Interface to be used with the THUNK version of MidiShare 16 bit.
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;    From Jacques.Herry@wanadoo.fr 
;            Jacques Herry 
;           35, resid Corlay 
;           22300 Lannion 
;            France 
;  	Grame  Player library for schemers. 
;  "Welcome to Grame MidiShare(c)extension player, version 1.6.22 Copyright(c)2001 Jacques Herry" 



;; The player3216.dll file must be located either in the Windows Folder
;; or in the same folder as the Player32.lsp file.

;(load-extension "mzplayer3216.dll")


;; Date structures ond constants
;;===============================

;;-------------------------------------------------------------------------- 
;; Player state   
;;-------------------------------------------------------------------------- 


(define kIdle       0)
(define kPause      1)
(define kRecording  2)
(define kPlaying    3)
(define kWaiting    4)


;;-------------------------------------------------------------------------- 
;; Tracks state   
;;-------------------------------------------------------------------------- 

(define kMaxTrack	256)
(define kMuteOn  	1)
(define kMuteOff   	0)
(define kSoloOn  	1)
(define kSoloOff   	0)
(define kMute  		0)
(define kSolo  		1)


;;------------------------------------------------------------------ 
;; Recording management  
;;-------------------------------------------------------------------------- 

(define kNoTrack		-1)
(define kEraseMode  	1)
(define kMixMode 		0)

;;-------------------------------------------------------------------------- 
;; Loop  management  
;;-------------------------------------------------------------------------- 

(define kLoopOn  	0)
(define kLoopOff 	1)


;;-------------------------------------------------------------------------- 
;; Step Playing  
;;-------------------------------------------------------------------------- 

(define kStepPlay  1)
(define kStepMute  0)

;;-------------------------------------------------------------------------- 
;; Synchronisation  
;;-------------------------------------------------------------------------- 

(define kInternalSync	0)
(define kClockSync  	1)
(define kSMPTESync 		2)
(define kExternalSync   3)


(define kNoSyncOut		0)
(define kClockSyncOut  	1)


;;-------------------------------------------------------------------------- 
;; MIDIfile  
;;-------------------------------------------------------------------------- 

(define midifile0	0)
(define midifile1  	1)
(define midifile2  	2)


(define TicksPerQuarterNote	0)
(define Smpte24			24)
(define Smpte25			25)
(define Smpte29			29)
(define Smpte30			30)


;;-------------------------------------------------------------------------- 
;; Errors  : for the player
;;-------------------------------------------------------------------------- 
 
(define PLAYERnoErr 			-1)		;; no error			            		 
(define PLAYERerrAppl			-2)		;; Unable to open MidiShare application	 
(define PLAYERerrEvent  		-3)		;; No more MidiShare Memory 			 
(define PLAYERerrMemory	        -4)		;; No more  Memory 			       
(define PLAYERerrSequencer		-5)		;; Sequencer error
			          


 ;;-------------------------------------------------------------------------- 
 ;; Errors  :  for MidiFile
 ;;-------------------------------------------------------------------------- 

(define noErr				0)		;; no error 					 
(define ErrOpen				1)		;; file open error 	 
(define ErrRead				2)		;; file read error	 
(define ErrWrite			3)		;; file write error	 
(define ErrVol				4)		;; Volume error 	 
(define ErrGetInfo 			5)		;; GetFInfo error	 
(define ErrSetInfo			6)		;; SetFInfo error	 
(define ErrMidiFileFormat	7)	    ;; bad MidiFile format	 


