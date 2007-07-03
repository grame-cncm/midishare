/*

  Copyright © Grame 1996-2006

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
//	TChaserVisitor.cpp			    
// ===========================================================================
//
//	A chaser contains several hastables (instance of the TEventTable class)
//  Each hastable keeps the state of KeyOn, CtrlChange, ProgChange, KeyPress, 
//  ChanPress,PitchBend, Sysex and Tune events.

#include "TChaserVisitor.h"

/*--------------------------------------------------------------------------*/

void TChaserVisitor::Init()
{
	fKeyontable.Free();
	fCtrlchangetable.Free();
	fKeypresstable.Free();
	fChanpresstable.Free();
	fPitchwheeltable.Free();
	fProgchangetable.Free();
	fSysextable.Free();
	MidiFreeEv(fTune); 
}

/*--------------------------------------------------------------------------*/
void TChaserVisitor::Visite(TKeyOnPtr ev, Boolean forward)
{ 
	MidiEvPtr key = ev->MidiEvent();
	if(Vel(key) == 0)  // KeyOn with Vel == 0 ==> KeyOff
		MidiFreeEv(fKeyontable.RemoveEvent(key)); 
	else
		fKeyontable.InsertEvent(key);
}

/*--------------------------------------------------------------------------*/
void TChaserVisitor::Visite(TKeyOffPtr ev, Boolean forward)
{
	MidiFreeEv(fKeyontable.RemoveEvent(ev->MidiEvent()));
} 

/*--------------------------------------------------------------------------*/

void TChaserVisitor::Visite(TKeyPressPtr ev, Boolean forward)
{
	MidiEvPtr e = ev->MidiEvent();
	MidiEvPtr cur = fKeypresstable.GetEvent(e);
	
	if (cur) {
		MidiSetField(cur, 1, MidiGetField(e, 1));
	} else {
		fKeypresstable.InsertEvent(e);
	}
}

/*--------------------------------------------------------------------------*/

void TChaserVisitor::Visite(TCtrlChangePtr ev, Boolean forward)
{
	MidiEvPtr e = ev->MidiEvent();
	MidiEvPtr cur = fCtrlchangetable.GetEvent(e);
	
	if (cur) {
		MidiSetField(cur, 1, MidiGetField(e, 1));
	} else {
		fCtrlchangetable.InsertEvent(e);
	}
}

/*--------------------------------------------------------------------------*/

void TChaserVisitor::Visite(TProgChangePtr ev, Boolean forward)
{
	MidiEvPtr e = ev->MidiEvent();
	MidiEvPtr cur = fProgchangetable.GetEvent1(e);
	
	if (cur) {
		MidiSetField(cur, 0, MidiGetField(e, 0));
	} else {
		fProgchangetable.InsertEvent(e);
	}
}

/*--------------------------------------------------------------------------*/

void TChaserVisitor::Visite(TChanPressPtr ev, Boolean forward)
{
	MidiEvPtr e = ev->MidiEvent();
	MidiEvPtr cur = fChanpresstable.GetEvent1(e);
	
	if (cur) {
		MidiSetField(cur, 0, MidiGetField(e, 0));
	} else {
		fChanpresstable.InsertEvent(e);
	}
}

/*--------------------------------------------------------------------------*/

void TChaserVisitor::Visite(TPitchBendPtr ev, Boolean forward)
{
	MidiEvPtr e = ev->MidiEvent();
	MidiEvPtr cur = fPitchwheeltable.GetEvent1(e);
	
	if (cur) {
		MidiSetField(cur, 0, MidiGetField(e,0));
		MidiSetField(cur, 1, MidiGetField(e,1));
	} else {
		fPitchwheeltable.InsertEvent(e);
	}
}

/*--------------------------------------------------------------------------*/

void TChaserVisitor::Visite(TTunePtr ev, Boolean forward)
{
	if (!fTune) {
		fTune = MidiCopyEv(ev->MidiEvent());
		if (fTune) Link(fTune) = 0;
	}
}

/*--------------------------------------------------------------------------*/

void TChaserVisitor::Visite(TSysExPtr ev, Boolean forward)
{
	MidiEvPtr e = ev->MidiEvent();
	MidiFreeEv(fSysextable.RemoveEvent1(e)); // enleve l'ev courant (A VERIFIER)
	fSysextable.InsertEvent(e);
}

/*----------------------------------------------------------------------------*/

void TChaserVisitor::ChaseOn (ULONG date_ticks)
{
	ULONG date_ms = MidiGetTime();
	
	fSysextable.ChaseOn(fUser, date_ms); // To be done FIRST
	fKeyontable.ChaseOn(fUser, date_ms);
	fCtrlchangetable.ChaseOn(fUser, date_ms);
	fKeypresstable.ChaseOn(fUser, date_ms);
	fProgchangetable.ChaseOn(fUser, date_ms);
	fChanpresstable.ChaseOn(fUser, date_ms);
	fPitchwheeltable.ChaseOn(fUser, date_ms);
	
	if (fTune) fUser->CopyAndUseEvent(fTune, date_ms);
}

/*--------------------------------------------------------------------------*/

void TChaserVisitor::ChaseOff (ULONG date_ticks)
{
	ULONG date_ms = MidiGetTime();
	
	fSysextable.ChaseOff(fUser, date_ms);
	fKeyontable.ChaseOff(fUser, date_ms);
	fCtrlchangetable.ChaseOff(fUser, date_ms);
	fKeypresstable.ChaseOff(fUser, date_ms);
	fProgchangetable.ChaseOff(fUser, date_ms);
	fChanpresstable.ChaseOff(fUser, date_ms);
	fPitchwheeltable.ChaseOff(fUser, date_ms);
}


