;    From Jacques.Herry@wanadoo.fr 
;            Jacques Herry 
;           35, resid Corlay 
;           22300 Lannion 
;            France 
;  	Grame  MidiShare for schemers. 
;  "Welcome to Grame MidiShare(c)extension player, version 1.6.22 Copyright(c)2001 Jacques Herry" 


;;(load-extension "MidiShare.so")
;;(load-extension "Player.so")


(load-extension "mzshare3216")

(if (midi-share?)
(load-extension "mzplayer3216")
(display "(load-extension \"mzshare\") before loading mzplayer"))

;;def from C header
(load "libmidishare.scm")  
(load "libplayer.scm")


(define player (open-player "player-plt"))

;;connecting to hardware
(midi-connect player 0 #t)
(midi-connect 0 player #t)

;;recept of midi-events
(printf "seq~n")
(define seq (midi-new-seq))

;;I fill the struct info for later use (a la Pascal) with midi-file-load
;; the 20 values are here just for dummy fill the struct wich is not automagically filled.
(define mf-infos (make-midi-file-infos 20 20  20 20))

;;pack the player with the midi-file infos
;;errorCodes are in player.h
(printf "midi-file-load~n")

;change the name of midifile for your need
(let ((error-code  (midi-file-load "c:/windows/media/canyon.mid" seq mf-infos)))
	   (printf "erreurCode = ~s  ~n"   error-code)
	   (if (= error-code noErr) ;;all is ok
	     		 (set-all-track-player! player seq (midi-file-infos-clicks mf-infos))
			 (display "not ok "))
	   (newline))
	
(printf "format-> ~s~n tracks-> ~s~n " (midi-file-infos-format mf-infos)(midi-file-infos-tracks mf-infos))

		 
;(define pos-loop-start (make-pos-player  29 1 1 ))
;(define pos-loop-end (make-pos-player 30 1 1 ))

;; you can go to the loop with this proc set or unset the loop with
(display "now execute (start-player player)")(newline)
(display "now execute  (go-and-loop)")(newline)
(define (go-and-loop)
  (set-loop-player! player kLoopOn) ;loop on
(set-pos-bbu-player! player  (make-pos-player  29 1 1 ))
(set-loop-end-bbu-player! player  (make-pos-player 30 1 1 )) 
(set-loop-start-bbu-player! player  (make-pos-player  29 1 1 ))
(display "you can stop the loop with (set-loop-player! player kLoopOff)")(newline))

;;create new state
  (define st (get-state-player player  ))

;;read the player bar
(state-player-bar st)

;;read the state of the previously allocated st
;;below in proc get-pos
;;(get-state-player player st) => st is filled with state-infos
;; get-state-player return also his 2d arg filled
;; so proc state-player-bar get st after
;; 

(define (get-pos)
(printf "bar ~s  beat ~s  unit ~s ~n " (state-player-bar (get-state-player player st)) 
(state-player-beat st)(state-player-unit st)))
