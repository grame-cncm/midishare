================
Player Library
================

This library allows to develop multi-tracks synchronisable MidiShare sequencers.


06/19/96 : Version 1.00

10/19/96 : Version 1.05 : minor corrections in GetAllTrack and GetTrack functions,
static PPC library (Code Warior), PPC Shared library Common Lisp interface (MCL 3.9 PPC).

10/30/96 : Version  1.07 : internal improvements

11/29/96 : Version  1.08 : correction of a bug when setting a empty sequence in 
the Player with SetTrack function.

12/02/96 : Version  1.09 : correction of a bug when a Player alarm in called 
during the Player destruction

06/28/97 : Version  1.10 : librairies to be used with CW11 and CWPro. Correction of a bug 
when the Player is in SynchroClock mode (pending KeyOff were not correctly sent)

12/28/97 : Version  1.11 : Correction of some bugs in Chase Events management.

04/01/99 : Version  2.00 : Complete rewrite of the library, new kExternalSync mode and
SetTempo function. Source code is distributed as Open Source Software under 
the "MidiShare Library Artistic License".

The behaviour of the following functions has changed:
- ForwardStepPlayer and BackwardStepPlayer not longer use their second argument, 
for compatibility reasons their prototypes remain the same.
- the KEraseMode used while recording now completely erase the track 
before recording new events.

09/07/99 : Version  2.01 : Some improvements in the source code structure
to improve cross-platform compatibility

23/10/99 : Version  2.02 : Bug correction in kExternalSync synchronisation mode.
Bug correction in chase management for KeyOn with velocity of 0.
Bug correction in the TickTask class destructor.

01/07/00 : Version  2.03 : Merge of the new scheduler branch : simplification of the scheduler class
to correct a synchronization problem on Linux. Works with MidiShare untill version 1.84

27/04/01 : Version  > 2.03 : Works only with MidiShare version >= 1.85
Add the management of PortPrefix events in MIDIFiles

06/02/03 : Version 2.04 : Correct a bug in ouput clock synchronization. Doxygen based source documentation.
Cleanup.

20/11/03 : Version 2.05 : Correct the position setting bug introduced in 2.04. Correct ChaseOff bug for PitchBend :
default values (0 64) were sent in ChaseOff, this behaviour is incorrect. ChaseOff for PitchBend does not send anything.

21/01/04 : Version 2.06 : New SetPosTicks and Version functions, ticks field in PlayerState.

??/??/?? : Next version : Correct bugs in Midifile.c and Tmtracks.c (J.J Ceresa). Correct bug in ConvertTickToTickAtPrevSP and ConvertTickToTickAtPrevClock : floor must be used.

==================
General Remarks
==================

The library entry points are not multi-thread safe. Synchronization in a multi-thread environment 
has to be done by the user of the library.


=================================================================================
The Player Library is Copyright (c) Grame, Computer Music Research Laboratory 
1996-2001, and is distributed as Open Source software under a slightly modified 
version of the Artistic License; see the file "Artistic" that is included in 
the distribution for details.
Grame : Computer Music Research Laboratory
Web : http://www.grame.fr/Research
E-mail : MidiShare@grame.fr
=================================================================================
