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
//	THashTable.h			    
// ===========================================================================
/*!
\brief  A Midi event hashtable used for chase.
*/


#ifndef __THashTable__
#define __THashTable__

 
#include "TEvent.h"

//------------------
// Class THashTable 
//------------------

class THashTable{

	private:

		MidiEvPtr fHashTable[kMaxHashCode];
		void Init();
	
	public:
		
		THashTable()	{ Init();}
		~THashTable() 	{ Free();}
	
		void		Free();
		void 		InsertEvent(MidiEvPtr ev);
		MidiEvPtr 	GetEvent(MidiEvPtr ev);
		MidiEvPtr 	GetEvent1(MidiEvPtr ev);
		MidiEvPtr 	RemoveEvent(MidiEvPtr ev);
		MidiEvPtr 	RemoveEvent1(MidiEvPtr ev);
		MidiEvPtr   GetLine(short line) {return fHashTable[line];}
	
};

typedef THashTable FAR * THashTablePtr;


#endif
