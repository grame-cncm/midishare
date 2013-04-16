/*
  MidiShare Project
  Copyright (C) Grame 1999

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

package grame.midishare;

import grame.midishare.*;
import java.util.*;

/**
This class allows a simpler manipulation of MidiShare applications in Java. 
A MidiNativeAppl instance is associated to a MidiShare application. Java code for
Alarms and Tasks is <b> directly called from the native side</b>. 
Use this class with caution  : since Java code is called from a native real-time thread, 
it has to be fast otherwise the application may ruin real-time performances of the machine.
*/
public class MidiNativeAppl extends MidiAppl {

		/**
 		* Constructor.
		*/
		public MidiNativeAppl() {}
	
		/* WARNING : this method must not :
			- be declared private since MidiNativeAppl could not overwrite it
			- be declared protected otherwise a derived class could overwrite it by error.
		*/
	
		void Init123456789() 
		{
			mode = kNativeMode;
			appl = new MidiApplNative();
		}
		
}

/**
 Internal use
*/
final class MidiApplNative extends MidiApplImpl {}



