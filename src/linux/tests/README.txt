
MidiShare tools
--------------------------------------------------------------------------
This folder contains various tools to check for a correct MidiShare kernel
behavior. Each tool is located in a separate folder:
	- Events: 	MidiShare events allocation, copy and transmission test
	- Functions : 	MidiShare API test
	- Filters : 	applications filters test
	- Drivers : 	MidiShare Drivers support test
	- IO :		MIDI input / output operation test

Each tool can run in 2 modes :


User mode checking :
--------------------
The tool name is T<test name> where <test name> is in :
Events, Filters, Fonctions etc... 
The tool is a MidiShare client running in user mode.
The test result is put on the standard output.

Kernel mode checking:
--------------------
this is done using clients plugged as modules in the MidiShare kernel and 
activated using a specific client. These modules are linux kernel modules
dynamically linked to the MidiShare module.
The module names are T<spec part>Module.o 
The client names are T<spec part>Client 
where <spec part> is specific to each test module. 

These tests output their result on the system console using the kernel
'printk' call. When running X, you can try several ways to get this output
(depending on your system and its configuration) :
 - open a console using xterm -C 
 - get the result in one of the system log files
   (tail -f /var/log/messages for example)

Before launching a kernel mode test client, you need to install the 
corresponding module using :
   make install
The target 'uninstall' is provided to remove a module and the 
corresponding device. 'install' and 'uninstall' targets are provided
in the folder's makefiles only.

The makefile at the root of the 'test' folder build all the tools,
user and kernel tools included.

Warning: user test of the Drivers is not yet supported.
