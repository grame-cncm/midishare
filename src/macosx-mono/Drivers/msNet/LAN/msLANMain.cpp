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

#include "MidiShare.h"
#include "msNetDriver.h"
#include "INetAlert.h"
#include "NetTools.h"
#include "profport.h"
#include "misc.h"

char * profileName 				= "msLANDriver.ini";
static char * NetSectionName	= "Net Params";
static char * UDPPort			= "UDP port";
static char * GroupTime			= "Grouping time";
static char * MaxLat			= "Maximum latency allowed";

State gParams; 

#ifdef __cplusplus
extern "C" {
#endif

Boolean Start();
void Stop();

#ifdef __cplusplus
}
#endif

//_________________________________________________________
static void GetNetConfig (char *file, NetConfigPtr cfg)
{
	cfg->port = get_private_profile_int (NetSectionName, UDPPort,
					kDefaultLANPort, file);	
	cfg->groupTime = get_private_profile_int (NetSectionName, GroupTime,
					kDefaultLANGroupTime, file);	
	cfg->maxlat = get_private_profile_int (NetSectionName, MaxLat,
					kDefaultLANLatency, file);	
}

//_________________________________________________________
Boolean Start()
{
	strPtr msg;
	gParams.driverMode = true;
	gParams.autoQuit = true;
	GetNetConfig (MakeMSFullName (profileName), &gParams.net);
	msg = SetUpDriver (&gParams);
	if (msg) {
		INetAlert alert;
		alert.FatalError ("MidiShare Net Driver", "can't launch", msg, 0L);
		return false;
	}
	return true;
}

//_________________________________________________________
void Stop()
{
	CloseInetDriver ();
}
