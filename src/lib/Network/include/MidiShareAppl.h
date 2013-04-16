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

		virtual bool Open 	(MidiName name, bool wantFilter = false);
		virtual void 	Close	();
				bool Opened 	();

		virtual void SetApplAlarm 	(bool on);	
		virtual void SetRcvAlarm 	(bool on);	

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
inline bool 	MidiShareAppl::Opened ()				{ return fRefNum > 0; }

#endif

