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
