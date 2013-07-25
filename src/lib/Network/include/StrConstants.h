/*
  MidiShare Project
  Copyright (C) Grame 2001
  Copyright (C) Mil Productions 2001

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/


#ifndef __StrConstants__
#define __StrConstants__

#if macintosh 

#define strLANName			"\pmsLANDriver"
#define strWANName			"\pmsWANDriver"
#define strWakeUpFailure 	"\pcannot wake up"
#define strRemCreateFailure "\pcannot create a remote"
#define strNetInfos			"\pcannot get local broadcast address"
#define strDrvRegFailure 	"\pcannot register driver"
#define strNetDisconnect 	"\pyou have been disconnected from the network"

#define strSockOpenFailure 	"\psocket open failed"
#define strICMPOpenFailure 	"\pICMP control open failed"
#define strTCPSockOpenFailure "\pTCP server open failed"
#define strTCPSockSendFailure "\pTCP communication failure"
#define strLookupFailure 	"\punkown host"
#define strConnectFailure 	"\pcannot connect to host"
#define strNotResponding 	"\phost is not responding correctly"
#define strSockSendFailure	"\psocket send failed"
#define strSockFailure		"\psocket failure"
#define strBusyFailure 		"\pconnection refused (too much clients, already connected or server error)"

#define strMemFail			"\pmemory allocation failed"
#define strMSMemFail		"\pMidiShare is out of memory"
#define strMSApplFail		"\pMidiShare client open failed"
#define strNetControl		"\pInet controler"
#define strAlreadyOpened 	"\premote host is already opened"
#else

#define strLANName			"msLANDriver"
#define strWANName			"msWANDriver"
#define strWakeUpFailure 	"cannot wake up"
#define strRemCreateFailure "cannot create a remote"
#define strNetInfos			"cannot get local broadcast address"
#define strDrvRegFailure 	"cannot register driver"
#define strNetDisconnect 	"you have been disconnected from the network"

#define strSockOpenFailure 	"socket open failed"
#define strICMPOpenFailure 	"ICMP control open failed"
#define strTCPSockOpenFailure "TCP server open failed"
#define strTCPSockSendFailure "TCP communication failure"
#define strLookupFailure 	"unkown host"
#define strConnectFailure 	"cannot connect to host"
#define strNotResponding 	"host is not responding correctly"
#define strSockSendFailure	"socket send failed"
#define strSockFailure		"socket failure"
#define strBusyFailure 		"connection refused (too much clients, already connected or server error)"

#define strMemFail			"memory allocation failed"
#define strMSMemFail		"MidiShare is out of memory"
#define strMSApplFail		"MidiShare client open failed"
#define strNetControl		"Inet controler"
#define strAlreadyOpened 	"remote host is already opened"

#endif

#endif
