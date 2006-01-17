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