/*

  Copyright © Grame 1999

  This library is free software; you can redistribute it and modify it under 
  the terms of the GNU Library General Public License as published by the 
  Free Software Foundation version 2 of the License, or any later version.

  This library is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public License 
  for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

  Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
  grame@rd.grame.fr

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



