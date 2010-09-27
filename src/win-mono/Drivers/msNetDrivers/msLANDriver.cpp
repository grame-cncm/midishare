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

#include "NetTools.h"

#include "TInetControler.h"



/* ----------------------------------*/

/* constants definitions             */

TInetControler * gControl = 0;

Boolean * 		 gAutoQuit;

HANDLE			 gIdleThread = 0;

unsigned char doneFlag = false;



/* -----------------------------------------------------------------------------*/

static DWORD WINAPI IdleThread (void * ptr)

{

	TInetControler * inet = (TInetControler *)ptr;

	while (!doneFlag) {

		if (inet) {

			inet->DoIdle();

			Sleep (50);

		}

	}

	return 0;

}



/* -----------------------------------------------------------------------------*/

static HANDLE ThreadCreate (TInetControler * inet)

{

	DWORD id;

	HANDLE thread = CreateThread(NULL, 0, IdleThread, inet, 0, &id);

	return thread;

}



/* -----------------------------------------------------------------------------*/

strPtr SetUpDriver (StatePtr params)

{

	strPtr msg = TCPOpen ();

	if (!msg) msg = ReStart (params);

	return msg;

}



strPtr ReStart (StatePtr params)

{

	strPtr msg;

	MidiName name = InetDriverName (strLANName, params->net.port, kDefaultLANPort);

	msg = CheckName (name);

	if (msg) return msg;



	short mode = params->driverMode ? TInetControler::kDriverMode : TInetControler::kApplMode;

	if (gControl) delete gControl;

	gControl = new TInetControler(&params->net, name, mode);

	if (!gControl) {

		CloseInetDriver ();

		return strMemFail;

	}

	else {

		gIdleThread = ThreadCreate (gControl);

		if (!gIdleThread) {

			CloseInetDriver ();

			return "CreateThread failed";

		}

	}

	gAutoQuit = &params->autoQuit;

	return 0;

}



/* -----------------------------------------------------------------------------*/

void CloseInetDriver ()

{

	doneFlag = true;

	if (gIdleThread) {

		WaitForSingleObject (gIdleThread, 1000);

		CloseHandle (gIdleThread);

	}

	gIdleThread = 0;

	if (gControl) delete gControl;

	gControl = 0;

	TCPClose ();

}

