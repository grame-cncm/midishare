MidiShare OMS Driver
--------------------

Like other driver implementations, the MidiShare OMS Driver exists as
a regular application or as a background only application. 
This background only application requires an enabler to open its real
OMS client. This enabler "msOMSEnabler" should be put in the MidiShare 
Folder with msOMSDriver. Its source code is located in the OMSDriverEnabler
folder.

This enabler is required for the following reason :

when the first OMS client opens and depending on its preferences, OMS put a 
dialog in front to ask the user whether he wants to turn AppleTalk off or not.
A background only application can't be putted in front and therefore, cannot
handle any modal dialog. When msOMSDriver is the first OMS client, opening
this dialog crash the computer. For this reason, msOMSDriver don't open
the OMS client at wakeup time but at the first MidiGetSlotInfo call.
This call (performed by any MidiShare client application) is relayed by
MidiShare from the caller to the driver code and is executed in the context
of the caller. In this context (which should not be a background only context),
opening a dialog is allowed and therefore, opening the OMS client can be safe.

The task performed by the enabler consists in waiting for the msOMSDriver and
in calling MidiGetSlotInfo to operate its real wakeup. The enabler quit as soon 
as done or after a 5 seconds timeout.
