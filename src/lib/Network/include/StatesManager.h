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


#ifndef __StatesManager__
#define __StatesManager__

#include "InetTypes.h"
#include "MidiShare.h"
#include "Handle.h"

//____________________________________________________________
typedef struct CnxDesc CnxDesc, * CnxDescPtr;
struct CnxDesc {
	CnxDescPtr	next;
	short		refnum;
	MidiEvPtr 	name;
};

//____________________________________________________________
class RemoteState
{
	public:
			 	 RemoteState () {}
		virtual ~RemoteState () {}
	
		virtual INetHandle	GetState () = 0;
		virtual void 	SetState (INetHandle h) = 0;
		virtual void 	Alarm 	 (short refnum, short srcref, short changeCode) = 0;
		virtual long 	ID 	 	 () = 0;
};

//____________________________________________________________
class StatesManager
{
	public:
			 	 StatesManager () { fState = 0; }
		virtual ~StatesManager ();
	
		void SetState  (RemoteState * state)  { fState = state; }
		void SaveState (IPNum id);
		void LoadState (IPNum id);
				
	private:
		RemoteState * 	fState;
};

//____________________________________________________________
class ApplState : public RemoteState
{
	public:
			 	 ApplState (short refNum);
		virtual ~ApplState ();
	
		INetHandle	GetState ();
		void	SetState (INetHandle h);
		void 	Alarm 	 (short refnum, short srcref, short changeCode);
		long 	ID 	 	 ();
				
	private:
		void		ReadCnx (INetHandle h, CnxDescPtr *dest);
		void		WriteCnx (INetHandle h, CnxDescPtr cnx);

		void		RestoreState (short refNum);

		void		ChangeCnx (short refnum);

		void		GetName (CnxDescPtr cnx, MidiName name);
		void		SetName (CnxDescPtr cnx, MidiName name);
		CnxDescPtr	Find 	(MidiName name, CnxDescPtr cnx);
		CnxDescPtr	Find 	(short ref, CnxDescPtr cnx);
		
		Boolean		Equal 	(MidiName name, MidiEvPtr evName);
		void		AddCnx 	(short with, CnxDescPtr *dest);
		void		RmvCnx 	(short with, CnxDescPtr *dest);
		CnxDescPtr	NewCnx 	(short with);
		CnxDescPtr	NewCnx 	(INetHandle with);
		CnxDescPtr	NewCnx 	();
		void		FreeCnx (CnxDescPtr cnx);
		long		MemSize (CnxDescPtr list);
		short		Count 	(CnxDescPtr list);
		void		ClearCnxList (CnxDescPtr list);
		void		Reset ();

		Boolean 	fIgnoreAlarm;
		INetHandle 	fMemory;
		CnxDescPtr	fIn, fOut;
		short		fRefNum;
};

class TRemoteSlot;
//____________________________________________________________
class SlotState : public RemoteState
{
	public:
			 	 SlotState (TRemoteSlot * slots);
		virtual ~SlotState ();
	
		INetHandle	GetState ();
		void 	SetState (INetHandle h);
		void 	Alarm 	 (short refnum, short srcref, short changeCode);
		long 	ID 	 	 ();

	private:
		void RestoreSlot  (SlotRefNum slot, char *cnx);

		INetHandle 		fMemory;
		TRemoteSlot * 	fSlots;
};

#endif
