=============================
Player Library for Macintosh
=============================

The Player library is provided to allow the development of a complete multi-tracks MidiShare sequencer. 
Each Player is a MidiShare application, has 256 tracks, four different synchronization mode,  
events chase and loop markers. A Player use MidiShare sequences. Functions are provided 
to read on write MIDIfiles and convert them to the MidiShare sequence format. 
        
=====================
Installation guide
=====================

The Player source package contains:

- src : the source code for all platforms.
- mac : Macintosh specific files, project to compile the library in several format.
- mac/Java : JNI interface for java.


==========================================
Compilation of Player library
==========================================

Use the Player2.01.prj Code Warrior 4 project to compile three different targets :

- PlayerPPC : a static PPC library
- Player68k : a static 68k library
- PlayerSharedPPC : a shared PPC library

==========================================
Compilation of JNI interface for JAVA
==========================================

Use the PlayerJNI.prj Code Warrior 4 project in the folder Java to compile two different targets :

- JPlayerMRJ : a shared library which contains the JNI interface and the Player code
- JPlayerMRJ(Shared) : a shared library which only contains the JNI interface. This library can be used with
the PlayerSharedPPC file.

===============
Installation 
===============

Java JNI shared libraries are usually installed in System Folder/ Extensions.

=================
Known problems
=================

None