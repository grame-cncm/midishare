/*

  Copyright © Grame 1996-2004

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
  research@grame.fr

*/

// ===========================================================================
//	TTrackTable.cpp			    
// ===========================================================================
//
//	Tracks management
//

#include "TTrackTable.h"

/*--------------------------------------------------------------------------*/

void TTrackTable::SetParam(short tracknum, short param, short value)
{
	if (IsValid(tracknum, param) && IsNewValue(tracknum, param, value)) // If params are valid 
	{ 
		fTracktable[tracknum].fParam[param] = value;
		if (param == kSolo) {if (value == kSoloOn) fSolo++; else fSolo--;}
	}
}

/*--------------------------------------------------------------------------*/

short TTrackTable::GetParam(short tracknum, short param) 
{
    return (IsValid(tracknum, param)) ? fTracktable[tracknum].fParam[param] : -1;
}

/*--------------------------------------------------------------------------*/

MidiEvPtr TTrackTable::IsPlayable(MidiEvPtr e) {return (IsOnTrack(TrackNum(e))) ? MidiCopyEv(e): 0;}

/*--------------------------------------------------------------------------*/

void TTrackTable::Clear()
{
	fSolo = 0;
	for (short i = 0; i< kMaxTrack; i++) fTracktable[i].Init();
}
