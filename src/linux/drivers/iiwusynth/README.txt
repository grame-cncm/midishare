IIWU Synth

IIWU Synth is a software, real-time synthesizer based on the Soundfont 2 specifications developed by Peter Hanappe 
and others hackers.

The IIWU Synth package provides two applications: iiwusynth and iiwuplay. iiwusynth 
reads and handles incoming MIDI events from a MIDI device. iiwuplay takes a MIDI file as input.

Both applications use a Soundfont for the description of the synthesis data 
(audio samples and control data). The generated sound stream is send to an audio device.

IIWU Synth provides a header file and a shared library that developers can use to 
integrate Soundfont synthesis in any multimedia application.

The IIWU Synth is part of the IIWU project.


The IIWU Synth  home page is :
http://www.hanappe.org/iiwusynth.html

The IIWU Synth can be used with MidiShare. Follow the INSTALL and README files included in the 
IIWU Synth package to see how to compile it to be used with MidiShare.
 
Launch IIWU Synth in midishare mode using :

iiwusynth --midi-driver=midishare

IIWU Synth will appear as a MidiShare application named "iiwusynth" in msconnect.
You will have to load a SoundFont using the "load" command in iiwusynth.
Connect the MidiShare application to it to play notes.