/*

  Copyright © Grame 2003

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

*/

#include "msFCallHandler.h"

#include "msAppFun.h"
#include "msConnx.h"
#include "msEvents.h"
#include "msExtern.h"
#include "msFilter.h"

extern TMSGlobalPtr gMem;

/*____________________________________________________________________________*/
char * Event2Text (MidiEvPtr e, char *buff, short len)
{
	char * ptr = buff;
    long i, n = MSCountFields( e);
    len -= 1;
    n = (n > len) ? len : n;
    for (i=0; i<n; i++) {
        *ptr++ = (char)MSGetField (e, i);
    }
    *ptr = 0;
	return buff;
}

/*__________________________________________________________________________*/
void FCMidiConnect (MidiEvPtr e)
{
    MSConnect (e->info.cnx.src, e->info.cnx.dst, CnxState(e), gMem);
    MSFreeEv(e, FreeList(Memory(gMem)));
}

/*__________________________________________________________________________*/
void FCMidiSetName (MidiEvPtr e)
{
    char name[256]; 
    MSSetName (RefNum(e), Event2Text (e, name, 256), Clients(gMem));
    MSFreeEv(e, FreeList(Memory(gMem)));
}

/*__________________________________________________________________________*/
void FCMidiSetInfo (MidiEvPtr e)
{
    MSSetInfo (RefNum(e), (void *)e->info.longField, Clients(gMem));
    MSFreeEv(e, FreeList(Memory(gMem)));
}

/*__________________________________________________________________________*/
void FCMidiSetFilter (MidiEvPtr e)
{
    MSSetFilter (RefNum(e), GetFilterPtr(e), Clients(gMem));
    MSFreeEv(e, FreeList(Memory(gMem)));
}
