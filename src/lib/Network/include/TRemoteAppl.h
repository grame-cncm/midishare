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


#ifndef __TMidiRemoteAppl__
#define __TMidiRemoteAppl__

#include "MidiShareAppl.h"
#include "StatesManager.h"

class TMidiRemote;
//____________________________________________________________
class TRemoteAppl : public MidiShareAppl
{
	public:
			 	 TRemoteAppl (TMidiRemote * owner);
		virtual ~TRemoteAppl () { if (fApplState) delete fApplState; }
	
		Boolean Open 	(MidiName name, Boolean wantFilter = false);
		void 	Close	();
		void RcvAlarm  (short refnum);
		void ApplAlarm (short refnum, short srcRefNum, short changeCode);	

	private:
		TMidiRemote * 	fOwner;
		StatesManager  	fStateManager;
		ApplState *		fApplState;	
};

#endif
