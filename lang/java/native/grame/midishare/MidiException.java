/*
  MidiShare Project
  Copyright (C) Grame 1999

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

/*****************************************************************************/
/* MidiException Class 
/*****************************************************************************/

package grame.midishare;

/**
The class of all MidiException.
*/

public class MidiException extends Exception {
    /**
     * Constructor.
     */
    public MidiException() {
		super();
    }

    /**
     * Constructor with error number.
     */
    public MidiException(int rc) {
		super("MIDI error return code: " + rc);
    }

    /**
     * Constructor with string.
     */
    public MidiException(String string) {
		super("MIDI error: " + string);
    }
}

