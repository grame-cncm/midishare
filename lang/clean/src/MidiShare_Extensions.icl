implementation module MidiShare_Extensions

import StdEnv
import MidiShare_Clean


Note:: !Int !Int !Int !Int !Int !State -> (!MidiEvPtr,!State)

Note h v d c p s 	# (ev,s) = Clean_MidiNewEv typeNote s
			| ev==0 = (ev,s)
			# s = Clean_MidiSetField ev 0 h s
			  s = Clean_MidiSetField ev 1 v s
			  s = Clean_MidiSetField ev 2 d s
			  s = Clean_MidiSetChan ev c s
			  s = Clean_MidiSetPort ev p s
			= (ev,s)		
	
	
KeyOn:: !Int !Int !Int !Int !State -> (!MidiEvPtr,!State)

KeyOn h v c p s 	# (ev,s) = Clean_MidiNewEv typeKeyOn s
			| ev==0 = (ev,s)
			# s = Clean_MidiSetField ev 0 h s
			  s = Clean_MidiSetField ev 1 v s
			  s = Clean_MidiSetChan ev c s
			  s = Clean_MidiSetPort ev p s
			= (ev,s)
	
	
KeyOff:: !Int !Int !Int !Int !State -> (!MidiEvPtr,!State)

KeyOff h v c p s 	# (ev,s) = Clean_MidiNewEv typeKeyOff s
			| ev==0 = (ev,s)
			# s = Clean_MidiSetField ev 0 h s
			  s = Clean_MidiSetField ev 1 v s
			  s = Clean_MidiSetChan ev c s
			  s = Clean_MidiSetPort ev p s
			= (ev,s)
