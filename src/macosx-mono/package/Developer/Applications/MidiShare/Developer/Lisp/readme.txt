interfaces/
	Contains the Lisp FFI to MidiShare and Player library.
        Defines a MidiShare Lisp package and exports 
	the MidiShare API. Also defines #+midishare 
        and #+player features.
        Tested in:
		OpenMCL 0.13/OS X
		MCL 5.0/OS X
		MCL 4.2/MacOS 9.2

openmcl/
	The FFI databases for OpenMCL. Copy the
	midishare/ subdirectory to the darwin-headers/
        subdirectory of your openmcl distribution: 
	cp -r midishare /Lisp/ccl/darwin-headers/midishare

tutorials/

	Contains a tutorial for the MidiShare interface and 
	a tutorial for the Player library interface.
