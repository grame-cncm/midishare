/*

  Copyright © Grame 2000

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
  grame@grame.fr

*/

#ifndef __SCC__
#define __SCC__

#include "EventToMidiStream.h"
#include "MidiStreamToEvent.h"
#include "MidiShare.h"

//_______________________________________________________________________
enum { kModem, kPrinter };

typedef struct {
	Ev2StreamRec	xmt;
	StreamFifo		rcv;
	TMidiSeq		seq;
	MidiEvPtr		checkTask;
	short  			port;
	short 		    slot;
	short 		    refNum;
	Boolean 		state;
} SCC, * SCCPtr;

typedef struct {
	short 			refNum;
	SCC				scc[2];
	SlotRefNum		slot[2];
	short			slotIndex[256];
	LinearizeMthTbl	lTbl;
	ParseMethodTbl	rTbl;
	Status2TypeTbl	s2t;
	TFilter			filter;
	Boolean			powerMgr;
	Boolean			cnxChange;
	Boolean			sleeping;
} SerialDrv, * SerialDrvPtr;

#define State(data, port) 	((data)->scc[port].state)


//_______________________________________________________________________
typedef void (*SCCIntHandler) ();

//_______________________________________________________________________
static inline void INT_OFF(void) = {0x40E7, 0x007C, 0x0700};
static inline void INT_ON(void)  = {0x46DF};

//_______________________________________________________________________
void 	StartXmt (SCCPtr scc);
Boolean OpenLineA  (SerialDrvPtr data);
Boolean OpenLineB  (SerialDrvPtr data);
void 	CloseLineA (SerialDrvPtr data);
void 	CloseLineB (SerialDrvPtr data);

#endif
