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