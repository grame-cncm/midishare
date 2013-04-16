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
/* SmpteLoc class : data structure used for the SMPTE synchronisation management
/*****************************************************************************/

package grame.midishare;

/**
This class describes a SMPTE location. 
*@see grame.midishare.Midi#Smpte2Time
*@see grame.midishare.Midi#Time2Smpte
*/

public final class  SmpteLoc
{
	/** format: (0:24f/s, 1:25f/s, 2:30DFf/s, 3:30f/s) */
	public short		format;
	/** hours : 0..23.*/			
 	public short		hours;
 	/** minutes : 0..59*/				
 	public short		minutes;	
 	/** seconds : 0..59*/		
 	public short		seconds;	
 	/** frames : 0..30 (according to format)*/
 	public short		frames;	
 	/** 0..99 (1/100 of frames) */	
 	public short		fracs;			
}