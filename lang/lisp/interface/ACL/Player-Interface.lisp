;;; MidiShare's Player library ACL binding
;;; ----------------------------------------------------------------------
;;; Saved for the last time with emacs on (and by): 
;;;     Time-stamp: <2004-04-16 15:14:45 Saugier>

(eval-when (:compile-toplevel :load-toplevel :execute)
  (load "Player32.dll"))

(in-package :midishare)

(export '(kIdle kPause kRecording kPlaying kWaiting kMaxTrack kMuteOn
          kMuteOff kSoloOn kSoloOff kMute kSolo kNoTrack kEraseMode
          kMixMode kLoopOn kLoopOff kStepPlay kStepMute kInternalSync
          kClockSync kSMPTESync kExternalSync kNoSyncOut kClockSyncOut
          midifile0 midifile1 midifile2 TicksPerQuarterNote Smpte24
          Smpte25 Smpte29 Smpte30 noErr ErrOpen ErrRead ErrWrite ErrVol
          ErrGetInfo ErrSetInfo ErrMidiFileFormat PLAYERnoErr
          PLAYERerrAppl PLAYERerrEvent PLAYERerrMemory PLAYERerrSequencer
          player-framework p-bar p-beat p-unit s-bar s-beat s-unit s-date
          s-tempo s-num s-denom s-click s-quarter s-state
          s-syncin s-syncout
          mf-format mf-timedef mf-clicks mf-tracks
          OpenPlayer ClosePlayer open-player
          StartPlayer ContPlayer StopPlayer PausePlayer
          SetRecordModePlayer RecordPlayer SetRecordFilterPlayer
          SetPosBBUPlayer SetPosMsPlayer 
          SetLoopPlayer SetLoopStartBBUPlayer
          SetLoopEndBBUPlayer SetLoopStartMsPlayer SetLoopEndMsPlayer
          SetSynchroInPlayer SetSynchroOutPlayer SetSMPTEOffsetPlayer
          SetTempoPlayer GetStatePlayer GetEndScorePlayer 
          ForwardStepPlayer BackwardStepPlayer GetAllTrackPlayer
          GetTrackPlayer SetTrackPlayer SetAllTrackPlayer SetParamPlayer
          InsertAllTrackPlayer InsertTrackPlayer MidiFileSave
          MidiFileLoad midi-file-save midi-file-load MidiNewMidiFileInfos
          MidiNewPlayerState MidiNewPos MidiNewTSmpteLocation Version)
        :midishare)

;;===============================
;; Date structures ond constants
;;===============================

;;-------------------------------------------------------------------------- 
;; Player state   
;;-------------------------------------------------------------------------- 

(defconstant kIdle       0)
(defconstant kPause  	 1)
(defconstant kRecording  2)
(defconstant kPlaying    3)
(defconstant kWaiting    4)


;;-------------------------------------------------------------------------- 
;; Tracks state   
;;-------------------------------------------------------------------------- 

(defconstant kMaxTrack	256)
(defconstant kMuteOn  1)
(defconstant kMuteOff 0)
(defconstant kSoloOn  1)
(defconstant kSoloOff 0)
(defconstant kMute  0)
(defconstant kSolo  1)

;;-------------------------------------------------------------------------- 
;; Recording management  
;;-------------------------------------------------------------------------- 

(defconstant kNoTrack		-1)
(defconstant kEraseMode  	1)
(defconstant kMixMode 		0)

;;-------------------------------------------------------------------------- 
;; Loop  management  
;;-------------------------------------------------------------------------- 

(defconstant kLoopOn  	0)
(defconstant kLoopOff 	1)

;;-------------------------------------------------------------------------- 
;; Step Playing  
;;-------------------------------------------------------------------------- 

(defconstant kStepPlay  1)
(defconstant kStepMute  0)

;;-------------------------------------------------------------------------- 
;; Synchronisation  
;;-------------------------------------------------------------------------- 

(defconstant kInternalSync	0)
(defconstant kClockSync  	1)
(defconstant kSMPTESync 	2)
(defconstant kExternalSync 	3)

(defconstant kNoSyncOut	0)
(defconstant kClockSyncOut 1)

;;-------------------------------------------------------------------------- 
;; MIDIfile  
;;-------------------------------------------------------------------------- 

(defconstant midifile0  0)
(defconstant midifile1  1)
(defconstant midifile2  2)
  
(defconstant TicksPerQuarterNote     0)
(defconstant Smpte24                 24)
(defconstant Smpte25                 25)
(defconstant Smpte29                 29)
(defconstant Smpte30                 30)  
  
;;-------------------------------------------------------------------------- 
;; Errors  :  for MidiFile
;;-------------------------------------------------------------------------- 
 
(defconstant noErr               0)   ;; no error
(defconstant ErrOpen             1)   ;; file open error       
(defconstant ErrRead             2)   ;; file read error       
(defconstant ErrWrite            3)   ;; file write error      
(defconstant ErrVol              4)   ;; Volume error          
(defconstant ErrGetInfo          5)   ;; GetFInfo error        
(defconstant ErrSetInfo          6)   ;; SetFInfo error        
(defconstant ErrMidiFileFormat   7)   ;; bad MidiFile format   

;;-------------------------------------------------------------------------- 
;; Errors  : for the player
;;-------------------------------------------------------------------------- 

(defconstant PLAYERnoErr           -1)  ;; No error            
(defconstant PLAYERerrAppl         -2)  ;; Unable to open MidiShare app  
(defconstant PLAYERerrEvent        -3)  ;; No more MidiShare Memory
(defconstant PLAYERerrMemory       -4)  ;; No more Mac Memory
(defconstant PLAYERerrSequencer    -5)  ;; Sequencer error

;;--------------------------------------------------------------------------
;; C types
;;-------------------------------------------------------------------------- 

(def-foreign-type PlayerState
    (:struct
     (date :long)
     (tempo :long)
     (tsnum :short)
     (tsdenom :short)
     (tsclick :short)
     (tsquarter :short)
     (bar :short)
     (beat :short)
     (unit :short)
     (state :short)
     (syncin :short)
     (syncout :short)
     (ticks :long)))

(def-foreign-type PlayerStatePtr
    (* PlayerState))

(defun MidiNewPlayerState ()
  (allocate-fobject 'PlayerState :foreign-static-gc))

(defun s-bar (e)
  (fslot-value-typed 'PlayerState :foreign-static-gc e 'bar))

(defun s-beat (e)
  (fslot-value-typed 'PlayerState :foreign-static-gc e 'beat))

(defun s-unit (e)
  (fslot-value-typed 'PlayerState :foreign-static-gc e 'unit))

(defun s-date (e)
  (fslot-value-typed 'PlayerState :foreign-static-gc e 'date))

(defun s-tempo (e)
  (fslot-value-typed 'PlayerState :foreign-static-gc e 'tempo))

(defun s-num (e)
  (fslot-value-typed 'PlayerState :foreign-static-gc e 'tsnum))

(defun s-denom (e)
  (fslot-value-typed 'PlayerState :foreign-static-gc e 'tsdenom))

(defun s-click (e)
  (fslot-value-typed 'PlayerState :foreign-static-gc e 'tsclick))

(defun s-quarter (e)
  (fslot-value-typed 'PlayerState :foreign-static-gc e 'tsquarter))

(defun s-state (e)
  (fslot-value-typed 'PlayerState :foreign-static-gc e 'state))

(defun s-syncin (e)
  (fslot-value-typed 'PlayerState :foreign-static-gc e 'syncin))

(defun s-syncout (e)
  (fslot-value-typed 'PlayerState :foreign-static-gc e 'syncout))

;--------------------------------------------------------------------------

(def-foreign-type Pos
    (:struct
     (bar :short)
     (beat :short)
     (unit :short)))

(def-foreign-type PosPtr
    (* Pos))

(defun MidiNewPos()
  (allocate-fobject 'Pos :foreign-static-gc))

(defun p-bar (e &optional (d nil d?))
  (if d?
      (setf (fslot-value-typed 'Pos :foreign-static-gc  e 'bar) d)
    (fslot-value-typed 'Pos :foreign-static-gc e 'bar)))

(defun p-beat (e &optional (d nil d?))
  (if d?
      (setf (fslot-value-typed 'Pos :foreign-static-gc e 'beat) d)
    (fslot-value-typed 'Pos :foreign-static-gc e 'beat)))

(defun p-unit (e &optional (d nil d?))
  (if d?
      (setf (fslot-value-typed 'Pos :foreign-static-gc e 'unit) d)
    (fslot-value-typed 'Pos :foreign-static-gc e 'unit)))

;--------------------------------------------------------------------------

(def-foreign-type MidiFileInfos
    (:struct
     (format :long)
     (timedef :long)
     (clicks :long)
     (tracks :long)))

(def-foreign-type MidiFileInfosPtr
    (* MidiFileInfos))

(defun MidiNewMidiFileInfos ()
  (allocate-fobject 'MidiFileInfos :foreign-static-gc))

(defun mf-format (e &optional (d nil d?))
  (if d?
      (setf (fslot-value-typed 'MidiFileInfos :foreign-static-gc e 'format) d)
    (fslot-value-typed 'MidiFileInfos :foreign-static-gc e 'format)))

(defun mf-timedef (e &optional (d nil d?))
  (if d?
      (setf (fslot-value-typed 'MidiFileInfos :foreign-static-gc e 'timedef) d)
    (fslot-value-typed 'MidiFileInfos :foreign-static-gc e 'timedef)))

(defun mf-clicks (e &optional (d nil d?))
  (if d?
      (setf (fslot-value-typed 'MidiFileInfos :foreign-static-gc e 'clicks) d)
    (fslot-value-typed 'MidiFileInfos :foreign-static-gc e 'clicks)))

(defun mf-tracks (e)
    (fslot-value-typed 'MidiFileInfos :foreign-static-gc e 'tracks))

;;--------------------------------------------------------------------------
;; C functions
;;--------------------------------------------------------------------------

;; Version

(def-foreign-call
    (Version "Version")
    (:void)
  :returning :short)

;; Opening/Closing

(def-foreign-call
    (OpenPlayer "OpenPlayer")
    ((name MidiName))
  :returning :short)

(defun open-player (name)
  (openplayer name))

(def-foreign-call
    (ClosePlayer "ClosePlayer")
    ((refnum :short))
  :returning :void)

;; Transport control

(def-foreign-call
    (StartPlayer "StartPlayer")
    ((refnum :short))
  :returning :void)

(def-foreign-call
    (ContPlayer "ContPlayer")
    ((refnum :short))
  :returning :void)

(def-foreign-call
    (StopPlayer "StopPlayer")
    ((refnum :short))
  :returning :void)

(def-foreign-call
    (PausePlayer "PausePlayer")
    ((refnum :short))
  :returning :void)

;; Record management

(def-foreign-call
    (SetRecordModePlayer "SetRecordModePlayer")
    ((refnum :short)
     (state :short))
  :returning :void)

(def-foreign-call
    (RecordPlayer "RecordPlayer")
    ((refnum :short)
     (tracknum :short))
  :returning :void)

(def-foreign-call
    (SetRecordFilterPlayer "SetRecordFilterPlayer")
    ((refnum :short)
     (tracknum :short))
  :returning :void)

;; Position management

(def-foreign-call
    (SetPosBBUPlayer "SetPosBBUPlayer")
    ((refnum :short)
     (pos PosPtr))
  :returning :void)

(def-foreign-call
    (SetPosMsPlayer "SetPosMsPlayer")
    ((refnum :short)
     (date_ms :long))
  :returning :void)

(def-foreign-call
    (SetPosTicksPlayer "SetPosTicksPlayer")
    ((refnum :short)
     (date_ticks :long))
  :returning :void)

;; Loop management

(def-foreign-call
    (SetLoopPlayer "SetLoopPlayer")
    ((refnum :short)
     (state :short))
  :returning :void)

(def-foreign-call
    (SetLoopStartBBUPlayer "SetLoopStartBBUPlayer")
    ((refnum :short)
     (pos PosPtr))
  :returning :long)

(def-foreign-call
    (SetLoopEndBBUPlayer "SetLoopEndBBUPlayer")
    ((refnum :short)
     (pos PosPtr))
  :returning :long)

(def-foreign-call
    (SetLoopStartMsPlayer "SetLoopStartMsPlayer")
    ((refnum :short)
     (date_ms :long))
  :returning :long)

(def-foreign-call
    (SetLoopEndMsPlayer "SetLoopEndMsPlayer")
    ((refnum :short)
     (date_ms :long))
  :returning :long)

;; Synchronisation management

(def-foreign-call
    (SetSynchroInPlayer "SetSynchroInPlayer")
    ((refnum :short)
     (state :short))
  :returning :void)

(def-foreign-call
    (SetSynchroOutPlayer "SetSynchroOutPlayer")
    ((refnum :short)
     (state :short))
  :returning :void)

(def-foreign-call
    (SetSMPTEOffsetPlayer "SetSMPTEOffsetPlayer")
    ((refnum :short)
     (smptepos SmpteLocPtr))
  :returning :void)

(def-foreign-call
    (SetTempoPlayer "SetTempoPlayer")
    ((refnum :short)
     (tempo :long))
  :returning :void)

;; State management

(def-foreign-call
    (GetStatePlayer "GetStatePlayer")
    ((refnum :short)
     (playerstate PlayerStatePtr))
  :returning :void)

(def-foreign-call
    (GetEndScorePlayer "GetEndScorePlayer")
    ((refnum :short)
     (playerstate PlayerStatePtr))
  :returning :void)

;; Step playing

(def-foreign-call
    (ForwardStepPlayer "ForwardStepPlayer")
    ((refnum :short)
     (state :short))
  :returning :void)

(def-foreign-call
    (BackwardStepPlayer "BackwardStepPlayer")
    ((refnum :short)
     (state :short))
  :returning :void)

;; Tracks management

(def-foreign-call
    (GetAllTrackPlayer "GetAllTrackPlayer")
    ((refnum :short))
  :returning MidiSeqPtr)

(def-foreign-call
    (GetTrackPlayer "GetTrackPlayer")
    ((refnum :short)
     (tracknum :short))
  :returning MidiSeqPtr)

(def-foreign-call
    (SetTrackPlayer "SetTrackPlayer")
    ((refnum :short)
     (tracknum :short)
     (s MidiSeqPtr))
  :returning :long)

(def-foreign-call
    (SetAllTrackPlayer "SetAllTrackPlayer")
    ((refnum :short)
     (s MidiSeqPtr)
     (ticks_per_quarter :long))
  :returning :long)

(def-foreign-call
    (SetParamPlayer "SetParamPlayer")
    ((refnum :short)
     (tracknum :short)
     (param :short)
     (value :short))
  :returning :void)

(def-foreign-call
    (GetParamPlayer "GetParamPlayer")
    ((refnum :short)
     (tracknum :short)
     (param :short))
  :returning :short)

(def-foreign-call
    (InsertAllTrackPlayer "InsertAllTrackPlayer")
    ((refnum :short)
     (s MidiSeqPtr))
  :returning :long)

(def-foreign-call
    (InsertTrackPlayer "InsertTrackPlayer")
    ((refnum :short)
     (tracknum :short)
     (s MidiSeqPtr))
  :returning :long)

;; Midifile management

(def-foreign-call
    (MidiFileSave "MidiFileSave")
    ((name (* :char))
     (seq MidiSeqPtr)
     (infos MidiFileInfosPtr))
  :returning :int)

(defun midi-file-save(name seq infos)
  (MidiFileSave name seq infos))

(def-foreign-call
    (MidiFileLoad "MidiFileLoad")
    ((name (* :char))
     (seq MidiSeqPtr)
     (infos MidiFileInfosPtr))
  :returning :int)

(defun midi-file-load(name seq infos)
  (MidiFileLoad name seq infos))
