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

#include <windows.h>
#include <process.h>

#include "MidiShare.h"
#include "msNetDriver.h"
#include "INetAlert.h"
#include "NetTools.h"

char * profileName 		= "msLANDriver.ini";
char * NetSectionName	= "Net Params";
char * UDPPort			= "UDP port";
char * GroupTime		= "Grouping time";
char * MaxLat			= "Maximum latency allowed";
char * fullProfileName = 0;

State gParams; 

static BOOL Start();
static void Stop();

//_________________________________________________________
BOOL WINAPI DllEntryPoint (HINSTANCE  hinstDLL, DWORD fdwReason, LPVOID lpvReserved )
{
	switch (fdwReason) {
		case DLL_PROCESS_ATTACH:
			if (MidiGetVersion() < 184) return FALSE;
			return Start();
		case DLL_PROCESS_DETACH:
			Stop();
			break;
	 }
    return TRUE;
}

//_________________________________________________________
static void GetNetConfig(NetConfigPtr cfg)
{
	cfg->port = GetPrivateProfileInt (NetSectionName, UDPPort,
					kDefaultLANPort, profileName);	
	cfg->groupTime = GetPrivateProfileInt (NetSectionName, GroupTime,
					kDefaultLANGroupTime, profileName);	
	cfg->maxlat = GetPrivateProfileInt (NetSectionName, MaxLat,
					kDefaultLANLatency, profileName);	
}

//_________________________________________________________
static BOOL Start()
{
	strPtr msg;
	gParams.driverMode = TRUE;
	gParams.autoQuit = TRUE;
	fullProfileName = GetProfileFullName (profileName);
	GetNetConfig (&gParams.net);
	msg = SetUpDriver (&gParams);
	if (msg) {
		Beep (440, 100);
		INetAlert alert;
		alert.FatalError ("MidiShare Net Driver", "can't launch", msg, 0L);
		return FALSE;
	}
	return TRUE;
}

//_________________________________________________________
static void Stop()
{
	CloseInetDriver ();
}

// intended only to avoid link error
main() { return 0; }
