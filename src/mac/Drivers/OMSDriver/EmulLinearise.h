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
  grame@rd.grame.fr

*/


#ifndef __EmulLinearise__
#define __EmulLinearise__

#include "MidiShare.h"

//____________
// Data types

typedef unsigned char 	e2pRet;
typedef unsigned char 	uchar;

//___________________________________
// Conversions from events into packets 
typedef struct {
	MidiEvPtr			evToSend;		// event to send	
	MidiEvPtr			cont;			// its continuation if it can not be written in one buffer
	unsigned char	*	dest;			// the destination buffer 
	short				free;			// available room in the buffer
} E2PInfos, *E2PInfosPtr;


//_____________________
// Conversion methods

typedef e2pRet 	(*Ev2PacketFunc)( E2PInfosPtr i);

//_________________________
// Initialisations methods

void InitLinearizeMthTbl (Ev2PacketFunc* e2p);
e2pRet EvToPacket(E2PInfosPtr i, Ev2PacketFunc*	e2p);


#endif
