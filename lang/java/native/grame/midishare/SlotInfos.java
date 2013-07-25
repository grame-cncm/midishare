/*
  MidiShare Project
  Copyright (C) Grame 2001-2006

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

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
