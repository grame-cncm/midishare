==============================
MidiShare samples for Linux
==============================

The folder contains several simple MidiShare applications in console mode.

- msArpege : is a simple arpegiator.
- msClock : allows to produce synchronization events: start, stop, continue, clocks.
- msDisplay : display incoming events in a textual format.
- msEcho : produces an echo on incoming Note and KeyOn events
- msMidiPlayer : a MIDIfile player
- msSendNote : allows to send a single note.
- msSendSeq : allows to play a MidiShare sequence.
- msSequencer : a simple sequencer with record and play features.
- msTutorial : tutorial for MidiShare programming.

===============
Installation
===============
You must have MidiShare compiled and installed on your machine. The MidiShare Linux kernel 
is available at ftp://www.grame.fr/pub/midishare-kernel/



To compile all applications: 

make all

To compile a specific application

make msEcho
make msTutorial
....

To launch applications:

./msTutorial
./msEcho -delay 500 -vel -10
....
