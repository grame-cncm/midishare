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
//	TChaserVisitor.cpp			    
// ===========================================================================
//
//	A chaser contains several hastables (instance of the TEventTable class)
//  Each hastable keeps the state of KeyOn, CtrlChange, ProgChange, KeyPress, 
//  ChanPress,PitchBend, Sysex and Tune events.

#include "TChaserVisitor.h"


/*--------------------------------------------------------------------------*/

TChaserVisitor::TChaserVisitor(TEventSenderInterfacePtr user) 
{
	fUser = user; 
	fKeyontable = new TKeyOnTable();
	fCtrlchangetable = new TCtrlChangeTable();
	fKeypresstable = new TEventTable();
	fChanpresstable = new TEventTable();
	fPitchwheeltable = new TPitchWheelTable();
	fProgchangetable = new TEventTable();
	fSysextable = new TEventTable();
	fTune = 0;
}

/*--------------------------------------------------------------------------*/

void TChaserVisitor::Init()
{
	fKeyontable->Free();
	fCtrlchangetable->Free();
	fKeypresstable->Free();
	fChanpresstable->Free();
	fPitchwheeltable->Free();
	fProgchangetable->Free();
	fSysextable->Free();
	MidiFreeEv(fTune); 
}

/*--------------------------------------------------------------------------*/
		
TChaserVisitor::~TChaserVisitor() 
{
	delete(fKeyontable);
	delete(fCtrlchangetable);
	delete(fKeypresstable);
	delete(fChanpresstable);
	delete(fPitchwheeltable);
	delete(fProgchangetable);
	delete(fSysextable);
	MidiFreeEv (fTune);
}

/*--------------------------------------------------------------------------*/

void TChaserVisitor::Visite (TKeyOnPtr ev,Boolean forward)
{ 
	fKeyontable->InsertEvent(ev->MidiEvent());
}

/*--------------------------------------------------------------------------*/
void TChaserVisitor::Visite (TKeyOffPtr ev,Boolean forward)
{
	MidiFreeEv(fKeyontable->RemoveEvent(ev->MidiEvent()));
} 

/*--------------------------------------------------------------------------*/

void TChaserVisitor::Visite (TKeyPressPtr ev,Boolean forward)
{
	MidiEvPtr e = ev->MidiEvent();
	MidiEvPtr cur = fKeypresstable->GetEvent(e);
	
	if (cur) {
		MidiSetField(cur, 1, MidiGetField (e,1));
	}else{
		fKeypresstable->InsertEvent(e);
	}
}

/*--------------------------------------------------------------------------*/

void TChaserVisitor::Visite (TCtrlChangePtr ev,Boolean forward)
{
	MidiEvPtr e = ev->MidiEvent();
	MidiEvPtr cur = fCtrlchangetable->GetEvent(e);
	
	if (cur) {
		MidiSetField(cur, 1, MidiGetField (e,1));
	}else {
		fCtrlchangetable->InsertEvent(e);
	}
}

/*--------------------------------------------------------------------------*/

void TChaserVisitor::Visite (TProgChangePtr ev,Boolean forward)
{
	MidiEvPtr e = ev->MidiEvent();
	MidiEvPtr cur = fProgchangetable->GetEvent1(e);
	
	if (cur) {
		MidiSetField(cur, 0, MidiGetField (e,0));
	}else {
		fProgchangetable->InsertEvent(e);
	}
}

/*--------------------------------------------------------------------------*/

void TChaserVisitor::Visite (TChanPressPtr ev,Boolean forward)
{
	MidiEvPtr e = ev->MidiEvent();
	MidiEvPtr cur = fChanpresstable->GetEvent1(e);
	
	if (cur) {
		MidiSetField(cur, 0, MidiGetField (e,0));
	}else {
		fChanpresstable->InsertEvent(e);
	}
}

/*--------------------------------------------------------------------------*/

void TChaserVisitor::Visite (TPitchBendPtr ev,Boolean forward)
{
	MidiEvPtr e = ev->MidiEvent();
	MidiEvPtr cur = fPitchwheeltable->GetEvent1(e);
	
	if (cur) {
		MidiSetField(cur, 0, MidiGetField (e,0));
		MidiSetField(cur, 1, MidiGetField (e,1));
	}else{
		fPitchwheeltable->InsertEvent(e);
	}
}

/*--------------------------------------------------------------------------*/

void TChaserVisitor::Visite (TTunePtr ev,Boolean forward)
{
	if (!fTune) {
		fTune = MidiCopyEv(ev->MidiEvent());
		if (fTune) Link(fTune) = 0;
	}
}

/*--------------------------------------------------------------------------*/

void TChaserVisitor::Visite (TSysExPtr ev,Boolean forward)
{
	MidiEvPtr e = ev->MidiEvent();
	MidiFreeEv(fSysextable->RemoveEvent1(e)); // enleve l'ev courant (A VERIFIER)
	fSysextable->InsertEvent(e);
}

/*----------------------------------------------------------------------------*/

void TChaserVisitor::ChaseOn (ULONG date_ticks)
{
	ULONG date_ms = MidiGetTime();
	
	fSysextable->ChaseOn(fUser,date_ms); // To be done FIRST
	fKeyontable->ChaseOn(fUser,date_ms);
	fCtrlchangetable->ChaseOn(fUser,date_ms);
	fKeypresstable->ChaseOn(fUser,date_ms);
	fProgchangetable->ChaseOn(fUser,date_ms);
	fChanpresstable->ChaseOn(fUser,date_ms);
	fPitchwheeltable->ChaseOn(fUser,date_ms);
	
	if (fTune) fUser->CopyAndUseEvent(fTune,date_ms);
}

/*--------------------------------------------------------------------------*/

void TChaserVisitor::ChaseOff (ULONG date_ticks)
{
	ULONG date_ms = MidiGetTime();
	
	fSysextable->ChaseOff(fUser,date_ms);
	fKeyontable->ChaseOff(fUser,date_ms);
	fCtrlchangetable->ChaseOff(fUser,date_ms);
	fKeypresstable->ChaseOff(fUser,date_ms);
	fProgchangetable->ChaseOff(fUser,date_ms);
	fChanpresstable->ChaseOff(fUser,date_ms);
	fPitchwheeltable->ChaseOff(fUser,date_ms);
	
}


