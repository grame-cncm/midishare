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
/* MidifileInfos class : data structure used with MidiFileLoad and MidiFileSave functions
/*****************************************************************************/

package grame.midishare.midifile;

/**
A MidiFileInfos object holds informations about a MIDIfile.
*@see grame.midishare.midifile.MidiFileStream#Load
*@see grame.midishare.midifile.MidiFileStream#Save
*/

public final class   MidiFileInfos
{
		
	/**  MIDIfile format. */	
	public static final int midifile0 			 = 0;
	/**  MIDIfile format. */	
	public static final int midifile1 			 = 1;
	/**  MIDIfile format. */	
	public static final int midifile2 			 = 2;
	
	/**  time definition. */	
	public static final int TicksPerQuarterNote		= 0;
	/**  time definition. */	
	public static final int Smpte24					= 24;
	/**  time definition. */	
	public static final int Smpte25					= 25;
	/**  time definition. */	
	public static final int Smpte29					= 29;
	/**  time definition. */	
	public static final int Smpte30					= 30;

	/**  MIDIfile format: midifile0, midifile1 or midifile2.	*/
	public int format;	
	/** time definition	:	TicksPerQuarterNote or Smpte(Smpte24,Smpte25,Smpte29 or Smpte30).		
		*/			
	public int timedef;
	/** tick/per/quarter or frame.			*/				
	public int clicks;
	/** track number.                 	*/					
	public int tracks;         
}