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
  grame@grame.fr

  modifications history:
   [06-09-99] DF reducing dependencies by replacing msExtern.h by mem.h

*/

#include "msFilter.h"
#include "msMem.h"


MSFunctionType(MidiFilterPtr) MSNewFilter (void) {
	return (MidiFilterPtr)AllocateFilter (sizeof(TFilter));
}

MSFunctionType(void) MSFreeFilter (MidiFilterPtr filter) {
	FreeFilter (filter);
}

MSFunctionType(void) MSAcceptPort(MidiFilterPtr f, short port, Boolean state) {
	if (state) AcceptBit(f->port, port);
	else RejectBit (f->port, port);
}

MSFunctionType(void) MSAcceptChan(MidiFilterPtr f, short chan, Boolean state) {	
	if (state) AcceptBit(f->channel, chan);
	else RejectBit (f->channel, chan);
}

MSFunctionType(void) MSAcceptType(MidiFilterPtr f, short type, Boolean state) {	
	if (state) AcceptBit(f->evType, type);
	else RejectBit (f->evType, type);
}

MSFunctionType(Boolean) MSIsAcceptedPort(MidiFilterPtr f, short port) {
	return (Boolean)IsAcceptedBit (f->port, port);
}

MSFunctionType(Boolean) MSIsAcceptedChan(MidiFilterPtr f, short chan) {	
	return (Boolean)IsAcceptedBit (f->channel, chan);
}

MSFunctionType(Boolean) MSIsAcceptedType(MidiFilterPtr f, short type) {	
	return (Boolean)IsAcceptedBit (f->evType, type);
}

