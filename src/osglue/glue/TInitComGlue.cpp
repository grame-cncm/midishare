/*
  Copyright © Grame 2002

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
  
  modifications history:
 
*/

#include "TInitComGlue.h"
#include "TLog.h"

#ifdef WIN32
#	define ReadMsg(a,b) 	Read(&a, b)
#	define WriteMsg(a,b) 	Write(&a, b)
#else
#	define ReadMsg(a,from) 	Read(&a, sizeof(a), from)
#	define WriteMsg(a,to) 	Write(to, &a, sizeof(a))
	typedef struct{
		int	type;
		int	id;
	} ContactMsg, * ContactMsgPtr;
#endif

//_____________________________________________________________________
TInitComGlue::TInitComGlue (TLog * log)
	: fCommChan(log)
{
	fLog = log;
}

//_____________________________________________________________________
int TInitComGlue::Open (char *name) 
{
	return fCommChan.Open (kServerContactName);
}

//_____________________________________________________________________
int TInitComGlue::Create (char *name) 
{
	return fCommChan.Create (kServerContactName);
}

//_____________________________________________________________________
void TInitComGlue::Close ()
{
	fCommChan.Close();
}

//_____________________________________________________________________
int TInitComGlue::Write (int id, DestAddr to)
{
	ContactMsg msg;
	msg.type = kContactType;
	msg.id = id;
	return fCommChan.WriteMsg (msg, to);
}

//_____________________________________________________________________
int TInitComGlue::Read (FromHandler from)
{
	ContactMsg msg; int ret;
	ret = fCommChan.ReadMsg (msg, from);
	if (ret) {
		if (msg.type != kContactType) return kBadContactType;
		return msg.id;
	}
	return kReadContactFailed;
}
