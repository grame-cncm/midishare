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
//	THashTable.h			    
// ===========================================================================


#ifndef __THashTable__
#define __THashTable__

 
#include "TEvent.h"

//------------------
// Class THashTable 
//------------------
/*!
\brief  A Midi event hashtable used for chase.
*/

class THashTable{

	private:

		MidiEvPtr fHashTable[kMaxHashCode];
		void Init();
	
	public:
		
		THashTable(){Init();}
		virtual ~THashTable(){Free();}
	
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
