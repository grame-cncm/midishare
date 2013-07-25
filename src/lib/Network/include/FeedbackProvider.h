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


#ifndef __FeedbackProvider__
#define __FeedbackProvider__

#include "StrConstants.h"
#include "InetTypes.h"

class TMidiRemote;
//_________________________________________________________________________________
//    FeedbackProvider
//_________________________________________________________________________________
class FeedbackProvider
{
	public:
		enum	{ kCnxRefused = 1, kNotResponding };
		
		//------------------------------------------ constructeur / destructeur
				 FeedbackProvider () {}
		virtual ~FeedbackProvider (void) {}		

		//------------------------------------------ methodes
		virtual void 	Lookup 		(strPtr name) = 0;
		virtual void 	Connecting 	(char * name) = 0;
		virtual void 	CheckLatency (char * name) = 0;
		virtual void 	CloseReport (TMidiRemote * remote, CnxReportPtr report) = 0;
		virtual void 	Complete 	(TMidiRemote * remote) = 0;
		virtual void 	Remove 		(TMidiRemote * remote) = 0;
		virtual void 	CnxRefused	(strPtr host, short reason) = 0;
		virtual void 	Failed 		(short reason) = 0;
};


#endif
