/*

  Copyright (C) Grame 1996-2012

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
//	THashTable.cpp			    
// ===========================================================================
//
//  A Midi event hashtable used for chase
//

#include "THashTable.h"
#include "UMidi.h"

/*--------------------------------------------------------------------------*/
                                                    
void THashTable::Init() {for (short i = 0; i< kMaxHashCode; i++) {fHashTable[i] = 0;}}

/*--------------------------------------------------------------------------*/

void THashTable::Free()
{
	for (short i = 0; i < kMaxHashCode; i++) {
		UMidi::FreeOneLine(fHashTable[i]);
		fHashTable[i] = 0;
	}
}

/*--------------------------------------------------------------------------*/

void THashTable::InsertEvent(MidiEvPtr ev)
{
	MidiEvPtr e = MidiCopyEv(ev);

	if(e) {
      	Byte hashcode = HashCode(RefNum(e), Chan(e));
		Link(e) = fHashTable[hashcode];
		fHashTable[hashcode]= e;
	}
}

/*--------------------------------------------------------------------------*/

MidiEvPtr THashTable::RemoveEvent(MidiEvPtr e)
{
	Byte hashcode = HashCode(RefNum(e), Chan(e));
	MidiEvPtr prev, cur;
	cur = prev = fHashTable[hashcode];

	if (cur && MatchEvent(cur,e)) {  // Match on the first ev
		fHashTable[hashcode] = Link(cur);
		return cur;
	}
	
	while (cur && !MatchEvent(cur,e)) {
		prev = cur;
		cur = Link (cur);
	}
	
	if (cur) Link(prev) = Link(cur);
	return cur;
}

/*--------------------------------------------------------------------------*/

MidiEvPtr THashTable::RemoveEvent1(MidiEvPtr e)
{
	Byte hashcode = HashCode(RefNum(e), Chan(e));
	MidiEvPtr prev, cur;
	cur = prev = fHashTable[hashcode];

	if (cur && MatchEvent1(cur,e)) {  // Match on the first ev
		fHashTable[hashcode] = Link(cur);
		return cur;
	}
	
	while (cur && !MatchEvent1(cur,e)) {
		prev = cur;
		cur = Link (cur);
	}
	
	if (cur) Link(prev) = Link(cur);
	return cur;
}

/*--------------------------------------------------------------------------*/

MidiEvPtr THashTable::GetEvent(MidiEvPtr e)
{
	Byte hashcode = HashCode(RefNum(e), Chan(e));
	MidiEvPtr cur = fHashTable[hashcode];

	if (cur && MatchEvent(cur,e)) {  // Match on the first ev
		return cur;
	}
	
	while (cur && !MatchEvent(cur,e)) {cur = Link (cur);}
	return cur;
}

/*--------------------------------------------------------------------------*/

MidiEvPtr THashTable::GetEvent1(MidiEvPtr e)
{
	Byte hashcode = HashCode(RefNum(e), Chan(e));
	MidiEvPtr cur =  fHashTable[hashcode];

	if (cur && MatchEvent1(cur,e)) {  // Match on the first ev
		return cur;
	}
	
	while (cur && !MatchEvent1(cur,e)) {cur = Link (cur);}
	return cur;
}





