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

// printer portB //
#ifdef __MTP_Printer__
#define por 			kPrinter
#define PortUse			0x291	/* bit 7 = 1 signifie : NOT in use */
#define useMidiShare	0x07	/* usage MidiShare du port B dans SPConfig */

#define Ctl			0			/* Offset des registres SCC */
#define Data_		4

#define xmtVector	    0x1b2	/* Printer Port : send			*/
#define rcvVector	    0x1ba	/* Printer Port : normal rcv	*/
#define SpecialVector	0x1be	/* Printer Port : special rcv	*/

#define	WCL				0x40	/*pour le *WriteCtrl*/
#define	NBL				0xf0	/*pour le nibble&*/

#define	PwMngON			BOn()	/* power manager ON  */	
#define	PwMngOFF		BOff()	/* power manager OFF */

#else

// modem portA //
#define por 			kModem
#define PortUse			0x290	/* bit 7 = 1 signifie : NOT in use */
#define useMidiShare	0x70	/* usage MidiShare du port A dans SPConfig */

#define Ctl			2			/* Offset des registres SCC */
#define Data_		6

#define xmtVector	    0x1c2	/* Modem Port : send			*/
#define rcvVector	    0x1ca	/* Modem Port : normal rcv		*/
#define SpecialVector	0x1ce	/* Modem Port : special rcv		*/

#define	WCL				0x80	/*pour le *WriteCtrl*/
#define	NBL				0x0f	/*pour le nibble&*/

#define	PwMngON			AOnIgnoreModem()	/* power manager ON  */	
#define	PwMngOFF		AOff()				/* power manager OFF */

#endif



#include "EventToMidiStream.h"
#include "MidiStreamToEvent.h"
#include "MidiShare.h"

//_______________________________________________________________________
enum{ kModem, kPrinter};

typedef struct {
	Ev2StreamRec	xmt;
	StreamFifo		rcv;
	TMidiSeq		seq;
	MidiEvPtr		checkTask;
	MidiEvPtr		waitEvent;
	short  			MTPOutPort;			// port courant en sortie (SlotRefNum)
	short 		    inSlots[16];		// slot refnum en entrŽe indicŽe par no de port    : inSlots[MTPport]-->SlotRefNum
	short 		    outSlots[256];		// indices de port en sortie indicŽs par no de ref : outSlots[SlotRefNum]-->MTPport
	short		 	slots[32];			// no de reference complet des slots e11;s11;e12;s12
	short 		    refNum;
	Boolean 		state;
} SCC, * SCCPtr;

typedef struct {
	short 			refNum;
	SCC				scc;
	LinearizeMthTbl	lTbl;
	ParseMethodTbl	rTbl; 
	Status2TypeTbl	s2t;
	TFilter			filter;
	Boolean			powerMgr;
} MtpDrv, * MtpDrvPtr;

#define State(data) 	((data)->scc.state)


//_______________________________________________________________________
typedef void (*SCCIntHandler) ();

//_______________________________________________________________________
static inline void INT_OFF(void) = {0x40E7, 0x007C, 0x0700};
static inline void INT_ON(void)  = {0x46DF};

//_______________________________________________________________________
void 	StartXmt (SCCPtr scc);
Boolean OpenLine  (MtpDrvPtr data);
void 	CloseLine (MtpDrvPtr data);

Boolean __CheckSCCFree();

#endif
