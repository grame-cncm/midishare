// ===========================================================================
// The Player Library is Copyright (c) Grame, Computer Music Research Laboratory 
// 1996-1999, and is distributed as Open Source software under the Artistic License;
// see the file "Artistic" that is included in the distribution for details.
//
// Grame : Computer Music Research Laboratory
// Web : http://www.grame.fr/Research
// E-mail : MidiShare@rd.grame.fr
// ===========================================================================


// ===========================================================================
//	THashTable.cpp			    
// ===========================================================================
//
//  A Midi event hashtable used for chase
//

#include "THashTable.h"
#include "UMidi.h"

/*--------------------------------------------------------------------------*/
                                                    
void THashTable::Init(){ for (short i = 0; i< kMaxHashCode; i++) {fHashTable[i] = 0;} }

/*--------------------------------------------------------------------------*/

void THashTable::Free()
{
	for (short i = 0; i < kMaxHashCode; i++) {
		UMidi::FreeOneLine(fHashTable[i]);
		fHashTable[i] = 0;
	}
}

/*--------------------------------------------------------------------------*/

void THashTable::InsertEvent( MidiEvPtr ev)
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

	if (cur && MatchEvent(cur,e)){  // Match on the first ev
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

	if (cur && MatchEvent1(cur,e)){  // Match on the first ev
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

	if (cur && MatchEvent(cur,e)){  // Match on the first ev
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

	if (cur && MatchEvent1(cur,e)){  // Match on the first ev
		return cur;
	}
	
	while (cur && !MatchEvent1(cur,e)) { cur = Link (cur);}
	return cur;
}





