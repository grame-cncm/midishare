/*
  MidiShare Project
  Copyright (C) Grame 2001
  Copyright (C) Mil Productions 2001

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

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
