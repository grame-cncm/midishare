MidiShare OMS Driver
--------------------

Unlike other driver implementations, the MidiShare OMS Driver exists only as 
a regular application. 

OMS prevents a background only client to run safely by occasionnaly putting 
modal dialogs in front (for example: to ask the user whether he wants to turn 
AppleTalk off or not... or when OMS detects some problem with the MIDI stream 
transmission). A background only application  trying to display a modal dialog 
results in a crash. Therefore and for safety reasons, there is no more background 
only MidiShare OMS driver.

The MidiShare OMS Driver application may be used like a background only application: 
you can put it in the MidiShare Folder and it will be automatically launched at 
MidiShare wakeup time (ie. when the first MidiShare client opens).

The application Setup menu allows to show/hide the window. 
The second item "Auto quit" allows to control the application behavior at MidiShare 
sleep time (ie. when the last MidiShare client quit) : when checked, the application 
will automatically quit at MidiShare sleep time.
If you want the driver to be as quiet as a background only driver, hide the window, 
check the 'Auto quit' item and put it at the root of the MidiShare folder.
