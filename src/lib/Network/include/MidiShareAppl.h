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


#ifndef __MidiShareAppl__
#define __MidiShareAppl__

#include "MidiShare.h"
#include "MidiShareFilter.h"
#include "MidiShareTasks.h"

typedef class MidiShareAppl * MidiShareApplPtr;

/*_______________________________________________________________________________*/
class MidiShareAppl
{
	public:
		enum { undefinedRefNum = -1 };
		
				 MidiShareAppl ();
		virtual ~MidiShareAppl ();

		virtual Boolean Open 	(MidiName name, Boolean wantFilter = false);
		virtual void 	Close	();
				Boolean Opened 	();

		virtual void SetApplAlarm 	(Boolean on);	
		virtual void SetRcvAlarm 	(Boolean on);	

		virtual void ApplAlarm 		(short refnum, short srcRefNum, short changeCode) {}	
		virtual void RcvAlarm 		(short refnum)	{ MidiFlushEvs (refnum); }
		virtual void Send 			(MidiEvPtr e)	{ MidiSend (fRefNum, e); }
		virtual MidiEvPtr GetEv 	()				{ return MidiGetEv (fRefNum); }

				long		 CountDTasks ()	{ return MidiCountDTasks (fRefNum); }
				void		 Exec1DTask ()	{ MidiExec1DTask (fRefNum); }
				short		 GetRefNum 	();
				void 		 SetName	(MidiName name);
		virtual MidiName     GetName	();
				MidiShareFilter * GetFilter ()	{ return &fFilter; }

	protected:
		short			fRefNum;
		MidiShareFilter	fFilter;
#if defined(macintosh) && defined(MidiSharePPC_68k)
		UPPRcvAlarmPtr	fUPPRcvAlarm;
		UPPApplAlarmPtr fUPPApplAlarm;
#endif
};

/*_______________________________________________________________________________*/
inline short 	MidiShareAppl::GetRefNum ()				{ return fRefNum; }
inline void 	MidiShareAppl::SetName (MidiName name)	{ MidiSetName (fRefNum, name); }
inline MidiName MidiShareAppl::GetName ()				{ return MidiGetName (fRefNum); }
inline Boolean 	MidiShareAppl::Opened ()				{ return fRefNum > 0; }

#endif

