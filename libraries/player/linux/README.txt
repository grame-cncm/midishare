
Player Library for Linux


===============
Description
===============

The Player library is provided to allow the development of a complete multi-tracks MidiShare sequencer. 
Each Player is a MidiShare application, has 256 tracks, four different synchronization mode,  
events chase and loop markers. A Player use MidiShare sequences. Functions are provided 
to read on write MIDIfiles and convert them to the MidiShare sequence format. 
	
=====================
Installation guide
=====================

The Player source package contains:

- ../src : the source code for all platforms.
- linux  : linux specific files


==========================================
Compilation of Player library
==========================================

You should have compiled MidiShare kernel and library before compiled the Player library.
Look at http://www.grame.fr/MidiShare/SCPP/SCPP.html for more informations.

In the linux folder:

make 

This compile the Player shared library (a libPlayer.1.0 file) 

To install the Player library kernel, you must have SU privilege:

make install

