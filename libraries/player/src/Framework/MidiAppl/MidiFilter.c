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

#include "MidiFilter.h"

/*--------------------------------------------------------------------------*/

#ifdef __Macintosh__
	
MidiFilterPtr 	MidiNewFilter() { return (MidiFilterPtr)NewPtr(sizeof(TFilter));}
void 		MidiFreeFilter(MidiFilterPtr filter) { DisposePtr((Ptr)filter);}



/*--------------------------------------------------------------------------*/

void MidiAcceptPort (MidiFilterPtr filter, short port, Boolean state)
{
	if (filter) {
		if (state)
			AcceptBit (filter->port, port);
		else
			RejectBit (filter->port, port);
	}
}
/*--------------------------------------------------------------------------*/

void  MidiAcceptChan (MidiFilterPtr filter, short chan, Boolean state)
{
	if (filter) {
		if (state)
			AcceptBit (filter->channel, chan);
		else
			RejectBit (filter->channel, chan);
	}
}
/*--------------------------------------------------------------------------*/

void  MidiAcceptType (MidiFilterPtr filter, short type, Boolean state)
{
	if (filter) {
		if (state)
			AcceptBit (filter->evType, type);
		else
			RejectBit (filter->evType, type);
	}
}

/*--------------------------------------------------------------------------*/

Boolean  MidiIsAcceptedType (MidiFilterPtr filter, short type)
{
	return (filter) ? IsAcceptedBit(filter->evType,type) : false;
}

/*--------------------------------------------------------------------------*/

Boolean  MidiIsAcceptedPort (MidiFilterPtr filter, short port)
{
	return (filter) ? IsAcceptedBit(filter->port,port) : false;
}

/*--------------------------------------------------------------------------*/

Boolean  MidiIsAcceptedChan (MidiFilterPtr filter, short chan)
{
	return (filter) ? IsAcceptedBit(filter->channel,chan): false;
}

#endif