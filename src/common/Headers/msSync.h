/*

  Copyright © Grame 1999

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
  grame@rd.grame.fr

*/

#ifndef __msSynch__
#define __msSynch__

#include "msExtern.h"

/*------------------------------------------------------------------------------*/
/*                             functions                                        */
/*------------------------------------------------------------------------------*/

/* linked lists management */
inline MidiEvPtr PopMidiEv 		(MidiEvPtr * list)
								{ return (MidiEvPtr)PopSync(list); }
inline void 	 PushMidiEv 	(MidiEvPtr * list, MidiEvPtr ev)
								{ PushSync(list, (msListPtr)ev); }
inline void 	 PushMidiList 	(MidiEvPtr * list, MidiEvPtr head, MidiEvPtr tail)
								{ PushSyncList(list, (msListPtr)head, (msListPtr)tail); }

/* memory blocks management */
inline MemBlockPtr PopMemBlock 	(MemBlockPtr * list)
								{ return (MemBlockPtr)PopSync(list); }
inline void 	   PushMemBlock (MemBlockPtr * list, MemBlockPtr block)
								{ PushSync(list, (msListPtr)block); }

#endif
