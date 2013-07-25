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
/* SyncInfo class : data structure used for the SMPTE synchronisation management
/*****************************************************************************/

package grame.midishare;

/**
This class describes the SMPTE synchronisation state of MidiShare.
*@see grame.midishare.Midi#GetSyncInfo
*/

public final class  SyncInfo
{
	/** the current MidiShare date (in milliseconds).*/
	public int		time;	
	/** the current reentrancy count of the interrupt handler.*/
	public int		reenter;
	/** the current synchronisation mode as defined by SetSyncMode.
	*@see grame.midishare.Midi#SetSyncMode
	*/
	public int		syncMode;
	/** the current synchronisation state (0 : unlocked, 1 : locked).	*/	
 	public int		syncLocked;	
 	/** the current synchronisation port.*/
	public int		syncPort;
	/** the date MidiShare started beeing locked to external sync (in milliseconds).*/
	public int		syncStart;
	/** the date MidiShare stopped being locked to external sync (in milliseconds).*/			
 	public int		syncStop;
 	/** the current offset (GetExtTime() - GetTime(), in milliseconds).
 	*@see grame.midishare.Midi#GetExtTime
	*/
 	public int		syncOffset;
 	/** the current value for the timer (implementation dependent).*/
	public int		syncSpeed;
	/** the current count of breaks (transition from state locked to unlocked).*/
 	public int		syncBreaks;
 	/** the current synchronisation format (0 : 24 f/s, 1 : 25 f/s, 2 : 30DF f/s, 3 : 30 f/s).
	*/
	public int		syncFormat;
}