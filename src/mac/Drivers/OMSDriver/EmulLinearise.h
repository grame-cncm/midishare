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
typedef short 			p2eRet;


//___________________________________
// Conversions from events into packets 
typedef struct {
	MidiEvPtr			evToSend;		// event to send	
	MidiEvPtr			cont;			// its continuation if it can not be written in one buffer
	unsigned char	*	dest;			// the destination buffer 
	short				free;			// available room in the buffer
} E2PInfos, *E2PInfosPtr;

//___________________________________
// Conversions from packets into events
typedef struct P2EInfos{
	unsigned char	*	src;			// source buffer 
	short				len;			// length of usable data 
	MidiEvPtr			cont;			// event continuation if it is not completely in the buffer
										
} P2EInfos, *P2EInfosPtr;



//_____________________
// Conversion methods

typedef e2pRet 	(*Ev2PacketFunc)( E2PInfosPtr i);
typedef p2eRet  (*Packet2EvFunc)( MidiEvPtr e, Byte *src, short len);

//_________________________
// Initialisations methods

void InitLinearizeMthTbl (Ev2PacketFunc* e2p, Packet2EvFunc*	p2e);
void InitTypeTbl( Byte*	typeTbl);

MidiEvPtr Packet2Evs( P2EInfosPtr i,   Packet2EvFunc*	p2e, Byte*	typeTbl);
e2pRet EvToPacket(E2PInfosPtr i, Ev2PacketFunc*	e2p);




#endif