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