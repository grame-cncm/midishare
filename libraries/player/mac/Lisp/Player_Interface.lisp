;; =====================================================================================
;; The Player Library is Copyright (c) Grame, Computer Music Research Laboratory 
;; 1996-2000, and is distributed as Open Source software under the Artistic License;
;; see the file "Artistic" that is included in the distribution for details.
;;
;; Grame : Computer Music Research Laboratory
;; Web : http://www.grame.fr/Research
;; E-mail : MidiShare@rd.grame.fr
;; =====================================================================================


;; This file contains definitions for entry points of the Player library
;; It must be used with the Player.Framework located in /System/Library/Frameworks


;; Utilities
;;===========

(defvar *player-framework* nil)

(defun player-framework ()
  (or *player-framework*
      (setq *player-framework*
            (load-framework-bundle "Player.framework"))))

(player-framework)

;;===============================
;; Date structures ond constants
;;===============================

;;-------------------------------------------------------------------------- 
;; Player state   
;;-------------------------------------------------------------------------- 

(defparameter kIdle       0)
(defparameter kPause  	  1)
(defparameter kRecording  2)
(defparameter kPlaying    3)
(defparameter kWaiting    4)


;;-------------------------------------------------------------------------- 
;; Tracks state   
;;-------------------------------------------------------------------------- 

(defparameter kMaxTrack	256)
(defparameter kMuteOn  	1)
(defparameter kMuteOff  0)
(defparameter kSoloOn  	1)
(defparameter kSoloOff  0)
(defparameter kMute  	0)
(defparameter kSolo  	1)

;;-------------------------------------------------------------------------- 
;; Recording management  
;;-------------------------------------------------------------------------- 

(defparameter kNoTrack		-1)
(defparameter kEraseMode  	1)
(defparameter kMixMode 		0)

;;-------------------------------------------------------------------------- 
;; Loop  management  
;;-------------------------------------------------------------------------- 

(defparameter kLoopOn  	0)
(defparameter kLoopOff 	1)

;;-------------------------------------------------------------------------- 
;; Step Playing  
;;-------------------------------------------------------------------------- 

(defparameter kStepPlay  1)
(defparameter kStepMute  0)

;;-------------------------------------------------------------------------- 
;; Synchronisation  
;;-------------------------------------------------------------------------- 

(defparameter kInternalSync	0)
(defparameter kClockSync  	1)
(defparameter kSMPTESync 	2)
(defparameter kExternalSync 	3)

(defparameter kNoSyncOut	0)
(defparameter kClockSyncOut  	1)

;;-------------------------------------------------------------------------- 
;; MIDIfile  
;;-------------------------------------------------------------------------- 

(defparameter midifile0	 0)
(defparameter midifile1  1)
(defparameter midifile2  2)

(defparameter TicksPerQuarterNote	0)
(defparameter Smpte24			24)
(defparameter Smpte25			25)
(defparameter Smpte29			29)
(defparameter Smpte30			30)

;;-------------------------------------------------------------------------- 
;; Errors  : for the player
;;-------------------------------------------------------------------------- 
 
(defparameter PLAYERnoErr 			-1)		;; No error			            		 
(defparameter PLAYERerrAppl			-2)		;; Unable to open MidiShare application	 
(defparameter PLAYERerrEvent  		-3)		;; No more MidiShare Memory 			 
(defparameter PLAYERerrMemory		-4)		;; No more Mac Memory 			       
(defparameter PLAYERerrSequencer	-5)		;; Sequencer error			          

 ;;-------------------------------------------------------------------------- 
 ;; Errors  :  for MidiFile
 ;;-------------------------------------------------------------------------- 

(defparameter noErr			0)		;; no error 						 
(defparameter ErrOpen		1)		;; file open error 	 
(defparameter ErrRead		2)		;; file read error	 
(defparameter ErrWrite		3)		;; file write error	 
(defparameter ErrVol			4)		;; Volume error 	 
(defparameter ErrGetInfo 		5)		;; GetFInfo error	 
(defparameter ErrSetInfo		6)		;; SetFInfo error	 
(defparameter ErrMidiFileFormat	        7)	        ;; bad MidiFile format	 

;;================================
;; Record for position management
;;================================

(defrecord Pos
  (bar  :short)  
  (beat :short)     
  (unit :short))

(defmacro p-bar (e &optional (d nil d?))
  (if d?
    `(rset ,e :Pos.bar ,d)
    `(rref ,e :Pos.bar)))

(defmacro p-beat (e &optional (d nil d?))
  (if d?
    `(rset ,e :Pos.beat ,d)
    `(rref ,e :Pos.beat)))
    
(defmacro p-unit (e &optional (d nil d?))
  (if d?
    `(rset ,e :Pos.unit ,d)
    `(rref ,e :Pos.unit)))


;;==============================
;; Record for state management
;;==============================

(defrecord PlayerState
  (date  :longint)
  (tempo :longint)
  (tsnum :short)
  (tsdenom :short)
  (tsclick :short)
  (tsquarter :short)
  (bar  :short)   
  (beat :short)     
  (unit  :short)
  (state  :short)
  (syncin  :short)
  (syncout  :short))

(defmacro s-bar (e )
   `(rref ,e :PlayerState.bar))

(defmacro s-beat (e)
  `(rref ,e :PlayerState.beat))
 
(defmacro s-unit (e)
  `(rref ,e :PlayerState.unit))

(defmacro s-date (e )
  `(rref ,e :PlayerState.date))

(defmacro s-tempo (e)
  `(rref ,e :PlayerState.tempo))

(defmacro s-num (e)
  `(rref ,e :PlayerState.tsnum))

(defmacro s-denom (e)
  `(rref ,e :PlayerState.tsdenom))

(defmacro s-click (e )
  `(rref ,e :PlayerState.tsclick))

(defmacro s-quarter (e )
  `(rref ,e :PlayerState.tsquarter))

(defmacro s-state (e )
  `(rref ,e :PlayerState.state))

(defmacro s-syncin (e )
  `(rref ,e :PlayerState.syncin))

(defmacro s-syncout (e )
  `(rref ,e :PlayerState.syncout))


;;======================
;; Record for MidiFile
;;======================

(defrecord MidiFileInfos
  (format  :longint)     
  (timedef  :longint)   
  (clicks :longint)      
  (tracks  :longint)  )   

(defmacro mf-format (e &optional (d nil d?))
  (if d?
    `(rset ,e :MidiFileInfos.format ,d)
    `(rref ,e :MidiFileInfos.format)))
 
(defmacro mf-timedef (e &optional (d nil d?))
  (if d?
    `(rset ,e :MidiFileInfos.timedef ,d)
    `(rref ,e :MidiFileInfos.timedef)))

(defmacro mf-clicks (e &optional (d nil d?))
  (if d?
    `(rset ,e :MidiFileInfos.clicks ,d)
    `(rref ,e :MidiFileInfos.clicks)))

(defmacro mf-tracks (e )
  `(rref ,e :MidiFileInfos.tracks))

;;=============================
;; Interface to C entry points
;;=============================

(defmacro Version ()
  `(ccl::ppc-ff-call (get-fun-addr "Version" *player-framework*) :signed-halfword))

(defmacro OpenPlayer (name)
  `(with-cstrs ((s ,name))
     (ccl::ppc-ff-call (get-fun-addr "OpenPlayer" *player-framework*) :address s :signed-halfword)))

(defmacro ClosePlayer (refnum)
  `(ccl::ppc-ff-call (get-fun-addr "ClosePlayer" *player-framework*) :signed-halfword ,refnum :void))

(defun open-player (name) (openplayer name))
  
;;===================
;; Transport control
;;===================

(defmacro StartPlayer (refnum)
  `(ccl::ppc-ff-call (get-fun-addr "StartPlayer" *player-framework*) :signed-halfword ,refnum :void))

(defmacro ContPlayer (refnum)
  `(ccl::ppc-ff-call (get-fun-addr "ContPlayer" *player-framework*) :signed-halfword ,refnum :void))

(defmacro StopPlayer (refnum)
  `(ccl::ppc-ff-call (get-fun-addr "StopPlayer" *player-framework*) :signed-halfword ,refnum :void))

(defmacro PausePlayer (refnum)
  `(ccl::ppc-ff-call (get-fun-addr "PausePlayer" *player-framework*) :signed-halfword ,refnum :void))

;;===================
;; Record management
;;===================

(defmacro SetRecordModePlayer (refnum state)
  `(ccl::ppc-ff-call (get-fun-addr "StartPlayer" *player-framework*) :signed-halfword ,refnum  :signed-halfword ,state :void))

(defmacro RecordPlayer (refnum tracknum)
  `(ccl::ppc-ff-call (get-fun-addr "RecordPlayer" *player-framework*) :signed-halfword ,refnum  :signed-halfword ,tracknum :void))

(defmacro SetRecordFilterPlayer (refnum filter)
  `(ccl::ppc-ff-call (get-fun-addr "StartPlayer" *player-framework*) :signed-halfword ,refnum  :address ,filter :void))

;;=====================
;; Position management
;;=====================

(defmacro SetPosBBUPlayer (refnum pos)
  `(ccl::ppc-ff-call (get-fun-addr "SetPosBBUPlayer" *player-framework*) :signed-halfword ,refnum :address ,pos :void))

(defmacro SetPosMsPlayer (refnum date_ms)
  `(ccl::ppc-ff-call (get-fun-addr "SetPosMsPlayer" *player-framework*) :signed-halfword ,refnum :signed-fullword ,date_ms :void))

;;==================
;; Loop management
;;==================

(defmacro SetLoopPlayer (refnum state)
  `(ccl::ppc-ff-call (get-fun-addr "SetLoopPlayer" *player-framework*) :signed-halfword ,refnum  :signed-halfword ,state :void))

(defmacro SetLoopStartBBUPlayer (refnum pos)
  `(ccl::ppc-ff-call (get-fun-addr "SetLoopStartBBUPlayer" *player-framework*) :signed-halfword ,refnum :address ,pos :void))

(defmacro SetLoopEndBBUPlayer (refnum pos)
  `(ccl::ppc-ff-call (get-fun-addr "SetLoopEndBBUPlayer" *player-framework*) :signed-halfword ,refnum :address ,pos :void))

(defmacro SetLoopStartMsPlayer (refnum date_ms)
  `(ccl::ppc-ff-call (get-fun-addr "SetLoopStartMsPlayer" *player-framework*) :signed-halfword ,refnum :signed-fullword ,date_ms :void))

(defmacro SetLoopEndMsPlayer (refnum date_ms)
  `(ccl::ppc-ff-call (get-fun-addr "SetLoopEndMsPlayer" *player-framework*) :signed-halfword ,refnum :signed-fullword ,date_ms :void))

;;============================
;; Synchronisation management
;;============================

(defmacro SetSynchroInPlayer (refnum state)
  `(ccl::ppc-ff-call (get-fun-addr "SetSynchroInPlayer" *player-framework*) :signed-halfword ,refnum  :signed-halfword ,state :void))

(defmacro SetSynchroOutPlayer (refnum state)
  `(ccl::ppc-ff-call (get-fun-addr "SetSynchroOutPlayer" *player-framework*) :signed-halfword ,refnum  :signed-halfword ,state :void))

(defmacro SetSMPTEOffsetPlayer (refnum smptepos)
  `(ccl::ppc-ff-call (get-fun-addr "SetSMPTEOffsetPlayer" *player-framework*) :signed-halfword ,refnum  :address ,smptepos :void))

(defmacro SetTempoPlayer (refnum tempo)
  `(ccl::ppc-ff-call (get-fun-addr "SetTempoPlayer" *player-framework*) :signed-halfword ,refnum  :signed-fullword ,tempo :void))

;;===================
;; State management
;;===================

(defmacro GetStatePlayer (refnum playerstate)
  `(ccl::ppc-ff-call (get-fun-addr "GetStatePlayer" *player-framework*) :signed-halfword ,refnum  :address ,playerstate :void))

(defmacro GetEndScorePlayer (refnum playerstate)
  `(ccl::ppc-ff-call (get-fun-addr "GetEndScorePlayer" *player-framework*) :signed-halfword ,refnum  :address ,playerstate :void))

;;==============
;; Step playing 
;;==============

(defmacro ForwardStepPlayer (refnum flag)
  `(ccl::ppc-ff-call (get-fun-addr "ForwardStepPlayer" *player-framework*) :signed-halfword ,refnum  :signed-halfword ,flag :void))

(defmacro BackwardStepPlayer (refnum flag)
  `(ccl::ppc-ff-call (get-fun-addr "BackwardStepPlayer" *player-framework*) :signed-halfword ,refnum  :signed-halfword ,flag :void))

;;====================
;; Tracks management
;;====================

(defmacro GetAllTrackPlayer (refnum)
  `(ccl::ppc-ff-call (get-fun-addr "GetAllTrackPlayer" *player-framework*) :signed-halfword ,refnum  :address))

(defmacro GetTrackPlayer (refnum tracknum)
  `(ccl::ppc-ff-call (get-fun-addr "GetTrackPlayer" *player-framework*) :signed-halfword ,refnum :signed-halfword ,tracknum  :address))

(defmacro SetTrackPlayer (refnum tracknum seq)
  `(ccl::ppc-ff-call (get-fun-addr "SetTrackPlayer" *player-framework*) :signed-halfword ,refnum :signed-halfword ,tracknum  :address ,seq :signed-fullword))

(defmacro SetAllTrackPlayer (refnum seq tpq)
  `(ccl::ppc-ff-call (get-fun-addr "SetAllTrackPlayer" *player-framework*) :signed-halfword ,refnum :address ,seq :address ,seq :signed-fullword, tpq :signed-fullword))

(defmacro SetParamPlayer (refnum track param val)
  `(ccl::ppc-ff-call (get-fun-addr "SetParamPlayer" *player-framework*) :signed-halfword ,refnum :signed-halfword ,track  
                     :signed-halfword ,param  :signed-halfword ,val  :void))

(defmacro InsertAllTrackPlayer (refnum seq)
  `(ccl::ppc-ff-call (get-fun-addr "InsertAllTrackPlayer" *player-framework*) :signed-halfword ,refnum :address ,seq  :signed-fullword ))

(defmacro InsertTrackPlayer (refnum track seq)
  `(ccl::ppc-ff-call (get-fun-addr "InsertTrackPlayer" *player-framework*) :signed-halfword ,refnum :signed-halfword ,track  :address ,seq  :signed-fullword ))

;;====================
;; Midifile management
;;====================

(defmacro MidiFileSave (name seq info)
  `(with-cstrs ((s ,name))
     (ccl::ppc-ff-call (get-fun-addr "MidiFileSave" *player-framework*) :address s :address ,seq :address ,info :signed-fullword)))

(defmacro MidiFileLoad (name seq info)
  `(with-cstrs ((s ,name))
     (ccl::ppc-ff-call (get-fun-addr "MidiFileLoad" *player-framework*) :address s :address ,seq :address ,info :signed-fullword)))

(defun midi-file-save (name seq info) (MidiFileSave  name seq info))
(defun midi-file-load (name seq info) (MidiFileLoad  name seq info))
 

