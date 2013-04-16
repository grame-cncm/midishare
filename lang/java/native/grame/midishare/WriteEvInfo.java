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
/* WriteEvInfo class : 
/*****************************************************************************/

package grame.midishare;

/**
This class is used internally. 
*/

public class  WriteEvInfo
{
	public int     event;   // l'evenement ˆ lineariser
	public byte [] buffer;  // le buffer ˆ ecrire
	public int     offset;  // la position courante dans le buffer
	public int     free;	// la longueur restante dans le buffer
	public int 	   start;   // la prochaine cellule ˆ Žcrire
	
	public WriteEvInfo (int len) {
		buffer = new byte[len];
		offset = 0;
		free = len;
		start = 0;
		event = 0;
	}
	
	public void ResetBuffer() {
		offset = 0;
		free = buffer.length;
	}
		
}
