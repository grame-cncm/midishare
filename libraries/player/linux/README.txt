===========================
Player Library for Linux
===========================

The Player library is provided to allow the development of a complete multi-tracks MidiShare sequencer. 
Each Player is a MidiShare application, has 256 tracks, four different synchronization mode,  
events chase and loop markers. A Player use MidiShare sequences. Functions are provided 
to read on write MIDIfiles and convert them to the MidiShare sequence format. 
	
=====================
Installation guide
=====================

The Player source package contains:

- src : the source code for all platforms.
- linux  : linux specific files, interface for java


==========================================
Compilation of Player library
==========================================

You should have compiled MidiShare kernel and library before compiled the Player library.
Look at http://www.grame.fr/MidiShare/SCPP/SCPP.html for more informations.

In the linux folder:

make 

This compile the Player shared library (a libPlayer.1.0 file) 

To install the Player library, you must have SU privilege:

make install

==========================================
Compilation of Player library for JAVA
==========================================

You should have compiled MidiShare kernel and library before compiled the Player library.
Look at http://www.grame.fr/MidiShare/SCPP/SCPP.html for more informations.

You must have the JAVA JDK installed on your machine. Look at http://java.sun.com/ 
for more informations.

Edit the makefile and change the INC2 and INC3 pathnames to match your environnement

In the linux folder:

make java

This compile the Player shared library for JAVA (a libJPlayer.so file). This library contains all
Player code and the JNI interface for the JVM.

===============
Installation 
===============

To install the Player library for JAVA, you must copy the libJPlayer.so in your jdkxx/lib
folder. It's strongly recommended to native threads vm in conjunction with JNI.

=================
Known problems
==================

- They are still synchronization problems in MidiShare and in the Player library that may crash applications
which use the Player library.

- The Player library has been successfully used with the SUN JRE 1.2.2 version for Linux used in native thread
mode. Use or other JVM implementation may work...or not.

