Player Library

=================================================================================
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

=================================================================================
The Player Library is Copyright (c) Grame, Computer Music Research Laboratory 
1996-1999, and is distributed as Open Source software under a slightly modified 
version of the Artistic License; see the file "Artistic" that is included in the distribution for details.
Grame : Computer Music Research Laboratory
Web : http://www.grame.fr/Research
E-mail : MidiShare@rd.grame.fr
=================================================================================
