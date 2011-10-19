/*

  Copyright © Grame 2002

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

#include <stdio.h>

#include "OSXFeedback.h"
#include "TInetAddress.h"
#include "TMidiRemote.h"
#include "misc.h"

static 	char msg[512];
static const char * space = "                    ";

//_________________________________________________________________________________
void OSXFeedback::Lookup (strPtr name)
{
	sprintf (msg, "Looking for %s...", name);
	logmsg (msg);
}

//_________________________________________________________________________________
void OSXFeedback::Connecting (char * name)
{
	sprintf (msg, "Connecting to %s...", name);
	logmsg (msg);
}

//_________________________________________________________________________________
void OSXFeedback::CheckLatency (char * name) 
{ 
	sprintf (msg, "Checking latency time...");
	logmsg (msg);
}

//_________________________________________________________________________________
void OSXFeedback::CloseReport (TMidiRemote * remote, CnxReportPtr report)
{
	sprintf (msg, "%s%s : remote connection report: v:%d maxLat:%d over:%d missing:%ld", 
		space, remote->GetName(), (int)report->version, (int)report->maxlat, 
		(int)report->maxLatOver, report->missing);
	logmsg (msg);
}

//_________________________________________________________________________________
void OSXFeedback::Complete (TMidiRemote * remote) 
{ 
	sprintf (msg, "%s %s : connection complete [%s].", dateString(), remote->GetName(), 
				TInetAddress::IP2String (remote->GetID()));
	logmsg (msg);
}

//_________________________________________________________________________________
void OSXFeedback::Remove (TMidiRemote * remote)
{
	sprintf (msg, "%s %s : connection closed.", dateString(), remote->GetName()); 
	logmsg (msg);
}

//_________________________________________________________________________________
void OSXFeedback::CnxRefused (strPtr host, short reason)
{
	const char *str;
	switch (reason) {
		case kTooMuchClients: 	str = "too much clients";
			break;
		case kAlreadyConnected:	str = "already connected";
			break;
		case kServerError:		str = "internal server error";
			break;
		default:				str = "unknown error";
	}
	sprintf (msg, "%s %s : connection refused: %s.", dateString(), host, str); 
	logmsg (msg);
}

//_________________________________________________________________________________
void OSXFeedback::Failed (short reason)
{
}
