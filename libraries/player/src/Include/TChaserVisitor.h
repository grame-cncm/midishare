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
//	TChaserVisitor.h			    
// ===========================================================================

/*!
  \brief A chaser visitor contains several hastables (instance of the TEventTable class)
  Each hastable keeps the state of KeyOn, CtrlChange, ProgChange, KeyPress, ChanPress,
  PitchBend, Sysex and Tune events.
*/


#ifndef __TChaserVisitor__
#define __TChaserVisitor__

#include "TEventTable.h"

/*--------------------------------------------------------------------------*/
// Class TChaserVisitor
/*--------------------------------------------------------------------------*/

class TChaserVisitor :public  TScoreVisitorInterface{

	private:
		/*! Event user */
		TEventSenderInterfacePtr 	fUser;      
		/*! Hashtable for KeyOn */
		TEventTablePtr   	fKeyontable;        
		/*! Hashtable for CtrlChange */
		TEventTablePtr   	fCtrlchangetable;	
		/*! Hashtable for KeyPress */
		TEventTablePtr   	fKeypresstable;		
		/*! Hashtable for ChanPress */
		TEventTablePtr   	fChanpresstable;	
		/*! Hashtable for PitchBend */
		TEventTablePtr   	fPitchwheeltable;	
		/*! Hashtable for ProgChange */
		TEventTablePtr   	fProgchangetable;	
		/*! Hashtable for Sysex */
		TEventTablePtr   	fSysextable;		
		/*! Tune event */
		MidiEvPtr    		fTune;            
			
	public:
	
		TChaserVisitor(TEventSenderInterfacePtr user);
		~TChaserVisitor();
		
		void Init();
		
		void Visite (TKeyOnPtr ev,Boolean forward);
		void Visite (TKeyOffPtr ev,Boolean forward); 
		void Visite (TKeyPressPtr ev,Boolean forward);
		void Visite (TCtrlChangePtr ev,Boolean forward);
		void Visite (TProgChangePtr ev,Boolean forward);
		void Visite (TChanPressPtr ev,Boolean forward);
		void Visite (TPitchBendPtr ev,Boolean forward);
		void Visite (TTunePtr ev,Boolean forward);
		void Visite (TSysExPtr ev,Boolean forward);
		
		void ChaseOn (ULONG date_ticks);
		void ChaseOff (ULONG date_ticks);
};


typedef TChaserVisitor FAR * TChaserVisitorPtr;

#endif
