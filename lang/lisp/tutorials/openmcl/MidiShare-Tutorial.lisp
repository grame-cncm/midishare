;;/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
;;===================================================================================
;;\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
;;
;; 			MIDISHARE MINI TUTORIAL
;;
;; 	 This file is a small guided tour for using MidiShare with MCL3.9 and later.
;;
;; INSTRUCTIONS :
;; If MidiShare is not installed, you need to install it (http://www.grame.fr/MidiShare/)
;;
;; Then you need to load the "MidiShare.lisp" file (Load cmd in the Eval menu). 
;; It contains the lisp interface to MidiShare.
;;
;; Now you are ready for the guided tour. Just follows the instructions for each
;; tests starting from test 1. Don't skip any test because some of them depend of 
;; previous ones.
;;
;; 28-10-03 Rename type macro to evtype.
;; 04-11-03 Add some explanation for port/slot connection management in test 14.
;; 29-01-04 Updated for new MidiShare function behaviour.

;;
;;/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
;;===================================================================================
;;\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/


;;===================================================================================
;; Use MidiShare package
;;===================================================================================

(use-package :ms)  ; <== EVALUATE THIS EXPRESSION.

;;===================================================================================
;; test 1 : Check if MidiShare is installed
;;===================================================================================
;; First of all, an application have to make sure that MidiShare is in memory. 
;; This checking is done thanks to the MidiShare function. The result is T if 
;; MidiShare is installed and NIL otherwise. Check the Listener window for the
;; result.




 (midishare)                                           	; <== EVALUATE THIS EXPRESSION.

;; Check the Listener window for the result.It must be 1. If it is not, you must 
;; install MidiShare an reboot.

;;===================================================================================
;; test 2 : current version of MidiShare
;;===================================================================================
;; MidiGetVersion gives the version number of MidiShare. The result must be
;; 168 or greater.

 (midigetversion)                                      	; <== EVALUATE THIS EXPRESSION.


;;===================================================================================
;; test 3 : how many client applications are running ?
;;===================================================================================
;; The MidiCountAppls gives the number of active Midi applications on. The result 
;; here will probably be 0. It means that no MidiShare client applications are running.

 (midicountappls)                                      	; <== EVALUATE THIS EXPRESSION.


;;===================================================================================
;; test 4 : register Common Lisp as a client application
;;===================================================================================
;; MidiOpen allows the recording of some information relative to the application 
;; context (its name, the value of the global data register, etc...), to allocate a 
;; reception FIFO and to attribute a unique reference number to the application. 
;; In counterpart to any MidiOpen call, the application must call the MidiClose 
;; function before leaving, by giving its reference number as an argument. 
;; MidiShare can thus be aware of the precise number of active Midi applications. 

 (defparameter *refnum* (midiopen "Common Lisp"))     	; <== EVALUATE THIS EXPRESSION.


;;===================================================================================
;; test 5 : how many client applications are running now?
;;===================================================================================
;; Now lets check again for the number of MidiShare client applications on activity.

 (midicountappls)                                     	; <== EVALUATE THIS EXPRESSION.

;; The result is probably 2. Actually when the first client opens, a pseudo application 
;; with name "MidiShare" and reference number 0 is also started. You can think of it as 
;; the Midi drivers of MidiShare. For this reason we have a count of 2 client 
;; applications instead of 1. This pseudo application "MidiShare" cannot be closed
;; directly. It is closed automatically when the very last client application closes. 


;;===================================================================================
;; test 6 : list MidiShare client applications
;;===================================================================================
;; We are now going to list all the client applications on activity.

(defun list-appls ()
  (format t "List of MidiShare client applications ~%")
  (dotimes (i (MidiCountAppls))
    (let ((ref (MidiGetIndAppl (1+ i))))
      (format t 
              " ~2d : reference number ~2d, name : '~a' ~%" 
              (1+ i) ref (MidiGetName ref)))))      	; <== EVALUATE THIS DEFINITION

;; NOTE : The MidiGetIndAppl function allows to know the reference number of any 
;; application by giving its order number (a number between 1 and MidiCountAppls).
;; The MidiGetName function gives the name of a client application from its
;; reference number.

 (list-appls)                                        	; <== EVALUATE THIS EXPRESSION.

;; In the Listener window the result will be :
;;	? List of MidiShare client applications 
;;	  1 : reference number  0, name : 'MidiShare' 
;;	  2 : reference number  1, name : 'Common Lisp' 
;;	NIL

;; NOTE : The reference number of the pseudo application "MidiShare" is always 0.

;;===================================================================================
;; test 7 : search for a client application with name "Common Lisp"
;;===================================================================================
;; Knowing an application name, it is possible to find its reference number using
;; the MidiGetNamedAppl function. If more than one client applications have the same
;; name the result is the reference number of the first one (the one with the smallest
;; reference number).

 (MidiGetNamedAppl "Common Lisp")			; <== EVALUATE THIS EXPRESSION.

;; The result here is 1, the same value stored in the *REFNUM* variable (see Test 4).

 *REFNUM*						; <== EVALUATE THIS EXPRESSION.

;;===================================================================================
;; test 8 : search for a client application with name "XYZ"
;;===================================================================================
;; If we look for a non existant name the result is -4. This means that no application
;; with that name was found. Negative reference numbers are used as error codes.

 (MidiGetNamedAppl "XYZ?!")				; <== EVALUATE THIS EXPRESSION.


;;===================================================================================
;; test 9 : change the name of client application "Common Lisp" to "MCL"
;;===================================================================================
;; MidiSetName allows to change the name of a client application.

 (MidiSetName (MidiGetNamedAppl "Common Lisp") "MCL")	; <== EVALUATE THIS EXPRESSION.

 (list-appls)						; <== EVALUATE THIS EXPRESSION.

;; In the Listener window the result will be :
;;	? List of MidiShare client applications 
;;	  1 : reference number  0, name : 'MidiShare' 
;;	  2 : reference number  1, name : 'MCL' 
;;	NIL

;;===================================================================================
;; test 10 : connect MCL to MidiShare
;;===================================================================================
;; For an application to be able to transmit and receive events, it must first connect
;; to some source and destination applications. The MidiConnect function is used to 
;; connect or disconnect a source and a destination. The function takes 3 arguments :
;; the reference number of the source, the reference number of the destination and a
;; state expressed as an integer value :
;; -1 for "true" to connect and 0 for "false" to disconnect.

 (MidiConnect *refnum* 0 -1)				; <== EVALUATE THIS EXPRESSION.

;; Now MCL will be able to send events to the pseudo application "MidiShare", i.e. the
;; Midi drivers.

;;===================================================================================
;; test 11 : test if MCL is connected to MidiShare
;;===================================================================================
;; We can test the connections using MidiIsConnected.

 (MidiIsConnected *refnum* 0)				; <== EVALUATE THIS EXPRESSION.

;; The result is 1 meaning that MCL sends to MidiShare


 (MidiIsConnected 0 *refnum*)				; <== EVALUATE THIS EXPRESSION.

;; The result is 0 meaning that MidiShare does not send to MCL


;;===================================================================================
;; test 12 : list the destinations of an application
;;===================================================================================
;; We can list all the destinations of an application by using MidiIsConnected for 
;; each possible destination

 (defun list-dest (ref1)
   (format t 
           "List of the destinations of '~a' (ref num = ~d) ~%" 
           (MidiGetName ref1) ref1)
   (dotimes (i (MidiCountAppls))
     (let ((ref2 (MidiGetIndAppl (1+ i))))
       (if (= (MidiIsConnected ref1 ref2) 1)
         (format t " --> '~a' (ref num = ~d) ~%"
                 (MidiGetName ref2)  
                 ref2)))))				; <== EVALUATE THIS DEFINITION

 (list-dest *refnum*)					; <== EVALUATE THIS EXPRESSION.

;; In the Listener window the result will be :
;;	? List of the destinations of 'MCL' (ref num = 1) 
;;	 --> 'MidiShare' (ref num = 0) 
;;	NIL


;;===================================================================================
;; test 13 : list the sources of an application
;;===================================================================================
;; We can list all the sources of an application by using MidiIsConnected for 
;; each possible source

 (defun list-src (ref1)
   (format t 
           "List of the sources of '~a' (ref num = ~d) ~%" 
           (MidiGetName ref1) ref1)
   (dotimes (i (MidiCountAppls))
     (let ((ref2 (MidiGetIndAppl (1+ i))))
       (if (= (MidiIsConnected ref2 ref1) 1)
         (format t " <-- '~a' (ref num = ~d) ~%"
                 (MidiGetName ref2)  
                 ref2)))))				; <== EVALUATE THIS DEFINITION

 (list-src 0)						; <== EVALUATE THIS EXPRESSION.

;; In the Listener window the result will be :
;;	? List of the sources of 'MidiShare' (ref num = 0) 
;; 	 <-- 'MCL' (ref num = 1) 
;;	NIL


;;===================================================================================
;; test 14 : send a note with a pitch, a velocity and a duration in milliseconds
;;===================================================================================
;; We are now ready to send a note event. Starting from version 1.80, drivers are external
;; ressources loaded dynamically by the MidiShare kernel. The "msDrivers" application must 
;; be used to define "logical" port to slot (device) connections. Setting the port field 
;; to different values allows to access different output devices. On input, events will have 
;; their port field set with the value corresponding to their slot (device) source.
;; Read the msDrivers documentation for more information. 
;; 

 (defun send-note (pitch)
   (let ((event (MidiNewEv typeNote)))	; ask for a new note event
     (unless (ccl:%null-ptr-p event)	; if the allocation was succesfull
       (chan event 0)			; set the midi channel to 0 (means channel 1)
       (port event 0)			; set the destination port to 0
       (field event 0 pitch)		; set the pitch field
       (field event 1 64)		; set the velocity field
       (field event 2 1000)		; set the duration field to 1 second
       (MidiSendIm *refnum* event))	; send the note immediatly
     ))							; <== EVALUATE THIS DEFINITION

 (send-note 60)						; <== EVALUATE THIS EXPRESSION.

;; The note was sent to your Midi equipment. Actually two messages where sent, a keyOn
;; and a keyOn with velocity 0 after the duration of the note.

;; IMPORTANT NOTE :
;; MidiShare includes its own memory manager to store events and sequences. MidiNewEv 
;; allocates the required memory to store the note event and returns a pointer. 
;; This space is automatically disposed by MidiShare when the event is sent. This means 
;; that once you have sent an event you MUST NOT use it anymore. In particular you 
;; MUST NOT send it a second time, free it or make a copy of it.

;;===================================================================================
;; test 15 : send multiple notes
;;===================================================================================
;; Here is an example of how to send several copies of an event

(defun send-multiple-notes (n pitch delay)
  (let ((event (MidiNewEv typeNote))	; ask for a new note event
        (date (MidiGetTime)))		; remember the current time
    (unless (ccl:%null-ptr-p event)	; if the allocation was succesful
      (chan event 0)			; set the midi channel to 0 (means channel 1)
      (port event 0)			; set the destination port to 0
      (field event 0 pitch)		; set the pitch field
      (field event 1 64)		; set the velocity field
      (field event 2 (- delay 1))	; set the duration field
      
      (dotimes (i n)			; loop for the requested number of events
        (MidiSendAt *refnum* 		; send a copy of the original note
                    (MidiCopyEv event)	 
                    (+ date (* i delay))))
      (MidiFreeEv event) )		; dispose the original note
    ))							; <== EVALUATE THIS DEFINITION

(send-multiple-notes 10 72 1000)			; <== EVALUATE THIS EXPRESSION.

(progn (send-multiple-notes 6 60 800)
       (send-multiple-notes 8 72 600))			; <== EVALUATE THIS EXPRESSION.

;; NOTE : Events can be sent in the future. MidiShare internal scheduler takes care of 
;; sending them at the right time according to the specified date.

;;===================================================================================
;; test 16 : send 'hello' lyric
;;===================================================================================
;; MidiShare defines several types of events. Some of them correspond to real Midi
;; messages, some others (like notes) are translated to Midi messages and some others
;; correspond to Midi File 1.0 data. Here is an example of sending Midi File Lyrics
;; data to another application. For this test you need to launch 'msDisplay' application
;; (in the 'MidiShare suite' folder) 
;;
;; WARNING : this test as well as test 17 and 18 will *not* work with the mono version 
;; of MidiShare since different MidiShare clients opened in different contexts won't 
;; see each other.
;; 

;; Then we connect MCL to msDisplay
 (MidiConnect *refnum* (MidiGetNamedAppl "msDisplay") -1) ; <== EVALUATE THIS EXPRESSION.

(defun send-lyric (aString)
  (let ((event (MidiNewEv typeLyric)))
    (unless (ccl:%null-ptr-p event)
      (chan event 0)
      (port event 0)
      (text event aString)		
      (MidiSendIm *refnum* event)) ))	; <== EVALUATE THIS DEFINITION

 (send-lyric "Hello")					; <== EVALUATE THIS EXPRESSION.
 
;; msDisplay shows the 'Hello' message in its window

;;===================================================================================
;; test 17 : send a text event
;;===================================================================================
;; Here is a more general way to send Midi File textual events 

(defun send-text (aType aString)
  (let ((event (MidiNewEv aType)))
    (unless (ccl:%null-ptr-p event)
      (chan event 0)
      (port event 0)
      (text event aString)		
      (MidiSendIm *refnum* event)) ))			; <== EVALUATE THIS DEFINITION

(progn
  (send-text typeTextual "Hello")
  (send-text typeCopyright "Mozart")
  (send-text typeSeqName "Concerto")
  (send-text typeInstrName "Piano")
  (send-text typeLyric "Hiiiiiii")
  (send-text typeMarker "mark 1")
  (send-text typeCuePoint "Reverb here"))		; <== EVALUATE THIS EXPRESSION.


;;===================================================================================
;; test 18 : send an SMPTE offset
;;===================================================================================

(defun send-smpte-offset (format hours minutes seconds frames subframes)
  (let ((event (MidiNewEv typeSMPTEOffset)))
    (unless (ccl:%null-ptr-p event)
      (chan event 0)
      (port event 0)
      (field event 0 format)		
      (field event 1 hours)		
      (field event 2 minutes)		
      (field event 3 seconds)		
      (field event 4 frames)		
      (field event 5 subframes)	
      (MidiSendIm *refnum* event)) ))			; <== EVALUATE THIS DEFINITION

(send-smpte-offset Smpte25Fr 10 24 59 12 00)		; <== EVALUATE THIS EXPRESSION.


;;===================================================================================
;; test 19 : delay and transpose incoming notes
;;===================================================================================
;; We are now going to see how to receive events. Every client application have a 
;; reception fifo where incoming events are stored. Applications can be informed
;; in real-time of incoming events using a receive alarm. But from Lisp we need to do 
;; some polling because Lisp code can't be called at interrupt level.
;; In this example, note, keyOn and keyOff events are transposed and sent with a delay. 
;; Other received events are deleted. The program loops until the stopkey is pressed.
;; Be sure to have a Midi keyboard connected to the modem port.

;;; HKT: changed to use stopkey instead off ccl:mouse-down-p

(defun transform (transpose delay &optional (stopkey 0))
  (MidiConnect *refnum* 0 -1)           ; connect MCL to MidiShare
  (MidiConnect 0 *refnum* -1)           ; connect MidiShare to MCL
  (MidiFlushEvs *refnum*)               ; flush old events in the rcv fifo
  (loop (if (eql stopkey t) (return))   ; loop until stopk pressed
      (do ((event (MidiGetEv *refnum*) (MidiGetEv *refnum*))) ;read all the events
          ((ccl:%null-ptr-p event))     ; in the rcv fifo
        ;; note, keyOn and KeyOff events
        (if (member (evtype event) (list typeNote typeKeyOn typeKeyOff))
          (if (eql stopkey (pitch event))
            (progn (setf stopkey t)
                   (MidiFlushEvs *refNum*))
            (progn                      
              (pitch event (+ transpose (pitch event))) ;   are transposed
              (date event (+ delay (date event))) ;   delayed
              (MidiSend *refnum* event))) ;   and sent.
          (MidiFreeEv event))))         ; other events are deleted
  (MidiConnect 0 *refnum* 0)            ; break the connection from MidiShare to MCL 	
  )                                     ; <== EVALUATE THIS DEFINITION

;;; HKT:  stops on c-sharp next to middle c
(transform 12 1000 61)                  ; <== EVALUATE THIS EXPRESSION.


;;===================================================================================
;; test 20 : some (pseudo) realtime processing
;;===================================================================================
;; In this example we define a MCL process to receive incoming events.


(progn

  (defun waiting-events-p (refnum)
    (> (midicountevs refnum) 0))
  
  (defun midi-thru (refnum)
    (loop
        ;(format t "~&processing...")
      (do ((event (MidiGetEv refnum) (MidiGetEv refnum)))
          ((ccl:%null-ptr-p event))
        (midisendim refnum event))
      (process-wait "waiting events" #'waiting-events-p refnum))))


;;; <== EVALUATE THESE EXPRESSIONS

(MidiConnect *refnum* 0 -1)           ; connect MCL to MidiShare
(MidiConnect 0 *refnum* -1)           ; connect MidiShare to MCL
(defparameter *midishare-process*     ; run process
  (process-run-function '(:name "midi-thru" :priority 2) #'midi-thru *refnum*))
;(process-kill *midishare-process*)   ; kill process when you are done.


;;===================================================================================
;; test 21 : more (pseudo) realtime processing
;;===================================================================================
;; In this example we simultate the tasks and the receive alarms of MidiShare.  


(progn
  (defvar *gc-protect-tbl*)
  
  (defun make-protection-tbl (size)
    "create an empty protection table"
    (setq *gc-protect-tbl* (make-array size))
    (dotimes (i size) (setf (aref *gc-protect-tbl* i) (+ i 1)))
    (setf (aref *gc-protect-tbl* (- size 1)) nil))
  
  (defun protect (task)
    "install a task in the protectio table and return its index"
    (let ((index (aref *gc-protect-tbl* 0)))
      (when index
        (setf (aref *gc-protect-tbl* 0) (aref *gc-protect-tbl* index))
        (setf (aref *gc-protect-tbl* index) task)
        index)))
  
  (defun refere (index)
    "return the task at index and make this index empty"
    (let ((task (aref *gc-protect-tbl* index)))
      (setf (aref *gc-protect-tbl* index) (aref *gc-protect-tbl* 0))
      (setf (aref *gc-protect-tbl* 0) index)
      task))

  (make-protection-tbl 500)
  ; :long :word :long :long :long
  (defcallback eval-task (:unsigned-fullword date :unsigned-halfword refnum
                                             :unsigned-fullword index :unsigned-fullword arg2
                                             :unsigned-fullword arg3)
    "evaluate the task at index"
    (declare (ignore date refnum arg2 arg3))
    (let ((task (refere index)))
      (when (consp task) (apply (car task) (cdr task)))))
  
  (defun schedule-task (date &rest task)
    (MidiDTask eval-task date *refnum* (protect task) 0 0))
  
  ;; macro to schedule expressions
  
  (defmacro at (date form)
    `(schedule-task ,date ,(symbol-function (car form)) ,@(cdr form)))
  
  (defmacro after (delay form)
    `(schedule-task (+ (MidiGetTime) ,delay) ,(symbol-function (car form)) ,@(cdr form)))

  )							; <== EVAL THESE DEFINITIONS



(progn

  (defvar *rcv-alarm* nil)

  (defun waiting-events-tasks-p (refnum)
    (or (> (midicountevs refnum) 0) 
        (> (midicountdtasks refnum) 0)))
  
  (defun midi-rcv-and-tasks (refnum)
    (let (*rcv-alarm*)
      (loop 
        (if (> (midicountevs refnum) 0)
          (if *rcv-alarm*
            (funcall *rcv-alarm* refnum)
            (midiflushevs refnum)))
        (dotimes (i (MidiCountDTasks refnum)) 
          (MidiExec1DTask refnum))
        (process-wait "waiting events or tasks" #'waiting-events-tasks-p refnum))))

  (defun set-rcv-alarm (process rcv-alarm)
    (setf (symbol-value-in-process '*rcv-alarm* process) rcv-alarm))

  )								; <== EVALUATE THESE DEFINITIONS

(process-preset *midishare-process*  
                #'midi-rcv-and-tasks *refnum*)			; <== EVALUATE THIS EXPRESSION.



;;---------------------------------------------------------------------------------------------
;; examples of scheduling in the future

(after 2000 (print "Wake up !!!!"))				; <== EVALUATE THIS EXPRESSION

(defun multi-print (n delay msg)
  (let ((d (MidiGetTime)))
    (dotimes (i n)
      (at (+ d (* i delay)) (print msg)))))		; <== EVALUATE THIS DEFINITION

(multi-print 10 1000 "hello")					; <== EVALUATE THIS EXPRESSION

(progn
  (multi-print 10 1000 "hello")
  (multi-print 5 2000 "goodby"))				; <== EVALUATE THIS EXPRESSION


;;---------------------------------------------------------------------------------------------
;; examples of receive alarms

(defun transpodelay (transpose delay)
  #'(lambda (refnum)
      (do ((event (MidiGetEv refnum) (MidiGetEv refnum)))
          ((ccl:%null-ptr-p event))
        
        (if (member (evtype event) (list typeNote typeKeyOn typeKeyOff))
          (progn
            (pitch event (+ transpose (pitch event)))
            (date event (+ delay (date event)))
            (MidiSend *refnum* event))
          (MidiFreeEv event)))))				; <== EVALUATE THIS DEFINITION

(defun echo (delay attenuation)
  #'(lambda (refnum)
      (do ((event (MidiGetEv refnum) (MidiGetEv refnum)))
          ((ccl:%null-ptr-p event))
        
        (when (or (and (= (evtype event) typeKeyOn) (> (vel event) 0))
                  (= (evtype event) typeNote))
          (do ((dt (+ (date event) delay) (+ dt delay))
               (vl (- (vel event) attenuation) (- vl attenuation))
               (note ))
              ((or (< vl 1)(> vl 127)))
            (unless (ccl:%null-ptr-p (setq note (MidiNewEv typeNote)))
              (port note (port event))
              (chan note (chan event))
              (pitch note (pitch event))
              (vel note vl)
              (dur note (- delay 1))
              (MidiSendAt *refnum* note dt))))
        (MidiFreeEv event))))				; <== EVALUATE THIS DEFINITION


;;---------------------------------------------------------------------------------------------
;; Now we can try these receive alarms
 
(set-rcv-alarm *midishare-process* 
               (transpodelay 12 2000))			; <== EVALUATE THIS EXPRESSION

 
(set-rcv-alarm *midishare-process* 
               (echo 250 5))					; <== EVALUATE THIS EXPRESSION


;;===================================================================================
;; test 22 : close MidiShare Session
;;===================================================================================

(progn
  (process-kill *midishare-process*)
  (MidiClose *refnum*))						; <== EVALUATE THIS EXPRESSION


(defmacro for-every-event (event refnum &body action)
  `(loop 
     (process-wait "waiting events" #'waiting-events-tasks-p ,refnum)
     (do ((,event (MidiGetEv ,refnum) (MidiGetEv ,refnum)))	;read all the events
         ((ccl:%null-ptr-p ,event))
       ,@action)))
