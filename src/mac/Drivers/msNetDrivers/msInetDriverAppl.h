/*

  Copyright © Grame 2001
  Copyright © Mil Productions 2001

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


#ifndef __msInetDriverAppl__
#define __msInetDriverAppl__

#include "StrConstants.h"
#include "InetTypes.h"
#include "FeedbackProvider.h"

extern Boolean doneFlag;

/* Application state	*/
typedef struct state {
	Boolean		driverMode;
	Boolean		showWindow;
	Boolean		autoQuit;
	NetConfig	net;
} State, *StatePtr;


#ifdef __cplusplus
extern "C" {
#endif

#ifdef __WANInet__

strPtr  SetUpDriver (StatePtr params, FeedbackProvider * f);
strPtr  ReStart (StatePtr params, FeedbackProvider * f);
void 	Connect (Str255 remote);
void 	Disconnect (long remoteID);

#else

strPtr  SetUpDriver (StatePtr params);
strPtr  ReStart (StatePtr params);
#endif


Boolean CheckOT ();
Boolean SetDriverMode (Boolean on);
void 	CloseInetDriver ();
void 	DoIdle ();

#ifdef __cplusplus
}
#endif

#endif