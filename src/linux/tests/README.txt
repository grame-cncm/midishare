
MidiShare tools
--------------------------------------------------------------------------
This folder contains various tools to check for a correct MidiShare kernel
behavior. These tools run in 2 modes :


Client mode checking :
--------------------
 - TestFunctions: a simple MidiShare client application which test every
                  MidiShare function call. 
 - TestEvents   : a simple MidiShare client application which test for all
                  event types allocation, copy and release.
Their result is put on the standard output.
To build these tools, simply type 'make'


Kernel mode checking:
--------------------
this is done using clients plugged as modules in the MidiShare kernel and 
activated using a specific client. These modules are linux kernel modules
dynamically linked to the MidiShare module.
The modules are :
 - TestFModule.o : kernel module which provides the functions tests
 - TestEModule.o : kernel module which provides the events tests

The clients are provided to activate the tests from user mode :
 - TFClient : TestFModule client
 - TEClient : TestEModule client

These tests output their result on the system console using the kernel
'printk' call. When running X, you can try several ways to get this output
(depending on your system and its configuration) :
 - open a console using xterm -C 
 - get the result in one of the system log files
   (tail -f /var/log/messages for example)

To build the kernel mode tools type the following:
  make modules
or
  make fmodule : to build only the functions module and client
  make emodule : to build only the events module and client
note that 'makefile' redirect these targets to the 'functions' and 
'events' makefiles.

Before launching the kernel mode tests clients, you need to install the 
corresponding modules using :
   make -f functions install
or make -f events install
The target 'uninstall' is provided to remove a module and the 
corresponding device.
