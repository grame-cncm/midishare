/*
  Copyright © Grame 2003

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

#ifndef __msLibContext__
#define __msLibContext__

#include "lffifo.h"
#include "msAppls.h"
#include "msKernel.h"
#include "msMem.h"
#include "EventToStream.h"
#include "StreamToEvent.h"

#include "msCommChans.h"
#include "msSharedMem.h"
#include "msThreads.h"

extern TMSGlobalPtr gMem;

#define kCommBuffSize	2048

typedef struct StreamDesc * StreamDescPtr;
typedef struct StreamDesc {
    msStreamBuffer 	parse;
    Ev2StreamRec	stream;
    char 			buff[kCommBuffSize];
}StreamDesc;

typedef Boolean (* SendToServerPtr) (MidiEvPtr e, TMSGlobalPtr g);

typedef struct {
	CommunicationChan 	cchan;		/* commaunication channel handler */
	SharedMemHandler	msMem;		/* public shared memory handler */
	SendToServerPtr		send;
	msThreadPtr			RTThread;	/* real-time thread handler */
	fifo				RTSnd;

	StreamDesc			std;		/* std request stream management */
	StreamDesc			RT;			/* real-time stream management */
	/* methods tables for the streams - shared between std and real-time streams */
    msStreamParseMethodTbl 	parseMthTable;
    msStreamMthTbl			streamMthTable;
} msLibContext, *msLibContextPtr;

#define LibContext 	(msLibContextPtr)gMem->context


#ifdef __cplusplus
extern "C" {
#endif

Boolean StdSend (MidiEvPtr e, TMSGlobalPtr g);
Boolean RTSend (MidiEvPtr e, TMSGlobalPtr g);

#ifdef __cplusplus
}
#endif


#endif
