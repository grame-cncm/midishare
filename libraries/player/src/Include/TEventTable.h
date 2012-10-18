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
//	TEventTable.h			 
// ===========================================================================

#ifndef __TEventTable__
#define __TEventTable__

#include "THashTable.h"
#include "TEventSenderInterface.h"

//-------------------
// Class TEventTable 
//-------------------
/*!
   \brief A Midi event hashtable used for chase.
*/

class TEventTable {

	protected:
	
		THashTable fHashTable;
		
		Boolean CheckTrack(MidiEvPtr e, short tracknum) {return (RefNum(e) == tracknum);}
	
	public:
	
		void		Free() {fHashTable.Free();}
		void 		InsertEvent(MidiEvPtr ev) 	{fHashTable.InsertEvent(ev);}
		MidiEvPtr 	GetEvent(MidiEvPtr ev)		{return fHashTable.GetEvent(ev);}
		MidiEvPtr 	GetEvent1(MidiEvPtr ev)		{return fHashTable.GetEvent1(ev);}
		MidiEvPtr 	RemoveEvent(MidiEvPtr ev)	{return fHashTable.RemoveEvent(ev);}
		MidiEvPtr 	RemoveEvent1(MidiEvPtr ev)	{return fHashTable.RemoveEvent1(ev);}
	
		virtual ~TEventTable() {}
		
		virtual void ChaseOn(TEventSenderInterfacePtr user , ULONG date_ms) 
		{
			MidiEvPtr cur,e;
			
			for (short i = 0; i < kMaxHashCode; i++) {
				cur = fHashTable.GetLine(i);
				while (cur) {
					if ((e = MidiCopyEv(cur))) {user->UseEvent(e,date_ms);}
					cur = Link(cur);
				}
			}
		}
		
		virtual void ChaseOff(TEventSenderInterfacePtr user, ULONG date_ms) {}
				
};

typedef TEventTable FAR * TEventTablePtr;

//-------------------
// Class TKeyOnTable 
//-------------------
/*!
   \brief A Midi event hashtable used for KeyOff chase.
*/

class TKeyOnTable : public TEventTable {

	public:
	
		virtual ~TKeyOnTable() {}

		void ChaseOn(TEventSenderInterfacePtr user, ULONG date_ms) {}
	
		void ChaseOff(TEventSenderInterfacePtr user, ULONG date_ms) 
		{
			MidiEvPtr cur,e;
			
			for (short i = 0; i < kMaxHashCode; i++) {
				cur =  fHashTable.GetLine(i);
				while (cur) {
					if ((e = MidiCopyEv(cur))) {
						EvType(e) = typeKeyOff;
						Vel(e) = 64; // KeyOff 
						user->UseEvent(e,date_ms);
					}
					cur = Link(cur);
				}
			}
		}
			
};

typedef TKeyOnTable FAR * TKeyOnTablePtr;

//------------------------
// Class TCtrlChangeTable 
//------------------------
/*!
   \brief A Midi event hashtable used for CtrlChange chase.
*/

class TCtrlChangeTable : public TEventTable {

	private:

		Byte	fCtrldefault[kMaxCtrl];     // Default value table for controlers
		
		Boolean IsSustain(MidiEvPtr e) { return ((MidiGetField(e,0) == 64) && (MidiGetField(e,1) == 127));}
		
	public:
	
		TCtrlChangeTable() {for (short i = 0; i < kMaxCtrl; i++) {fCtrldefault [i] = 0;}}
		virtual ~TCtrlChangeTable() {}
	
		void ChaseOff(TEventSenderInterfacePtr user, ULONG date_ms) 
		{
			MidiEvPtr cur,e;
			
			for (short i = 0; i < kMaxHashCode; i++) {
				cur =  fHashTable.GetLine(i);
				while (cur) {
					if (IsSustain(cur) && (e = MidiCopyEv(cur))) { 	
						MidiSetField(e,1,fCtrldefault[64]); 		// Default value
						user->UseEvent(e,date_ms);
					}
					cur = Link(cur);
				}
			}
		}		
				
};

typedef TCtrlChangeTable FAR * TCtrlChangeTablePtr;

#endif
