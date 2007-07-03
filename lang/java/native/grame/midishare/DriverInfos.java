/*

  Copyright © Grame 1999-2006

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
/* DriverInfos class : 
/*****************************************************************************/

package grame.midishare;


/**
This class describes a DriverInfos structure. 
*@see grame.midishare.Midi#GetDriverInfos
*/

public final class  DriverInfos
{
	/** Driver name*/	 		
	public String	name;
	/** Driver version*/	
	public short	version;
	/** Driver slot number*/
 	public short	slots;
 	/** Reserved */	
 	public int		reserved[];	
 	
 	public DriverInfos() {
 		reserved = new int[2];
 	}
}
