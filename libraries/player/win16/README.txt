
Player Library for Windows 16 bits


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

- src : the common source code for all platforms.
- win16 : Windows 16 bits specific files

==========================================
Compilation of the Player library
==========================================

To compile the Player library 16 bits thunk, you must have installed on your machine:
- Borland 4.5 or 5 environment. 
- Microsoft "thunk" compiler, "rc" (resource builder), "implib" utility, "nmake" utility.

In the win16 folder:

You must edit the makefile to reflect your machine configuration then use:

nmake 

This compile the thunked Player shared library (a Player.dll file) that can be used in a
32 bits environment.

