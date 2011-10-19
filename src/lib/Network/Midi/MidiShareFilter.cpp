/*

  Copyright © Grame 2001
  Copyright © Mil Productions 2001

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
  grame@grame.fr

*/

 
 #include "MidiShareFilter.h"

//_______________________________________________________________________________
MidiShareFilter::MidiShareFilter ()
{
#if !macintosh
	fFilter = MidiNewFilter();
#endif
	AcceptAll ();
}
 
//_______________________________________________________________________________
MidiShareFilter::~MidiShareFilter ()
{
#if !macintosh
	if (fFilter) MidiFreeFilter (fFilter);
#endif
}
 
//_______________________________________________________________________________
void MidiShareFilter::AcceptAll ()
{
	short i;
	for (i=0; i<256; i++)		AcceptPort (i);
	for (i=0; i<16; i++)		AcceptChan (i);
	for (i=0; i<256; i++)		AcceptType (i);
}
 
//_______________________________________________________________________________
void MidiShareFilter::RejectAll ()
{
	short i;
	for (i=0; i<256; i++)		RejectPort (i);
	for (i=0; i<16; i++)		RejectChan (i);
	for (i=0; i<256; i++)		RejectType (i);
}

//_______________________________________________________________________________
bool MidiShareFilter::IsAcceptedEv (MidiEvPtr e)
{
	unsigned short type = EvType(e);
	bool chanEvent = (type < typeSongPos) || ((type >= typeCtrl14b) && (type <= typeRegParam));
	bool chanAccept = chanEvent ? IsAcceptedChan (Chan(e)) : true;
	return  IsAcceptedPort (Port(e)) &&
			chanAccept &&
			IsAcceptedType (EvType(e));
}
