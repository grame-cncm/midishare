
(load-extension "mzshare3216")
(load "libmidishare.scm")

(define my-ref-num (midi-open "mz"))

(define (send-note ref note time)
  (let ((ev (midi-new-ev 0)))
    (when (midi-ev? ev)
    (midi-set-chan! ev 0)
    (midi-set-port! ev 0)
    (midi-set-field! ev 0 note)
    (midi-set-field! ev 1 80) ;velocite
    (midi-set-field! ev 2 time) ;time
(midi-send-im ref ev))))



(define (transform *refnum* transpose delay)
  (midi-connect *refnum* 0 #t)		; connect refnum to MidiShare (midi-out-hard your sound producer)
  (midi-connect 0 *refnum* #t)		; connect MidiShare to refnum (midi-in-hard your keyboard or another)
  (midi-flush-evs *refnum*)		; flush old events in the rcv fifo
(display "press  <Enter> to stop the loop")
(newline)
(read-char)
  (do ((i 1 (add1 i)))
      ( (char-ready?) "good stop")
    (do ((event (midi-get-ev *refnum*) (midi-get-ev *refnum*)))	;read all the events
	((eq? #f event) "no good stop")		; in the rcv fifo
      (if (member (midi-get-type event) (list typeNote typeKeyOn typeKeyOff typePitchWheel))
	  (begin			; note, keyOn and KeyOff events ...
	    (midi-set-field! event 0 (+ transpose (midi-get-field event 0 ))) ;   are transposed
	    (midi-set-date! event (+ delay (midi-get-date event))) ;   delayed
	    (midi-send *refnum* event))
	  (midi-free-ev event))))) ; other events are deleted

;(transform jo 7 500)					; <== EVALUATE THIS EXPRESSION.




(define (test-interrupt)
  (midi-set-rcv-alarm! refNum joue))
 
; (send-note 1 64)







