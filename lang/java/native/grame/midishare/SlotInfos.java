/*

  Copyright © Grame 2001

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
/* SlotInfos class : 
/*****************************************************************************/

package grame.midishare;

/**
This class describes a SlotInfos structure. 
*@see grame.midishare.Midi#GetSlotInfos
*/

public final class  SlotInfos
{
	// pointer to C string (Warning : for internal use only, do not use it)
	int	nameAux;  
	// pointer to C buffer (Warning : for internal use only, do not use it)
	int	cnxAux;  
	
	/** Slot name*/	
	public String	name;
	/** Slot direction 
	*@see grame.midishare.Midi#InputSlot
	*@see grame.midishare.Midi#OutputSlot
	*@see grame.midishare.Midi#InputOutputSlot
	*/
	public int		direction;
	/**Slot connection state*/
 	public byte		cnx[];	
 	/** Reserved */	
 	public int		reserved[];	
 	
 	public SlotInfos() {
 		cnx = new  byte[32];
 		reserved = new int[2];
 	}
}