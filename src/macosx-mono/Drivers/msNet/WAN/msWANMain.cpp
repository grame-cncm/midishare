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

#include <unistd.h>

#include "MidiShare.h"
#include "msNetDriver.h"
#include "INetAlert.h"
#include "NetTools.h"
#include "profport.h"
#include "misc.h"

const char * profileName 	= "msWANDriver.ini";
const char * NetSectionName	= "Net Params";
const char * UDPPort		= "UDP port";
const char * GroupTime		= "Grouping time";
const char * MaxLat			= "Maximum latency allowed";
const char * MaxHost		= "Maximum input connections count";
const char * Targets		= "Target hosts";

State gParams; 

#ifdef __cplusplus
extern "C" {
#endif

bool Start();
void Stop();

#ifdef __cplusplus
}
#endif

//_________________________________________________________
static void GetNetConfig (const char *file, NetConfigPtr cfg)
{
	cfg->port = get_private_profile_int (NetSectionName, UDPPort,
					kDefaultWANPort, file);	
	cfg->groupTime = get_private_profile_int (NetSectionName, GroupTime,
					kDefaultWANGroupTime, file);	
	cfg->maxlat = get_private_profile_int (NetSectionName, MaxLat,
					kDefaultWANLatency, file);	
}

//_________________________________________________________
bool Start()
{
	strPtr msg; const char *fullName = MakeMSFullName (profileName);
	gParams.driverMode = true;
	gParams.autoQuit = true;
	GetNetConfig (fullName, &gParams.net);
	gParams.maxClients = get_private_profile_int (NetSectionName, MaxHost,
					20, fullName);	
	msg = SetUpDriver (&gParams);
	if (msg) {
		INetAlert alert;
		alert.FatalError ("MidiShare WAN Driver", "can't launch", msg, 0L);
		return false;
	}
	return true;
}

//_________________________________________________________
void Stop()
{
	CloseInetDriver ();
}
