

;; =====================================================================================
;; The Player Library is Copyright (c) Grame, Computer Music Research Laboratory 
;; 1996-2000, and is distributed as Open Source software under the Artistic License;
;; see the file "Artistic" that is included in the distribution for details.
;;
;; Grame : Computer Music Research Laboratory
;; Web : http://www.grame.fr/Research
;; E-mail : MidiShare@rd.grame.fr
;;
;; History:
;; 
;; 18-09-03 : Use the  MidiNewMidiFileInfos MidiNewPlayerState MidiNewPos generic functions.
;; 24-09-03 : Check the version of the library.
;; =====================================================================================

;;
;; You must load the file Player_Interface.lisp before using this tutorial
;;

;; <<Begin modification by [RS]
(if (not (find-package "MIDISHARE"))
    (progn
      (setf system:*load-search-list*
	(append '(:first "e:/saugier/local/code/midishare-bind/")
		system:*load-search-list*
		)
	)
      (load "MidiShare-Interface.lisp" :verbose t)
      )
  )

(load "MidiShare-Interface.lisp" :verbose t)

(load "Player-Interface.lisp" :verbose t)

(print (list-all-packages))

(defun print-package-symbols(pkg &optional (show-internal-syms nil))
  "Prints the list of symbols contained in the package named pkg-name."
  (let ((sym-list ()))
    (if show-internal-syms
	(do-symbols (x pkg) (push x sym-list))
      (do-external-symbols (x pkg) (push x sym-list))
      )
    (format t "List of symbols in package ~A:~%" (package-name pkg))
    (dolist (x (sort sym-list #'string<))
      (format t "~A~%" x))
    )
  )

(print-package-symbols "MIDISHARE")

;; >>End modification by [RS]



;; Use the MIDISHARE package to have acces to all MidiShare and Player librariry symbols

(use-package :ms)

;; You must check if MidiShare is installed before using the Player library

(midishare)

;; You can check the Player library version

(Version)


;;=======================
;; Allocate a new player
;;=======================

;; Open a new Player

(defvar *player*) 
(setq *player*  (open-player "PlayerLisp"))

;; Each player is associated with a MidiShare application
;; Open-player gives the refence number of the MidiShare
;; application. It means that some of the MidiShare functions
;; can be used for the Player like MidiConnect, MidiSetname....

;; Connect the Player to MidiShare (input and ouput)

(midiconnect 0  *player* -1) 
(midiconnect *player* 0  -1) 


;;===================
;; Load a MidiFile
;;===================

 ;; Allocate an empty MidiShare sequence

(defvar *seq* (midinewseq)) 


;; Allocate a MidiFile record

(defvar *info* (MidiNewMidiFileInfos))

;; Load a  Midifile (set YOUR pathname)

(midi-file-load "E:\\saugier\\local\\music\\midi\\canyon.mid" *seq* *info*)
(midi-file-load "/Volumes/Documents/MacOS9/MidiFileEx/allblues.mid" *seq* *info*)
(midi-file-load "Document1:Developpements:MidiFileEx:Labamba.mid" *seq* *info*)
(midi-file-load "HD1200:MidiFileEx:ChopinNocturne4.2.mid" *seq* *info*)
(midi-file-load "HD1200:MidiFileEx:BECAUSE.MID" *seq* *info*)


;; Set the loaded MidiShare sequence in the Player 
;;
;; (You can  set directly a MidiShare sequence  "built from scratch"  in a player 
;; Look at the Track management  example)


(mf-clicks *info*)

(print *player*)

(print *seq*)



(setalltrackplayer *player* *seq* (mf-clicks *info*))


;;=====================
;; Transport control
;;=====================

(startplayer *player*)

(pauseplayer *player*)

(contplayer *player*)

(stopplayer *player*)


;;===================
;; State management
;;===================

;; GETSTATEPLAYER  can be used during playback

(defvar *state*)
(setq *state* (MidiNewPlayerState))


;; Print the state record

(defun midi-string-date (d)
  (let (hh mn ss ms)
    (multiple-value-setq (ss ms) (floor d 1000))
    (multiple-value-setq (mn ss) (floor ss 60))
    (multiple-value-setq (hh mn) (floor mn 60))
    (format nil "~2,'0d:~2,'0d:~2,'0d.~3,'0d" hh mn ss ms)))


(defun print-state ()
  (format t "Player current state ~%~%")
  (getstateplayer *player* *state*)
  (format t  "Position BBU :   ~2d ~2d ~3d ~%" 
              (s-bar *state*) (s-beat *state*) (s-unit *state*) )
  (format t  "Position Ms  :    ~2d  ~%" 
              (midi-string-date(s-date *state*)))
  (format t  "Tempo :     ~2d  ~%" 
              (round(/ 60000000(s-tempo *state*))))
  (format t  "TimeSign :  ~2d ~2d ~2d ~2d  ~%" 
              (s-num *state*)(s-denom *state*)(s-click *state*)(s-quarter *state*))
  (format t  "State :   ~2d  ~%" 
              (s-state *state*))
  (format t  "SyncIn :  ~2d  ~%" 
              (s-syncin *state*))
  (format t  "SyncOut : ~2d  ~%" 
              (s-syncout *state*)))

(print-state)


;;======================
;; Position management
;;======================

;; SETPOSBBUPLAYER and SETPOSMSPLAYER can be used during playback


(defvar *pos* (MidiNewPos))

;; set the postion in bar,beat,unit

(defun set-pos-bbu-player (bar beat unit)
  (p-bar *pos* bar)
  (p-beat *pos* beat)
  (p-unit *pos* unit)
  (setposbbuplayer *player* *pos*))

(set-pos-bbu-player 5 1 1)


;; set the postion in min,sec,ms

(defun set-pos-ms-player (min sec ms)
   (setposmsplayer *player* (+ (* 60000 min) (* 1000 sec) ms)))

(set-pos-ms-player 1 2 1)


;;==========================
;; Step playing management
;;==========================

;; if used during playback FORWARDSTEPPLAYER and BACKWARDSTEPPLAYER first stop the player


;; you will have a Sequencer error when using 
;; forwardstepplayer at the end of the sequence
;; or backwardstepplayer at the beginning of the sequence.

(forwardstepplayer  *player*  kStepPlay)

(backwardstepplayer  *player*  kStepPlay)  

(defun forward-step(n)
  (dotimes (i n)
    (forwardstepplayer  *player*  kStepPlay) 
    (sleep 0.05)))

(forward-step 10)


;;=====================
;; Loop management
;;=====================

;; Loop management functions can be used during playback

;; you will have a Sequencer error when setting loopstart marker after loopend marker or the contrary

(defun set-loopend-bbu-player (bar beat unit)
  (p-bar *pos* bar)
  (p-beat *pos* beat)
  (p-unit *pos* unit)
  (setloopendbbuplayer *player* *pos*))

(set-loopend-bbu-player 3 1 1)

(defun set-loopstart-bbu-player (bar beat unit)
  (p-bar *pos* bar)
  (p-beat *pos* beat)
  (p-unit *pos* unit)
  (setloopstartbbuplayer *player* *pos*))

(set-loopstart-bbu-player 1 1 1)


;; enable the loop
(setloopplayer *player* kloopon)

(startplayer *player*)
(stopplayer *player*)


;; disable the loop
(setloopplayer *player* kloopoff)


;; You can also use SETLOOPSTARTMSPLAYER and SETLOOPENDMSPLAYER


;;=====================
;; Record management
;;=====================

;; Each player has 256 tracks, track number 0 is reserved for the Tempo Map

;; Set a track in record mode

(recordplayer *player* 1)


(startplayer  *player*)
(stopplayer *player*)

;; By default , the record mode is kmixmode, new Midi events are mixed on the 
;; recording track
 

;; Change the record mode so that the track is erased between punch-in/punch-out points
;; when recording 

(setrecordmodeplayer *player* kerasemode)


;;=====================
;; Tracks management
;;=====================

;; un applicateur de fonction sur une sequence MidiShare

(defun apply-seq (seq fun)
  (do ((ev (firstev seq) (link ev)))
      ((eq ev 0))
   (funcall fun ev)))


;; Fonctions to print a Midi event in textual format

(defun midi-string-ev (e &optional (f 4))
  (unless (eq e 0)
    (list 
    (format nil "~A ~3D/~4A ~A ~:a" 
                (midi-string-date (date e)) 
                (port e) (chan e) 
                (midi-string-type (evtype e)) 
                (midi-list-fields e f)))))

(defun midi-list-fields (e &optional (n 4))
  (if (or (and (>= (evtype e) typeTextual) (<= (evtype e) typeCuePoint)) (= (evtype e) typeSpecific))
    (text e)
    (let (l)
      (dotimes (i (min n (midicountfields e)))
        (push (midigetfield e i) l))
      (nreverse l))))


(defun midi-string-date (d)
  (let (hh mn ss ms)
    (multiple-value-setq (ss ms) (floor d 1000))
    (multiple-value-setq (mn ss) (floor ss 60))
    (multiple-value-setq (hh mn) (floor mn 60))
    (format nil "~2,'0d:~2,'0d:~2,'0d.~3,'0d" hh mn ss ms)))


(defun midi-string-type (n)
  "convert a MidiShare event type to its name, a string."
  (let* ((lName '((0 . "Note") (1 . "KeyOn") (2 . "KeyOff") (3 . "KeyPress")
                  (4 . "CtrlChange") (5 . "ProgChange") (6 . "ChanPress") (7 . "PitchBend")
                  (8 . "SongPos") (9 . "SongSel") (10 . "Clock") (11 . "Start")
                  (12 . "Cont") (13 . "Stop") (14 . "Tune") (15 . "ActiveSens")
                  (16 . "Reset") (17 . "SysEx") (18 . "Stream") 
                  (128 . "Process") (129 . "DProcess") (130 . "QFrame")
                  (131 . "Ctrl14b") (132 . "NonRegParam") (133 . "RegParam")
                  (134 . "SeqNum") (135 . "Text:") (136 . "Copyright:")
                  (137 . "SeqName:") (138 . "InstrName:") (139 . "Lyric:")
                  (140 . "Marker:") (141 . "CuePoint:") (142 . "ChanPrefix")
                  (143 . "EndTrack") (144 . "Tempo") (145 . "SMPTEOffset")
                  (146 . "TimeSign") (147 . "KeySign") (148 . "Specific:")
                  (255 . "Dead")))
         (l (assoc n lName)) )
    (cond (l 
           (cdr l))
          ((< n typeProcess)
           "Private")
          (t
           "Undefined") )))


;; Fonctions to print a MidiShare sequence in textual format

(defun print-seq (seq ) 
  (apply-seq seq #'(lambda (ev) (print (midi-string-ev ev)))))

;; Each track can be individually accessed, changed, muted...

;; Get a COPY of the track number 1

(setq *seq* (gettrackplayer *player* 1))

(defvar *seq1*)
(setq *seq1* (getalltrackplayer *player* ))

(print-seq *seq1*)


;; Warning : you must free the MidiShare sequence when they are no longer used

(midifreeseq *seq*)
(midifreeseq *seq1*)


;; The third parameter to the setalltrackplayer function is the tick-per-quarter info.
;  This value is usually read in the MidiFile header. 

;; The internal time unit of a player is a "tick" which is related to absolute time (in millisecond)
;; using tempo informations and tick-per-quarter value.

;; When opening a new empty player, the default tempo is set to 120 beat-per-minute. If you set 
;; the tick-per-quater value to 500, 1 tick will correspond exactly to 1 millisecond.
;; 

;; Replace all the tracks with a new MidiShare sequence
;; Tracks are mixed in a unique MidiShare sequence but are distinguish by the refnum of their Midi event


(defun add-track (seq tracknum pitch startdate)
  (dotimes (i 500)
    (let ((ev (midinewev typenote)))
      (unless (eq ev 0)
        (midisetfield ev 0 pitch)
        (midisetfield ev 1 80)
        (midisetfield ev 2 125)
        (date ev (+ startdate  (* i 500))) ;; <== date in "ticks"  
        (ref ev tracknum)  ;; <=== set the track number in the event refnum field 
        (midiaddseq seq ev)))))

(defun buid-multitrack-score ( tracklist)
  (let ((seq (midinewseq)))
    (mapc #'(lambda (tracknum) (add-track seq tracknum (+ tracknum 60) (* tracknum 100) )) tracklist) seq))


(setalltrackplayer *player*   (buid-multitrack-score  '(1 2 3)) 500)

(startplayer *player*)
(stopplayer *player*)


;; Set a new track in the player

(defun build-track ()
  (let ((seq (midinewseq)))
    (dotimes (i 50)
      (let ((ev (midinewev typenote)))
        (unless (eq ev 0)
          (midisetfield ev 0 72)
          (midisetfield ev 1 80)
          (midisetfield ev 2 125)
          (date ev (* i 1000))
          (midiaddseq seq ev))))
    seq))


(settrackplayer *player*  1 (build-track))

;; You can clear tracks by putting a new empty MidiShare sequence in the player

;; Clear the track 1

(settrackplayer *player*  1 (midinewseq))


;; Mute the track 1

(setparamplayer *player* 1  kMute kMuteOn)

(startplayer *player*)
(stopplayer *player*)


;; Unmute a the track 1

(setparamplayer *player* 1  kMute kMuteOff)

;; The track 1 plays solo

(setparamplayer *player* 1  kSolo kSoloOn)

(startplayer *player*)
(stopplayer *player*)
(contplayer *player*)

(setparamplayer *player* 1  kSolo kSoloOff)

;;==============================
;; Synchronisation  management
;;==============================

;; By default, the player is internally synchronized (it uses it's internal tempo map)


;;===========================
;; External synchronisation
;;===========================

;; Set the Player in External synchro mode

(setsynchroinplayer *player* kExternalSync)

(startplayer *player*)

(settempoplayer *player* 1000000) ;; Tempo is expressed in micro-sec/per/quarter
(settempoplayer *player* 500000)
(settempoplayer *player* 250000)
(settempoplayer *player* 100000)
(settempoplayer *player* 50000)

(stopplayer *player*)

(settempoplayer *player* 250000)


;;=============================
;; Midi Clock synchronisation
;;=============================

;; Set the Player in Clock synchro mode

(setsynchroinplayer *player* kClockSync)


;; Use a new MidiShare application to send synchronisation Midi messages

(defvar *synchro*)

(setq *synchro* (midiopen "synchro"))

(midiconnect *synchro* *player* -1)


;; Send a Midi start message
(defun start-clock-player ()
  (midisendim *synchro* (midinewev typestart)))


;; Send a Midi stop message
 
(defun stop-clock-player ()
  (midisendim *synchro* (midinewev typestop)))


;; "Play" a sequence of Midi clock messages
 
(defun send-synchro(n tempo)
  (let ((time (midigettime)))
    (dotimes (i n)
      (midisendat *synchro* (midinewev typeclock) (round(+ time (* i (/ 60000 (* tempo 24)))))))))

(start-clock-player )
(send-synchro 500 120)


(stop-clock-player)


(start-clock-player )
(send-synchro 500 240)


(stop-clock-player)

(midiclose *synchro*)


;; A player can send Midi synchro messages (start, stop, clock, continue...) to be used as a master

(setsynchrooutplayer *player* kClockSyncOut)


;;=======================
;; SMPTE synchronisation 
;;=======================

(defvar *syncinfo*)

(setq *syncinfo* (make-record :TSmpteLocation))  ;; <=== defined in the MidiShare.lisp file

(defun set-SMPTE-offset (hrs min sec frames frac)
  (rset *syncinfo* :TSmpteLocation.hours hrs)
  (rset *syncinfo* :TSmpteLocation.minutes min)
  (rset *syncinfo* :TSmpteLocation.seconds sec)
  (rset *syncinfo* :TSmpteLocation.frames frames)
  (rset *syncinfo* :TSmpteLocation.fracs frac)
  (SetSMPTEOffsetPlayer *player* *syncinfo*))


;; Set MidiShare in SMPTE synchro mode

 (MidiSetSyncMode (logior MidiExternalSync MidiSyncAnyPort))


;; Set the Player in SMPTE synchro mode

(setsynchroinplayer *player* kSMPTESync)


;; Set a new SMPTE Offset

(set-SMPTE-offset 0 1 30 0 0) 

;; Now the Player will start when the SMPTE date coming from 
;; an external source is reached.


;;=====================
;; MidiFile management 
;;=====================

(defvar *info*)
(setq *info* (MidiNewMidiFileInfos)) 


;;=====================
;; Load a  Midifile
;;=====================

(setq *seq* (midinewseq))

;; Fill a MidiShare sequence with the MidiFile

(midi-file-load "e:\\saugier\\local\\music\\midi\\canyon.mid" *seq* *info*)

;;===================
;; Save a MidiFile 
;;===================

(setq *seq* (getalltrackplayer *player*))

(mf-format *info* midifile1)
(mf-timedef *info* TicksPerQuarterNote)
(mf-clicks  *info* 500)


;; Warning : the tick-per-quater  (here 500) is either read
;; in the MidiFile header or set to 500 if you start your session
;; with a empty Player. 

(midi-file-save "e:\\saugier\\local\\music\\midi\\fromlisp.mid" *seq* *info*)


;;=========
;; Close 
;;=========
;;
;; Close the Player: the internal score is garbaged

(closeplayer *player*)


;;=====================
;; General remarks
;;=====================

;; - Several Player can be opened together.
;;
;; - Some functions return error codes. You should test them in your code. 
;; 
;; - Date structures allocated with make-record must be desallocated with dispose-record.  
