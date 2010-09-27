/*



  Copyright © Grame



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





#include "MidiShare.h"

#include "msNetDriver.h"

#include "INetAlert.h"

#include "NetTools.h"



char * profileName 		= "msWANDriver.ini";

char * NetSectionName	= "Net Params";

char * UDPPort			= "UDP port";

char * GroupTime		= "Grouping time";

char * MaxLat			= "Maximum latency allowed";

char * MaxHost			= "Maximum input connections count";

char * Targets			= "Hosts target";

char * fullProfileName	= 0;



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

					kDefaultWANPort, profileName);	

	cfg->groupTime = GetPrivateProfileInt (NetSectionName, GroupTime,

					kDefaultWANGroupTime, profileName);	

	cfg->maxlat = GetPrivateProfileInt (NetSectionName, MaxLat,

					kDefaultWANLatency, profileName);	

}



//_________________________________________________________

char * NextCnx (char *ptr, Boolean first);

static void LaunchTargets()

{

	char buff[1024], *next, *ptr;

	DWORD n = GetPrivateProfileString (NetSectionName, Targets,

					"", buff, 1024, profileName);

	if (n) {

		ptr = NextCnx (buff, true);

		while (ptr) {

			next = NextCnx (ptr, false);

			if (next) {

				next[-1] = 0;

				Connect (ptr);

				next[-1] = ' ';

			}

			else Connect (ptr);

			ptr = next;

		}

	}

}



//_________________________________________________________

static BOOL Start()

{

	strPtr msg;

	fullProfileName = GetProfileFullName (profileName);

	gParams.driverMode = TRUE;

	gParams.autoQuit = TRUE;

	GetNetConfig (&gParams.net);

	gParams.maxClients = GetPrivateProfileInt (NetSectionName, MaxHost,

					20, profileName);	

	msg = SetUpDriver (&gParams);

	if (msg) {

		Beep (440, 100);

		INetAlert alert;

		alert.FatalError ("MidiShare WAN Driver", "can't launch", msg, 0L);

		return FALSE;

	}

	else LaunchTargets ();

	return TRUE;

}



//_________________________________________________________

static void Stop()

{

	CloseInetDriver ();

}



// intended only to avoid link error

int main() { return 0; }

