;;; Midishare ACL binding
;;; ----------------------------------------------------------------------
;;; Saved for the last time with emacs on (and by): 
;;;     Time-stamp: <2004-04-21 10:59:14 Saugier>

(eval-when (:compile-toplevel :load-toplevel :execute)
  (load "mshare32.dll")
  )

(in-package :cl-user)

(cl:defpackage "MIDISHARE"
  (:nicknames "MS")
  (:use common-lisp ff)
  (:export "TYPENOTE" "TYPEKEYON" "TYPEKEYOFF" "TYPEKEYPRESS" 
          "TYPECTRLCHANGE" "TYPEPROGCHANGE" "TYPECHANPRESS" "TYPEPITCHWHEEL" 
          "TYPEPITCHBEND" "TYPESONGPOS" "TYPESONGSEL" "TYPECLOCK" "TYPESTART" 
          "TYPECONTINUE" "TYPESTOP" "TYPETUNE" "TYPEACTIVESENS" "TYPERESET" "TYPESYSEX"
          "TYPESTREAM" "TYPEPRIVATE" "TYPEPROCESS" "TYPEDPROCESS" 
          "TYPEQFRAME" "TYPECTRL14B" "TYPENONREGPARAM" "TYPEREGPARAM"
          "TYPESEQNUM" "TYPETEXTUAL" "TYPECOPYRIGHT" "TYPESEQNAME"
          "TYPEINSTRNAME" "TYPELYRIC" "TYPEMARKER" "TYPECUEPOINT"
          "TYPECHANPREFIX" "TYPEENDTRACK" "TYPETEMPO" "TYPESMPTEOFFSET"
          "TYPETIMESIGN" "TYPEKEYSIGN" "TYPESPECIFIC" "TYPEPORTPREFIX"
          "TYPERCVALARM" "TYPEAPPLALARM" "TYPERESERVED" "TYPEDEAD"
          "MIDIERRSPACE" "MIDIERRREFNUM" "MIDIERRBADTYPE" "MIDIERRINDEX"
          "MODEMPORT" "PRINTERPORT" "MIDIEXTERNALSYNC" "MIDISYNCANYPORT"
          "SMPTE24FR" "SMPTE25FR" "SMPTE29FR" "SMPTE30FR"
          "MIDIOPENAPPL" "MIDICLOSEAPPL" "MIDICHGNAME" "MIDICHGCONNECT"
          "MIDIOPENMODEM" "MIDICLOSEMODEM" "MIDIOPENPRINTER"
          "MIDICLOSEPRINTER" "MIDISYNCSTART" "MIDISYNCSTOP" "MIDICHANGESYNC"
          "MIDIOPENDRIVER" "MIDICLOSEDRIVER" "MIDIADDSLOT" "MIDIREMOVESLOT"
          "MIDICHGSLOTCONNECT" "MIDICHGSLOTNAME"
          "INSTALL-MIDISHARE-INTERFACE" "REMOVE-MIDISHARE-INTERFACE"
          "LINK" "DATE" "EVTYPE" "REF" "PORT" "CHAN" "FIELD" "FIELDSLIST" "PITCH" "VEL" "DUR"
          "LINKSE" "LINKST" "KPRESS" "CTRL" "PARAM" "NUM" "PREFIX" "TEMPO" "SECONDS"
          "SUBFRAMES" "VAL" "PGM" "BEND" "CLK" "SONG" "FIELDS" "TEXT" "FMSG" "FCOUNT"
          "TSNUM" "TSDENOM" "TSCLICK" "TSQUARTER" "ALTERATION" "MINOR-SCALE" "INFO"
          "FIRSTEV" "LASTEV" "MIDISHARE"
          "MIDIGETVERSION" "MIDICOUNTAPPLS" "MIDIGETINDAPPL"
          "MIDIGETNAMEDAPPL" "MIDIOPEN" "MIDICLOSE"
          "MIDIGETNAME" "MIDISETNAME" "MIDIGETINFO" "MIDISETINFO" "MIDINEWFILTER"
          "MIDIFREEFILTER" "MIDIACCEPTCHAN" "MIDIACCEPTTYPE" "MIDIACCEPTPORT"
          "MIDIISACCEPTEDCHAN" "MIDIISACCEPTEDTYPE" "MIDIISACCEPTEDPORT"
          "MIDIGETFILTER" "MIDISETFILTER" "MIDIGETRCVALARM" "MIDISETRCVALARM"
          "MIDIGETAPPLALARM" "MIDISETAPPLALARM"
          "MIDICONNECT" "MIDIISCONNECTED" "MIDIGETPORTSTATE"
          "MIDISETPORTSTATE" "MIDIFREESPACE" "MIDINEWEV" "MIDICOPYEV" "MIDIFREEEV" "MIDISETFIELD"
          "MIDIGETFIELD" "MIDIADDFIELD" "MIDICOUNTFIELDS"
          "MIDINEWSEQ" "MIDIADDSEQ" "MIDIFREESEQ" "MIDICLEARSEQ" "MIDIAPPLYSEQ" "MIDIGETTIME"
          "MIDISENDIM" "MIDISEND" "MIDISENDAT"
          "MIDICOUNTEVS" "MIDIGETEV" "MIDIAVAILEV" "MIDIFLUSHEVS"
          "MIDIREADSYNC" "MIDIWRITESYNC"
          "MIDICALL" "MIDITASK" "MIDIDTASK" "MIDIFORGETTASKHDL" "MIDIFORGETTASK"
          "MIDICOUNTDTASKS" "MIDIFLUSHDTASKS" "MIDIEXEC1DTASK"
          "MIDINEWCELL" "MIDIFREECELL" "MIDITOTALSPACE" "MIDIGROWSPACE"
          "MIDIGETSYNCINFO" "MIDISETSYNCMODE" "MIDIGETEXTTIME"
          "MIDIINT2EXTTIME" "MIDIEXT2INTTIME" "MIDITIME2SMPTE"
          "MIDISMPTE2TIME" "MIDICOUNTDRIVERS" "MIDIGETINDDRIVER" "MIDIGETDRIVERINFOS"
          "MIDIGETINDSLOT" "MIDIGETSLOTINFOS" "MIDICONNECTSLOT"
          "MIDIISSLOTCONNECTED"
          "NULLPTRP" "NULLPTR" "LOAD-FRAMEWORK-BUNDLE" "GET-FUN-ADDR"
          "ADD-STARTUP-ACTION" "ADD-QUIT-ACTION"
           ))

(in-package midishare)

;;;-----------------------------------------------------------------------
;;;
;;; 			MidiShare Constant Definitions
;;;
;;;-----------------------------------------------------------------------

;;; Constant definitions for every type of MidiShare event

(defconstant typeNote          0 "note with pitch, velocity and duration")
(defconstant typeKeyOn         1 "key on with pitch and velocity")
(defconstant typeKeyOff        2 "key off with pitch and velocity")
(defconstant typeKeyPress      3 "key pressure with pitch and pressure value")
(defconstant typeCtrlChange    4 "control change with control number and control value")
(defconstant typeProgChange    5 "program change with program number")
(defconstant typeChanPress     6 "channel pressure with pressure value")
(defconstant typePitchWheel    7 "pitch bend with lsb and msb of the 14-bit value")
(defconstant typePitchBend     7 "pitch bender with lsb and msb of the 14-bit value")
(defconstant typeSongPos       8 "song position with lsb and msb of the 14-bit position")
(defconstant typeSongSel       9 "song selection with a song number")
(defconstant typeClock        10 "clock request (no argument)")
(defconstant typeStart        11 "start request (no argument)")
(defconstant typeContinue     12 "continue request (no argument)")
(defconstant typeStop         13 "stop request (no argument)")
(defconstant typeTune         14 "tune request (no argument)")
(defconstant typeActiveSens   15 "active sensing code (no argument)")
(defconstant typeReset        16 "reset request (no argument)")
(defconstant typeSysEx        17 "system exclusive with any number of data bytes. Leading $F0 and tailing $F7 are automatically supplied by MidiShare and MUST NOT be included by the user")
(defconstant typeStream       18 "special event with any number of unprocessed data/status bytes")
(defconstant typePrivate      19 "private event for internal use with 4 32-bits arguments")
(defconstant typeProcess     128 "interrupt level task with a function adress and 3 32-bits args")
(defconstant typeDProcess    129 "foreground task with a function address and 3 32-bits arguments")
(defconstant typeQFrame      130 "quarter frame message with a type from 0 to 7 and a value")

(defconstant typeCtrl14b     131)
(defconstant typeNonRegParam 132)
(defconstant typeRegParam    133)

(defconstant typeSeqNum	     134)
(defconstant typeTextual     135)
(defconstant typeCopyright   136)
(defconstant typeSeqName     137)
(defconstant typeInstrName   138)
(defconstant typeLyric	     139)
(defconstant typeMarker	     140)
(defconstant typeCuePoint    141)
(defconstant typeChanPrefix  142)
(defconstant typeEndTrack    143)
(defconstant typeTempo	     144)
(defconstant typeSMPTEOffset 145)

(defconstant typeTimeSign    146)
(defconstant typeKeySign     147)
(defconstant typeSpecific    148)
(defconstant typePortPrefix  149)

(defconstant typeRcvAlarm    150)
(defconstant typeApplAlarm   151)

(defconstant typeReserved    152 "events reserved for futur use")
(defconstant typedead        255 "dead task. Used by MidiShare to forget and inactivate typeProcess and typeDProcess tasks")

;;; Constant definition for every MidiShare error code

(defconstant MIDIerrSpace   -1 "too many applications")
(defconstant MIDIerrRefNu   -2 "bad reference number")
(defconstant MIDIerrBadType -3 "bad event type")
(defconstant MIDIerrIndex   -4 "bad index")

;;; Constant definition for the Macintosh serial ports

(defconstant ModemPort   0 "Macintosh modem port")
(defconstant PrinterPort 1 "Macintosh printer port")

;;; Constant definition for the synchronisation modes

(defconstant MidiExternalSync #x8000 
  "Bit-15 set for external synchronisation")
(defconstant MidiSyncAnyPort  #x4000
  "Bit-14 set for synchronisation on any port")

;;; Constant definition for SMPTE frame format

(defconstant smpte24fr 0 "24 frame/sec")
(defconstant smpte25fr 1 "25 frame/sec")
(defconstant smpte29fr 2 "29 frame/sec (30 drop frame)")
(defconstant smpte30fr 3 "30 frame/sec")

;;; Constant definition for MidiShare world changes

(defconstant MIDIOpenAppl     1 "application was opened")
(defconstant MIDICloseAppl    2 "application was closed")
(defconstant MIDIChgName      3 "application name was changed")
(defconstant MIDIChgConnect   4 "connection was changed")
(defconstant MIDIOpenModem    5 "Modem port was opened") ; obsolete
(defconstant MIDICloseModem   6 "Modem port was closed") ; obsolete
(defconstant MIDIOpenPrinter  7 "Printer port was opened")
(defconstant MIDIClosePrinter 8 "Printer port was closed")
(defconstant MIDISyncStart    9 "SMPTE synchronisation just start")
(defconstant MIDISyncStop    10 "SMPTE synchronisation just stop")
(defconstant MIDIChangeSync     11)
(defconstant MIDIOpenDriver     12)
(defconstant MIDICloseDriver    13)
(defconstant MIDIAddSlot        14)
(defconstant MIDIRemoveSlot     15)
(defconstant MIDIChgSlotConnect 16)
(defconstant MIDIChgSlotName    17)


;;;-----------------------------------------------------------------------
;;;
;;; 			     MidiShare Types
;;;
;;;-----------------------------------------------------------------------

(def-foreign-type TMidiSEX
    (:struct
     (link (* TMidiSEX))
     (data (:array :unsigned-char 12))
     ))
(def-foreign-type MidiSEXPtr (* TMidiSEX))

(def-foreign-type TMidiST 
    (:struct
     (val (:array :long 4))
     ))
(def-foreign-type MidiSTPtr (* TMidiST))

(def-foreign-type TMidiEv
    (:struct
     (link (* TMidiEv))
     (date :unsigned-long)
     (evtype :unsigned-char)
     (refnum :unsigned-char)
     (port :unsigned-char)
     (chan :unsigned-char)
     (info (:union
	    (note 
	     (:struct
	      (pitch :unsigned-char)
	      (vel :unsigned-char)
	      (dur :unsigned-short)
	      )
	     )
	    (timesign
	     (:struct
	      (numerator :unsigned-char)
	      (denominator :unsigned-char)
	      (nclocks :unsigned-char)
	      (n32nd :unsigned-char)
	      )
	     )
	    (keysign
	     (:struct
	      (ton :char)
	      (mode :unsigned-char)
	      (unused :short)
	      )
	     )
	    (param
	     (:struct
	      (num :short)
	      (val :short)
	      )
	     )
	    (seqnum
	     (:struct
	      (number :unsigned-short)
	      (unused :short)
	      )
	     )
	    (shortfields
	     (:array :short 2)
	     )
	    (longfield :long)
	    (tempo :long)
	    (data
	     (:array :unsigned-char 4)
	     )
	    (linkSE MidiSEXPtr)
	    (linkST MidiSTPtr)
	    )
	   )
     )
  )
(def-foreign-type MidiEvPtr (* TMidiEv))

(def-foreign-type TMidiSeq 
    (:struct
     (first MidiEvPtr)
     (last MidiEvPtr)
     (undef1 :unsigned-long)
     (undef2 :unsigned-long)
     )
  )
(def-foreign-type MidiSeqPtr (* TMidiSeq))

(def-foreign-type TFilter
    (:struct
     (port (:array :char 32))
     (evtype (:array :char 32))
     (channel (:array :char 2))
     (unused (:array :char 2))
     )
  )
(def-foreign-type MidiFilterPtr (* TFilter))

(def-foreign-type MidiName (* :char))
(def-foreign-type DriverName (:array :char 32))  
(def-foreign-type SlotName DriverName)  
(def-foreign-type SlotRefNum (:struct
			      (drvref :short)
			      (slotref :short)
			      ))

(defconstant MidiInputSlot 1)
(defconstant MidiOutputSlot 0)
(defconstant MidiInputOutputSlot 1)

(def-foreign-type SlotDirection :int)

(def-foreign-type TSlotInfos 
    (:struct
     (name SlotName)
     (direction SlotDirection)
     (cnx (:array :char 32))
     (reserved (:array :long 2))
     ))

(def-foreign-type TDriverInfos
    (:struct
     (name DriverName)
     (version :short)
     (slots :short)
     (reserved (:array :long 2))
     ))

(def-foreign-type TSyncInfo
    (:struct
     (time :long)
     (reenter :long)
     (syncmode :unsigned-short)
     (synclocked :unsigned-char)
     (syncport :unsigned-char)
     (syncstart :long)
     (syncstop :long)
     (syncoffset :long)
     (syncspeed :long)
     (syncbreaks :long)
     (syncformat :short)
     ))

(def-foreign-type SyncInfoPtr (* TSyncInfo))

(def-foreign-type TSmpteLocation
    (:struct
     (format :short)
     (hours :short)
     (minutes :short)
     (seconds :short)
     (frames :short)
     (fracs :short)
     ))

(def-foreign-type SmpteLocPtr (* TSmpteLocation))

;; Function pointer types

(def-foreign-type TaskPtr :foreign-address)
(def-foreign-type RcvAlarmPtr :foreign-address)
(def-foreign-type ApplAlarmPtr :foreign-address)
(def-foreign-type ApplyProcPtr :foreign-address)

;;;-----------------------------------------------------------------------
;;;
;;;      Macros for accessing MidiShare Events data structures
;;;
;;;-----------------------------------------------------------------------

;;;-----------------------------------------------------------------------
;;; Macros common to every type of event

(defun link (e &optional (d nil d?))
  "read or set the link of an event"
  (if d?
      (setf (ff::fslot-value-typed 'TMidiEv :c e 'link) d)
    (ff::fslot-value-typed 'TMidiEv :c e 'link)
    )
  )

(defun date (e &optional (d nil d?))
  "read or set the date of an event"
  (if d?
      (setf (ff::fslot-value-typed 'TMidiEv :c e 'date) d)
    (ff::fslot-value-typed 'TMidiEv :c e 'date)
    )
  )

(defun evtype (e &optional (d nil d?))
  "read or set the type of an event. Be careful in modifying the type of an event"
  (if d?
      (setf (ff::fslot-value-typed 'TMidiEv :c e 'evType) d)
    (ff::fslot-value-typed 'TMidiEv :c e 'evType)
    )
  )

(defun ref (e &optional (d nil d?))
  "read or set the reference number of an event"
  (if d?
      (setf (ff::fslot-value-typed 'TMidiEv :c e 'refnum) d)
    (ff::fslot-value-typed 'TMidiEv :c e 'refnum)
    )
  )

(defun port (e &optional (d nil d?))
  "read or set the port number of an event"
  (if d?
      (setf (ff::fslot-value-typed 'TMidiEv :c e 'port) d)
    (ff::fslot-value-typed 'TMidiEv :c e 'port)
    )
  )

(defun chan (e &optional (d nil d?))
  "read or set the chan number of an event"
  (if d?
      (setf (ff::fslot-value-typed 'TMidiEv :c e 'chan) d)
    (ff::fslot-value-typed 'TMidiEv :c e 'chan)
    )
  )

(defun field (e &optional f v)
  "give the number of fields or read or set a particular field of an event"
  (if f
      (if v
	  (midiSetField e f v)
	(midiGetField e f))
    (midiCountFields e)))
  
(defun fieldsList (e &optional (n 4))
  "collect all the fields of an event into a list"
  (let (l)
    (dotimes (i (min n (midiCountFields e)))
      (push (midiGetField e i) l))
    (nreverse l)))

;;;-----------------------------------------------------------------------
;;; Specific to typeNote events

(defun pitch (e &optional (d nil d?))
  "read or set the pitch of an event"
  (if d?
      (setf (ff::fslot-value-typed 'TMidiEv :c e 'info 'note 'pitch) d)
    (ff::fslot-value-typed 'TMidiEv :c e 'info 'note 'pitch)
    )
  )

(defun vel (e &optional (d nil d?))
  "read or set the velocity of an event"
  (if d?
      (setf (ff::fslot-value-typed 'TMidiEv :c e 'info 'note 'vel) d)
    (ff::fslot-value-typed 'TMidiEv :c e 'info 'note 'vel)
    )
  )

(defun dur (e &optional (d nil d?))
  "read or set the duration of an event"
  (if d?
      (setf (ff::fslot-value-typed 'TMidiEv :c e 'info 'note 'dur) d)
    (ff::fslot-value-typed 'TMidiEv :c e 'info 'note 'dur)
    )
  )

;;;-----------------------------------------------------------------------
;;; For other types of events

(defun linkSE (e &optional (d nil d?))
  "read or set the link of an SEXevent"
  (if d?
      (setf (ff::fslot-value-typed 'TMidiEv :c e 'linkSE) d)
    (ff::fslot-value-typed 'TMidiEv :c e 'linkSE)
    )
  )

(defun linkST (e &optional (d nil d?))
  "read or set the link of an STevent"
  (if d?
      (setf (ff::fslot-value-typed 'TMidiEv :c e 'linkST) d)
    (ff::fslot-value-typed 'TMidiEv :c e 'linkST)
    )
  )

(defun kpress (e &optional (d nil d?))
  "read or set the link of an STevent"
  (if d?
      (setf (ff::fslot-value-typed 'TMidiEv :c e 'info 'note 'vel) d)
    (ff::fslot-value-typed 'TMidiEv :c e 'info 'note 'vel)
    )
  )

(defun ctrl (e &optional v)
  (if v
    (midisetfield e 0 v)
    (midigetfield e 0)))

(defun param (e &optional v)
  (if v
    (midisetfield e 0 v)
    (midigetfield e 0)))

(defun num (e &optional v)
  (if v
    (midisetfield e 0 v)
    (midigetfield e 0)))

(defun prefix (e &optional v)
  (if v
    (midisetfield e 0 v)
    (midigetfield e 0)))

(defun tempo (e &optional v)
  (if v
    (midisetfield e 0 v)
    (midigetfield e 0)))

(defun seconds (e &optional v)
  (if v
    (midisetfield e 0 v)
    (midigetfield e 0)))

(defun subframes (e &optional v)
  (if v
    (midisetfield e 1 v)
    (midigetfield e 1)))

(defun val (e &optional v)
  (if v
    (midisetfield e 1 v)
    (midigetfield e 1)))

(defun pgm (e &optional v)
  (if v
      (setf (ff::fslot-value-typed 'TMidiEv :c e 'pgm) d)
    (ff::fslot-value-typed 'TMidiEv :c e 'pgm) d)
)

(defun bend (e &optional v)
  "read or set the bend value of an event"
  (if v
      (multiple-value-bind (ms7b ls7b) (floor (+ v 8192) 128)
	(setf (ff::fslot-value-typed 'TMidiEv :c e 'pitch) ls7b)
	(setf (ff::fslot-value-typed 'TMidiEv :c e 'vel) ms7b)
	)
    (- (+ (ff::fslot-value-typed 'TMidiEv :c e 'pitch)
	  (* 128 (ff::fslot-value-typed 'TMidiEv :c e 'vel) ))
       8192)))

(defun clk (e &optional v)
  (if v
      (multiple-value-bind (ms7b ls7b) (floor (round (/ v 6)) 128)
	(setf (ff::fslot-value-typed 'TMidiEv :c e 'pitch) ls7b)
	(setf (ff::fslot-value-typed 'TMidiEv :c e 'vel) ms7b))
    (* 6 (+ (pitch e) (* 128 (vel e)))) ))

(defun song (e &optional (d nil d?))
  (if d?
      (setf (ff::fslot-value-typed 'TMidiEv :c e 'pitch) d)
    (ff::fslot-value-typed 'TMidiEv :c e 'pitch)
    )
  )

(defun fields (e &optional v)
  (if v
    (let ((e e)) (mapc #'(lambda (f) (MidiAddField e f)) v))
    (let (l (e e))
      (dotimes (i (MidiCountFields e)) (push (MidiGetField e i) l))
      (nreverse l)) ))

(defun text (e &optional s)
  (if s
    (fields e (map 'list #'char-code s))
    (map 'string #'character (fields e)) ))

(defun fmsg (e &optional v)
  (if v
    (setf (ff::fslot-value-typed 'TMidiEv :c e 'pitch) v)
    (ff::fslot-value-typed 'TMidiEv :c e 'pitch)))

(defun fcount (e &optional v)
  (if v
    (setf (ff::fslot-value-typed 'TMidiEv :c e 'vel) v)
    (ff::fslot-value-typed 'TMidiEv :c e 'vel)))

(defun tsnum (e &optional v)
  (if v
    (midisetfield e 0 v)
    (midigetfield e 0)))

(defun tsdenom (e &optional v)
  (if v
    (midisetfield e 1 v)
    (midigetfield e 1)))

(defun tsclick (e &optional v)
  (if v
    (midisetfield e 2 v)
    (midigetfield e 2)))

(defun tsquarter (e &optional v)
  (if v
    (midisetfield e 3 v)
    (midigetfield e 3)))

(defun alteration (e &optional v)
  (if v
    (midisetfield e 0 v)
    (midigetfield e 0)))

(defun minor-scale (e &optional v)
  (if v
    (midisetfield e 1 (if v 1 0))
    (= 1 (midigetfield e 1))))

(defun info (e &optional d)
  "read or set the info of an event"
  (if d
    (setf (ff::fslot-value-typed 'TMidiEv :c e 'info) d)
    (ff::fslot-value-typed 'TMidiEv :c e 'info)))

;;;-----------------------------------------------------------------------
;;; For event sequences

(defun firstEv (s &optional (e nil e?))
  "read or set the first event of a sequence"
  (if e?
    (setf (ff::fslot-value-typed 'TMidiSeq :c s 'first) e)
    (ff::fslot-value-typed 'TMidiSeq :c s 'first)))

(defun lastEv (s &optional (e nil e?))
  "read or set the last event of a sequence"
  (if e?
    (setf (ff::fslot-value-typed 'TMidiSeq :c s 'last) e)
    (ff::fslot-value-typed 'TMidiSeq :c s 'last)))


;;;-----------------------------------------------------------------------
;;;
;;; 		          Midishare API calls
;;;
;;;-----------------------------------------------------------------------


;; -- function: MidiGetVersion
(def-foreign-call
    (MidiGetVersion "MidiGetVersion") (:void)
  :returning :short)

;; -- function: MidiCountAppls
(def-foreign-call
    (MidiCountAppls "MidiCountAppls") (:void)
  :returning :short)

;; -- function: MidiGetIndAppl
(def-foreign-call
    (MidiGetIndAppl "MidiGetIndAppl") ((index :short))
  :returning :short)

;; -- function: MidiGetNamedAppl
(def-foreign-call
    (MidiGetNamedAppl "MidiGetNamedAppl") ((name MidiName))
  :returning :short)


;;;---------------------------------------------------------------------------


;; -- function: MidiGetSyncInfo
(def-foreign-call
    (MidiGetSyncInfo "MidiGetSyncInfo") ((syncInfo SyncInfoPtr))
  :returning :void)

;; -- function: MidiSetSyncMode
(def-foreign-call
    (MidiSetSyncMode "MidiSetSyncMode") ((mode :unsigned-short))
  :returning :void)

;; -- function: MidiGetExtTime
(def-foreign-call
    (MidiGetExtTime "MidiGetExtTime") (:void)
  :returning :long)

;; -- function: MidiInt2ExtTime
(def-foreign-call
    (MidiInt2ExtTime "MidiInt2ExtTime")
    ((xl :long))
  :returning :long)

;; -- function: MidiExt2IntTime
(def-foreign-call
    (MidiExt2IntTime "MidiExt2IntTime")
    ((xl :long))
  :returning :long)

;; -- function: MidiTime2Smpte
(def-foreign-call
    (MidiTime2Smpte "MidiTime2Smpte")
    ((time :long)
     (format :short)
     (loc SmpteLocPtr))
  :returning :void)

;; -- function: MidiSmpte2Time
(def-foreign-call
    (MidiSmpte2Time "MidiSmpte2Time")
    ((smpteLocation SmpteLocPtr))
  :returning :long)


;;;---------------------------------------------------------------------------


;; -- function: MidiOpen
(def-foreign-call
    (MidiOpen "MidiOpen")
    ((applName MidiName))
  :returning :short)

;; -- function: MidiClose
(def-foreign-call
    (MidiClose "MidiClose")
    ((refNum :short))
  :returning :void)


;;;--------------------------------------------------------------------------
;;; Application Configuration


;; -- function: MidiGetName
(def-foreign-call
    (MidiGetName "MidiGetName")
    ((refNum :short))
  :returning MidiName)

;; -- function: MidiSetName
(def-foreign-call
    (MidiSetName "MidiSetName")
    ((refNum :short)
     (applName MidiName))
  :returning :void)

;; -- function: MidiGetInfo
;; NOTE: Returns a 32 bits int instead of a void *
(def-foreign-call
    (MidiGetInfo "MidiGetInfo")
    ((refNum :short))
  :returning :unsigned-long)

;; -- function: MidiSetInfo
;; NOTE: Second arg is a 32 bit int instead of a void *
(def-foreign-call
    (MidiSetInfo "MidiSetInfo")
    ((refNum :short)
     (info :unsigned-long))
  :returning :void)

;; -- function: MidiGetRcvAlarm
(def-foreign-call
    (MidiGetRcvAlarm "MidiGetRcvAlarm")
    ((refnum :short)
     (alarm RcvAlarmPtr))
  :returning RcvAlarmPtr)

;; -- function: MidiSetRcvAlarm
(def-foreign-call
    (MidiSetRcvAlarm "MidiSetRcvAlarm")
    ((refnum :short)
     (alarm RcvAlarmPtr))
  :returning :void)

;; -- function: MidiGetApplAlarm
(def-foreign-call
    (MidiGetApplAlarm "MidiGetApplAlarm")
    ((refnum :short))
  :returning ApplAlarmPtr)

;; -- function: MidiSetApplAlarm
(def-foreign-call
    (MidiSetApplAlarm "MidiSetApplAlarm")
    ((refnum :short)
     (alarm ApplAlarmPtr))
  :returning :void)

;;;--------------------------------------------------------------------------
;;; Drivers Management

;; TODO: Lisp can't be used to implement a driver yet.
#|
(def-foreign-call
    (MidiRegisterDriver "MidiRegisterDriver")
    ((infos (* TDriverInfos))
    (op (* TDriverOperation)))
  :returning :short)

(def-foreign-call
    (MidiUnregisterDriver "MidiUnregisterDriver")
    ((refnum :short))
  :returning :void)
|#

;; However, Lisp can be used to get information about existing drivers
;; with the following functions:

;; -- function: MidiCountDrivers
(def-foreign-call
    (MidiCountDrivers "MidiCountDrivers")
    (:void)
    :returning :short)

;; -- function: MidiGetIndDriver
(def-foreign-call
    (MidiGetIndDriver "MidiGetIndDriver")
    ((index :short))
    :returning :short)

;; -- function: MidiGetDriverInfos
(def-foreign-call
    (MidiGetDriverInfos "MidiGetDriverInfos")
    ((refnum :short)
     (infos (* TDriverInfos)))
  :returning :unsigned-char)


;;;--------------------------------------------------------------------------
;;; Slots Management

;; Note that MidiAddSlot, MidiRemoveSlot and MidiSetSlotName are not
;; available, as Lisp can't be used to implement a driver.

;; -- function: MidiGetIndSlot
(def-foreign-call
    (MidiGetIndSlot "MidiGetIndSlot")
    ((refnum :short)
     (index :short))
  :returning SlotRefNum)

;; -- function: MidiGetSlotInfos
(def-foreign-call
    (MidiGetSlotInfos "MidiGetSlotInfos")
    ((slot SlotRefNum)
     (infos (* TSlotInfos)))
  :returning :unsigned-char)

;; -- function: MidiConnectSlot
(def-foreign-call
    (MidiConnectSlot "MidiConnectSlot")
    ((port :short)
     (slot SlotRefNum)
     (state :unsigned-char))
  :returning :void)

;; -- function: MidiIsSlotConnected
(def-foreign-call
    (MidiIsSlotConnected "MidiIsSlotConnected")
    ((port :short)
     (slot SlotRefNum))
  :returning :unsigned-char)

;;;--------------------------------------------------------------------------
;;; Filters configuration

;; -- function: MidiGetFilter
(def-foreign-call
    (MidiGetFilter "MidiGetFilter")
    ((refnum :short))
  :returning MidiFilterPtr)

;; -- function: MidiSetFilter
(def-foreign-call
    (MidiSetFilter "MidiSetFilter")
    ((refnum :short)
     (f MidiFilterPtr))
  :returning :void)

;; -- function: MidiNewFilter
(def-foreign-call
    (MidiNewFilter "MidiNewFilter")
    (:void)
  :returning MidiFilterPtr)

;; -- function: MidiFreeFilter
(def-foreign-call
    (MidiFreeFilter "MidiFreeFilter")
    ((f MidiFilterPtr))
  :returning :void)

;; -- function: MidiAcceptPort
(def-foreign-call
    (MidiAcceptPort "MidiAcceptPort")
    ((f MidiFilterPtr)
     (port :short)
     (b :unsigned-char))
  :returning :void)

;; -- function: MidiAcceptChan
(def-foreign-call
    (MidiAcceptChan "MidiAcceptChan")
    ((f MidiFilterPtr)
     (chan :short)
     (b :unsigned-char))
  :returning :void)

;; -- function: MidiAcceptType
(def-foreign-call
    (MidiAcceptType "MidiAcceptType")
    ((f MidiFilterPtr)
     (type :short)
     (b :unsigned-char))
  :returning :void)

;; -- function: MidiIsAcceptedPort
(def-foreign-call
    (MidiIsAcceptedPort "MidiIsAcceptedPort")
    ((f MidiFilterPtr)
     (port :short))
  :returning :unsigned-char)

;; -- function: MidiIsAcceptedChan
(def-foreign-call
    (MidiIsAcceptedChan "MidiIsAcceptedChan")
    ((f MidiFilterPtr)
     (chan :short))
  :returning :unsigned-char)

;; -- function: MidiIsAcceptedType
(def-foreign-call
    (MidiIsAcceptedType "MidiIsAcceptedType")
    ((f MidiFilterPtr)
     (type :short))
  :returning :unsigned-char)

;;;--------------------------------------------------------------------------
;;; Inter-Application Connections

;; -- function: MidiConnect
(def-foreign-call
    (MidiConnect "MidiConnect")
    ((src :short)
     (dest :short)
     (state :char boolean))
  :returning :void)

;; -- function: MidiIsConnected
(def-foreign-call
    (MidiIsConnected "MidiIsConnected")
    ((src :short)
     (dest :short))
  :returning (:unsigned-char fixnum))

;;;--------------------------------------------------------------------------
;;; Serial ports

;; -- function: MidiGetPortState
(def-foreign-call
    (MidiGetPortState "MidiGetPortState")
    ((port :short))
  :returning :unsigned-char)

;; -- function: MidiSetPortState
(def-foreign-call
    (MidiSetPortState "MidiSetPortState")
    ((port :short)
     (dest :short))
  :returning :unsigned-char)

;;;--------------------------------------------------------------------------
;;; Events and memory managing

;; -- function: MidiFreeSpace
(def-foreign-call
    (MidiFreeSpace "MidiFreeSpace")
    (:void)
  :returning :unsigned-long)

;; -- function: MidiTotalSpace
(def-foreign-call
    (MidiTotalSpace "MidiTotalSpace")
    (:void)
  :returning :unsigned-long)

;; -- function: MidiGrowSpace
(def-foreign-call
    (MidiGrowSpace "MidiGrowSpace")
    ((n :long))
  :returning :unsigned-long)

;; -- function: MidiNewCell
(def-foreign-call
    (MidiNewCell "MidiNewCell")
    (:void)
  :returning MidiEvPtr)

;; -- function: MidiFreeCell
(def-foreign-call
    (MidiFreeCell "MidiFreeCell")
    ((e MidiEvPtr))
  :returning :void)

;; -- function: MidiNewEv
(def-foreign-call
    (MidiNewEv "MidiNewEv")
    ((typenum :short))
  :returning MidiEvPtr)

;; -- function: MidiCopyEv
(def-foreign-call
    (MidiCopyEv "MidiCopyEv")
    ((e MidiEvPtr))
  :returning MidiEvPtr)

;; -- function: MidiFreeEv
(def-foreign-call
    (MidiFreeEv "MidiFreeEv")
    ((e MidiEvPtr))
  :returning :void)

;; -- function: MidiSetField
(def-foreign-call
    (MidiSetField "MidiSetField")
    ((e MidiEvPtr)
     (f :long)
     (v :long))
  :returning :void)

;; -- function: MidiGetField
(def-foreign-call
    (MidiGetField "MidiGetField")
    ((e MidiEvPtr)
     (f :long))
  :returning :long)

;; -- function: MidiAddField
(def-foreign-call
    (MidiAddField "MidiAddField")
    ((e MidiEvPtr)
     (v :long))
  :returning :void)

;; -- function: MidiCountFields
(def-foreign-call
    (MidiCountFields "MidiCountFields")
    ((e MidiEvPtr))
  :returning :long)

;; -- function: MidiGetPort
(def-foreign-call
    (MidiGetPort "MidiGetPort")
    ((e MidiEvPtr))
  :returning :short)

;; -- function: MidiSetPort
(def-foreign-call
    (MidiSetPort "MidiSetPort")
    ((e MidiEvPtr)
     :short)
  :returning :void)

;;;--------------------------------------------------------------------------
;;; Sequence managing

;; -- function: MidiNewSeq
(def-foreign-call
    (MidiNewSeq "MidiNewSeq")
    (:void)
  :returning MidiSeqPtr)

;; -- function: MidiAddSeq
(def-foreign-call
    (MidiAddSeq "MidiAddSeq")
    ((s MidiSeqPtr)
     (e MidiEvPtr))
  :returning :void)

;; -- function: MidiFreeSeq
(def-foreign-call
    (MidiFreeSeq "MidiFreeSeq")
    ((s MidiSeqPtr))
  :returning :void)

;; -- function: MidiClearSeq
(def-foreign-call
    (MidiClearSeq "MidiClearSeq")
    ((s MidiSeqPtr))
  :returning :void)

;; -- function: MidiApplySeq
;;
(def-foreign-call
    (MidiApplySeq "MidiApplySeq")
    ((s MidiSeqPtr)
     (proc ApplyProcPtr))
  :returning :void)

;;;--------------------------------------------------------------------------
;;; Time

;; -- function: MidiGetTime
(def-foreign-call
    (MidiGetTime "MidiGetTime")
    (:void)
  :returning :unsigned-long)

;;;--------------------------------------------------------------------------
;;; Sending

;; -- function: MidiSendIm
(def-foreign-call
    (MidiSendIm "MidiSendIm")
    ((refnum :short)
     (e :foreign-address))
  :returning :void)

;; -- function: MidiSend
(def-foreign-call
    (MidiSend "MidiSend")
    ((refnum :short)
     (e MidiEvPtr))
  :returning :void)

;; -- function: MidiSendAt
(def-foreign-call
    (MidiSendAt "MidiSendAt")
    ((refnum :short)
     (e MidiEvPtr)
     (d :unsigned-long))
  :returning :void)

;;;--------------------------------------------------------------------------
;;; Receiving

;; -- function: MidiCountEvs
(def-foreign-call
    (MidiCountEvs "MidiCountEvs")
    ((refnum :short))
  :returning :unsigned-long)

;; -- function: MidiGetEv
(def-foreign-call
    (MidiGetEv "MidiGetEv")
    ((refnum :short))
  :returning MidiEvPtr)

;; -- function: MidiAvailEv
(def-foreign-call
    (MidiAvailEv "MidiAvailEv")
    ((refnum :short))
  :returning MidiEvPtr)

;; -- function: MidiFlushEvs
(def-foreign-call
    (MidiFlushEvs "MidiFlushEvs")
    ((refnum :short))
  :returning :void)

;;;--------------------------------------------------------------------------
;;; Mail boxes

;; -- function: MidiReadSync
(def-foreign-call
    (MidiReadSync "MidiReadSync")
    ((adrmem :foreign-address))
  :returning :foreign-address)

;; -- function: MidiWriteSync
(def-foreign-call
    (MidiWriteSync "MidiWriteSync")
    ((adrmem :foreign-address)
     (val :unsigned-long))
  :returning :foreign-address)

;;;--------------------------------------------------------------------------
;;; Task Managing

;; -- function: MidiCall
;; WARNING: This function has no use for now.
(def-foreign-call
    (MidiCall "MidiCall")
    ((routine TaskPtr)
     (date :unsigned-long)
     (refnum :short)
     (a1 :long)
     (a2 :long)
     (a3 :long))
  :returning :void)

;; -- function: MidiTask
;; WARNING: This function has no use for now.
(def-foreign-call
    (MidiTask "MidiTask")
    ((routine TaskPtr)
     (date :unsigned-long)
     (refnum :short)
     (a1 :long)
     (a2 :long)
     (a3 :long))
  :returning MidiEvPtr)

;; -- function: MidiDTask
;; WARNING: This function has no use for now.
(def-foreign-call
    (MidiDTask "MidiDTask")
    ((routine TaskPtr)
     (date :unsigned-long)
     (refnum :short)
     (a1 :long)
     (a2 :long)
     (a3 :long))
  :returning MidiEvPtr)

;; -- function: MidiForgetTask
;; WARNING: This function has no use for now.
(def-foreign-call
    (MidiForgetTask "MidiForgetTask")
    ((e (* MidiEvPtr)))
  :returning :void)

;; -- function: MidiCountDTasks
(def-foreign-call
    (MidiCountDTasks "MidiCountDTasks")
    ((refnum :short))
  :returning :long)

;; -- function: MidiFlushDTasks
(def-foreign-call
    (MidiFlushDTasks "MidiFlushDTasks")
    ((refnum :short))
  :returning :void)

;; -- function: MidiExec1DTask
(def-foreign-call
    (MidiExec1DTask "MidiExec1DTask")
    ((refnum :short))
  :returning :void)

;;;--------------------------------------------------------------------------
;;; MidiShare

;; -- function: MidiShare
(def-foreign-call
    (MidiShare "MidiShare")
    (:void)
  :returning :short)
