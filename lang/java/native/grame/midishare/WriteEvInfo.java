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
