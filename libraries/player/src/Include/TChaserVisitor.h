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


// ===========================================================================
//	TChaserVisitor.h			    
// ===========================================================================


#ifndef __TChaserVisitor__
#define __TChaserVisitor__

#include "TEventTable.h"

//----------------------
// Class TChaserVisitor
//----------------------
/*!
  \brief A chaser visitor contains several hastables (instance of the TEventTable class)
  Each hastable keeps the state of KeyOn, CtrlChange, ProgChange, KeyPress, ChanPress,
  PitchBend, Sysex and Tune events.
*/


class TChaserVisitor :public  TScoreVisitorInterface{

	private:
		/*! Event user */
		TEventSenderInterfacePtr 	fUser;      
		
		/*! Hashtable for KeyOn */
		TKeyOnTable   		fKeyontable;        
		/*! Hashtable for CtrlChange */
		TCtrlChangeTable   	fCtrlchangetable;	
		/*! Hashtable for KeyPress */
		TEventTable   		fKeypresstable;		
		/*! Hashtable for ChanPress */
		TEventTable   		fChanpresstable;	
		/*! Hashtable for PitchBend */
		TEventTable   		fPitchwheeltable;	
		/*! Hashtable for ProgChange */
		TEventTable   		fProgchangetable;	
		/*! Hashtable for Sysex */
		TEventTable   		fSysextable;		
		/*! Tune event */
		MidiEvPtr    		fTune;            
			
	public:
	
		TChaserVisitor(TEventSenderInterfacePtr user):fUser(user),fTune(0){}
		virtual ~TChaserVisitor(){MidiFreeEv (fTune);}
		
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
