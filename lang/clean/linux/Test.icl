
module Test

import StdEnv

import MidiShare_Clean

Start world

	#	(console,world) = stdio world
		state = 0
		console = fwrites "MidiShare Test: type 'q' to Quit\n" console
		(ref,state) = Clean_MidiOpen "Test" state
		console = fwrites (toString ref) console
		(name,console) = freadline console
		state =  Clean_MidiClose ref state
		console = fwrites (toString state) console
		(ok,world) = fclose console world
	= world
