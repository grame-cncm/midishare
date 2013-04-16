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
/* ReadEvInfo class : 
/*****************************************************************************/

package grame.midishare;

/**
This class is used internally. 
*/

public final class  ReadEvInfo
{
	public byte []	buffer;  			// le buffer à lire
	public int   	offset;  			// la position courante dans le buffer
	public int   	free;	 			// la longueur à lire
	public int		toComplete;			// événement à compléter si plusieurs buffers 	
	public int		toRead;				// nbre d'octets restant à lire dans le cas	de	
										// buffers successifs et d'échec à l'allocation	
	public boolean	freeSpace;			// pour les erreurs d'allocation MidiShare	
	
	public ReadEvInfo (int len) {
		buffer = new byte[len];
		offset = 0;
		free = len;
		toComplete = 0;
		toRead = 0;
		freeSpace = true;
	}
		
	public final void ResetBuffer() {
		offset = 0;
		free = buffer.length;
	}
	
	public final void ResetBuffer(int len) {
		offset = 0;
		free = len;
	}
	
	public final boolean EmptyBuffer() {
		return (offset == 0);
	}		
}