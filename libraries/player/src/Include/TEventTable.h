// ===========================================================================
// The Player Library is Copyright (c) Grame, Computer Music Research Laboratory 
// 1996-2003, and is distributed as Open Source software under the Artistic License;
// see the file "Artistic" that is included in the distribution for details.
//
// Grame : Computer Music Research Laboratory
// Web : http://www.grame.fr/Research
// E-mail : MidiShare@rd.grame.fr
// ===========================================================================


// ===========================================================================
//	TEventTable.h			 
// ===========================================================================
/*!
   \brief A Midi event hashtable used for chase.
*/


#ifndef __TEventTable__
#define __TEventTable__

#include "THashTable.h"
#include "TEventSenderInterface.h"


//-------------------
// Class TEventTable 
//-------------------

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
	
		virtual ~TEventTable(){}
		
		virtual void ChaseOn (TEventSenderInterfacePtr user , ULONG date_ms) {
			MidiEvPtr cur,e;
			
			for (short i = 0; i < kMaxHashCode; i++) {
				cur =  fHashTable.GetLine(i);
				while (cur) {
					if ((e = MidiCopyEv(cur))){ user->UseEvent(e,date_ms);}
					cur = Link(cur);
				}
			}
		}
		
		virtual void ChaseOff (TEventSenderInterfacePtr user, ULONG date_ms) {}
				
};

typedef TEventTable FAR * TEventTablePtr;

//-------------------
// Class TKeyOnTable 
//-------------------

class TKeyOnTable  :public TEventTable{

	public:
	
		virtual ~TKeyOnTable(){}

		void ChaseOn (TEventSenderInterfacePtr user, ULONG date_ms) {}
	
		void ChaseOff (TEventSenderInterfacePtr user, ULONG date_ms) {
			MidiEvPtr cur,e;
			
			for (short i = 0; i < kMaxHashCode; i++) {
				cur =  fHashTable.GetLine(i);
				while (cur) {
					if ((e = MidiCopyEv(cur))){
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
// Class TPitchWheelTable
//------------------------

class TPitchWheelTable  :public TEventTable{

	public:
	
		virtual ~TPitchWheelTable(){}
	
		void ChaseOff (TEventSenderInterfacePtr user, ULONG date_ms) {
			MidiEvPtr cur,e;
			
			for (short i = 0; i < kMaxHashCode; i++) {
				cur =  fHashTable.GetLine(i);
				while (cur) {
					if ((e = MidiCopyEv(cur))){
						MidiSetField(e,0,0);
						MidiSetField(e,1,64);
						user->UseEvent(e,date_ms);
					}
					cur = Link(cur);
				}
			}
		}		
				
};

typedef TPitchWheelTable FAR * TPitchWheelTablePtr;


//------------------------
// Class TCtrlChangeTable 
//------------------------

class TCtrlChangeTable  :public TEventTable{

	private:

		Byte	fCtrldefault[kMaxCtrl];     // Default value table for controlers
		
		Boolean IsSustain(MidiEvPtr e) { return ((MidiGetField(e,0) == 64) && (MidiGetField(e,1) == 127));}
		
	public:
	
		TCtrlChangeTable() {for (short i = 0; i <kMaxCtrl; i++) {fCtrldefault [i] = 0;}}
		virtual ~TCtrlChangeTable(){}
	
		void ChaseOff (TEventSenderInterfacePtr user, ULONG date_ms) {
			MidiEvPtr cur,e;
			
			for (short i = 0; i < kMaxHashCode; i++) {
				cur =  fHashTable.GetLine(i);
				while (cur) {
					if (IsSustain (cur) && (e = MidiCopyEv(cur))){ 	
						MidiSetField(e,1, fCtrldefault[64]); 		// Default value
						user->UseEvent(e,date_ms);
					}
					cur = Link(cur);
				}
			}
		}		
				
};

typedef TCtrlChangeTable FAR * TCtrlChangeTablePtr;


#endif
