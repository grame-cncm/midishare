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

#ifndef __Filter__
#define __Filter__

#include "PlayerDefs.h"
#include "MidiShare.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __Macintosh__
MidiFilterPtr MidiNewFilter();
void MidiFreeFilter(MidiFilterPtr filter);

void MidiAcceptPort (MidiFilterPtr filter, short port, Boolean state);
void MidiAcceptChan (MidiFilterPtr filter, short chan, Boolean state);
void MidiAcceptType (MidiFilterPtr filter, short type, Boolean state);

Boolean MidiIsAcceptedType (MidiFilterPtr filter, short type);
Boolean MidiIsAcceptedPort (MidiFilterPtr filter, short port);
Boolean MidiIsAcceptedChan (MidiFilterPtr filter, short chan);


#endif

#ifdef __cplusplus
}
#endif

#endif
