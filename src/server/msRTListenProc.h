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

#ifndef __msRTListenProc__
#define __msRTListenProc__

#include "msCommChans.h"
#include "msServerContext.h"
#include "msThreads.h"

#include "EventToStream.h"
#include "StreamToEvent.h"

#define kRTReadBuffSize		2048
#define kRTWriteBuffSize	2048

typedef struct {
	msThreadPtr 	RTThread;		/* real time communication thread   */
    msStreamBuffer 	parse;
    Ev2StreamRec	stream;
    char 			rbuff[kRTReadBuffSize];
    char 			wbuff[kRTWriteBuffSize];
} RTComm, * RTCommPtr;

#ifdef __cplusplus
extern "C" {
#endif

int  RTCommInit (msServerContextPtr c, CommunicationChan cc);
void RTCommStop (RTCommPtr rt);

#ifdef __cplusplus
}
#endif


#endif
