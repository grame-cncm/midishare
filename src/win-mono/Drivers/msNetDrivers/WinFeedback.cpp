/*

  Copyright © Grame 2001

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

#include "WinFeedback.h"
#include "TInetAddress.h"
#include "TMidiRemote.h"

void	log (char *msg);
char * 	dateString ();
static 	char msg[512];
static const char * space = "                    ";

//_________________________________________________________________________________
void WinFeedback::Lookup (strPtr name)
{
	sprintf (msg, "Looking for %s...", name);
	log (msg);
}

//_________________________________________________________________________________
void WinFeedback::Connecting (char * name)
{
	sprintf (msg, "Connecting to %s...", name);
	log (msg);
}

//_________________________________________________________________________________
void WinFeedback::CheckLatency (char * name) 
{ 
	sprintf (msg, "Checking latency time...");
	log (msg);
}

//_________________________________________________________________________________
void WinFeedback::CloseReport (TMidiRemote * remote, CnxReportPtr report)
{
	sprintf (msg, "%s%s : remote connection report: v:%d maxLat:%d over:%d missing:%ld", 
		space, remote->GetName(), (int)report->version, (int)report->maxlat, 
		(int)report->maxLatOver, report->missing);
	log (msg);
}

//_________________________________________________________________________________
void WinFeedback::Complete (TMidiRemote * remote) 
{ 
	sprintf (msg, "%s %s : connection complete [%s].", dateString(), remote->GetName(), 
				TInetAddress::IP2String (remote->GetID()));
	log (msg);
}

//_________________________________________________________________________________
void WinFeedback::Remove (TMidiRemote * remote)
{
	sprintf (msg, "%s %s : connection closed.", dateString(), remote->GetName()); 
	log (msg);
}

//_________________________________________________________________________________
void WinFeedback::CnxRefused (strPtr host, short reason)
{
	char *str;
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
	log (msg);
}

//_________________________________________________________________________________
void WinFeedback::Failed (short reason)
{
}
