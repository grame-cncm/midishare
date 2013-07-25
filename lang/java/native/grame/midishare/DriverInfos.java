/*
  MidiShare Project
  Copyright (C) Grame 1999-2006

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

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
