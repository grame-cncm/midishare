
MidiShare for Linux


=====================
Description
=====================

MidiShare on Linux is separated in a kernel module and a shared library.
The kernel module contains the code used with kernel privilege : scheduler, Midi drivers,IAC...
The shared library contains code used by MidiShare client applications. The library and kernel 
communicate using the standard ioctl mechanisms. A device named /dev/midiShare is defined.
	
=====================
Installation guide
=====================

The MidiShare source package contains:

- ../common : common MidiShare code for all platforms.
- Include  : the MidiShare.h file
- kernel : the platform specific kernel code.
- library : the MidiShare shared library code.
- tests : Two test applications, usefull to test the MidiShare API. 
- tools : Various MidiShare tools. 


Quick install
=============

There are two possible compilation targets :

- one without drivers which allows to test MidiShare applications, Inter Application Communication..
- one with Midi drivers buit using ALSA (http://www.alsa-project.org)


Compilation of MidiShare WITHOUT drivers
==========================================

In the linux folder:

make (or make midishare)


This compile the MidiShare kernel (the MidiShareKernel.o kernel module) 
and the shared MidiShare library (libMidiShare.so)

To install the MidiShare kernel, library and tools, you must have SU privilege:

make install


Compilation of MidiShare WITH drivers
=======================================

In the linux folder:

make midishare-alsa


This compile the MidiShare kernel with ALSA drivers (the MidiShareKernel.o kernel module) 
and the shared MidiShare library (libMidiShare.so)

You will have to download, compile and install Alsa drivers corresponding to your machine setup. 
Look at http://www.alsa-project.org for more information on ALSA drivers compilation and installation.

To install the MidiShare kernel and library  and tools, you must have SU privilege:

make install


Checking MidiShare installation
================================

cat proc/modules should show a "MidiShareKernel" modules installed.

Some basic MidiShare functions are accessible directly in the shell. (They are defined on the tools folder) 
To compile and install the tools:

make tools

Tools are installed in the /home/bin folder. To display the current MidiShare version number:

MidiGetVersion

To display the current MidiShare state (currently opened applications)

MidiGetStat


Running MidiShare tests
=========================
Two test applications are available : test of MidiShare functions, and test of MidiShare event management. 
They allow to check that MidiShare API correctly works. You must have SU privilege to run the test.

cd test

./TestFunctions
./TestEvents

