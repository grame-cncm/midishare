MidiShare Internet Drivers

The MidiShare Internet Drivers provide real-time communication over Internet 
to MidiShare client applications. 
These drivers are two separate shared libraries :
 - msLANDriver.dylib : provides low latency real-time communication restricted 
                       to local array networks. It includes automatic recognition 
                       of remote hosts.
 - msWANDriver. dylib: provides  real-time communication over Internet. Remote 
					   host connection is controlled by the .ini file.

For both drivers, remote hosts appears as MidiShare slots. Before sending events 
to these hosts you have to connect the corresponding slots to one (or several) 
MidiShare ports using the msDrivers application.

For more information, report to the MidiShare Internet Drivers documentation 
using the ReadMe.html file.
To install these drivers as MidiShare components, see the MidiShare 
documentation.
________________________________________________________________________

The MidiShare Internet drivers have been developed by Grame with the 
contribution of the company Mil Productions (http://www.milprod.com).

________________________________________________________________________
                  Grame - Computer Music Research Lab.
                            9, rue du Garet
                          69001 Lyon - France
                  grame@grame.fr    midishare@grame.fr
