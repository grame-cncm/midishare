
MidiShare for Linux


=============
Description
=============

MidiShare on Linux is separated in a kernel module and a shared library.
The kernel module contains the code used with kernel privilege : scheduler, Midi drivers,IAC...
The shared library contains code used by MidiShare client applications. The library and kernel 
communicate using the standard ioctl mechanisms. A device named /dev/midishare is defined.
	
=====================
Installation guide
=====================

The MidiShare source package contains:

- ../common : common MidiShare code for all platforms.
- ../lib : common MidiShare library code for all platforms.

- Include  : headers files.
- kernel : the platform specific kernel code.
- library : the MidiShare shared library code.
- lib : common MidiShare library object code
- scripts : usefull scripts to manage the MidiShare environement.
- tests : test applications, usefull to check the MidiShare API. 
- tools : several MidiShare tools. 
- applications : several GTK based MidiShare applications.
- drivers : MidiShare drivers.


==========================
Compilation of MidiShare 
==========================

In the linux folder type:
  ./configure
then 
  make 

This compile the MidiShare kernel (the MidiShare.o kernel module) 
and the shared MidiShare library (libMidiShare.so)

To install the MidiShare kernel and library, you must have SU privilege:

make install

This install in /etc/rc.d/init.d a script named "MidiShare" that load the MidiShare 
module at boot time. An alias on this script is created in rc startup folders level 3 and 5.


========================
Compilation of Drivers 
========================

Drivers are currently defined as regular MidiShare applications that other applications 
can use to access Midi input/output. To compile drivers:

make driver (or the global "make clients" command that compile applications, tools and drivers)

The "msRawMidi" driver access the standard /dev/midi device. To use it:

msRawMidi /dev/midi

This will open a MidiShare application named "dev/midi". Other applications must connect to this application
to send and receive Midi events.

================================
Checking MidiShare installation
================================

cat /proc/modules (or lsmod) should show a "MidiShare" module installed.

======================================
Compilation of tools and applications
======================================

Several basic MidiShare functions are accessible directly in the shell. (They are defined on the tools folder).
Several GTK based applications are available in the applications folder. Drivers are available in the driver folder.

To compile the tools, applications and drivers:

make clients

To install tools,applications,drivers and scripts in /usr/local/bin (with SU privilege):

make clientsinstall

To display the current MidiShare version number:

MidiGetVersion

To display the current MidiShare state (currently opened applications)

MidiGetStat


===============================
Known problems and limitations
===============================

- The MidiShare kernel module uses a 100 Hz timer task, so the scheduler calls the ClockHandler 10 times each 10 ms.
There is no yet a mechanism to synchronize the MidiShare time on the machine real-time clock.
(Look at is MidiShareLinux. c in function intrpt_routine)

- MidiShare callbacks (tasks and alarms) are executed in a separate (possibly real-time) thread. They are no more 
guarantee that callback code is not interrupted by "normal" code as is was the case on Macintosh or Windows. 
Thus some synchronization problems specific to the Linux platform have still to be solved.




