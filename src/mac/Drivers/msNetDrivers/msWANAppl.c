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


#include <Dialogs.h>
#include <Menus.h>
#include <TextEdit.h>
#include <Types.h>
#include <Windows.h>

#include "msInetDriverAppl.h"
#include "msWANAppl.h"
#include "MacFeedback.h"

/* rsrc constants */
enum { kConnect=1, kCancelCnct, kAddress=4 };

/* dialog & alerts */
enum { getNameID = 130};

/* global variables	*/
#ifndef CustomRadio
MacFeedback * gFeedback;
#endif

/* -----------------------------------------------------------------------------*/
static Boolean GetRemoteName (Str255 name, ModalFilterUPP filterProc)
{
	DialogPtr dialog; short item, iType;
	Handle iHandle; Rect r; Boolean ret = false;

 	dialog = GetNewDialog (getNameID, nil, (WindowPtr) -1);
 	if (dialog) {
		do {
			SelectDialogItemText (dialog, kAddress, 0, 32000);
			ModalDialog (filterProc, &item);
			if( item == kConnect) {
				GetDialogItem (dialog, kAddress, &iType, &iHandle, &r);
				GetDialogItemText (iHandle, name);
				ret = true;
			}
			else break;
		} while (!ret);
		DisposeDialog(dialog);
	}
	return ret;
}
		
/* -----------------------------------------------------------------------------*/
void OpenRemote (ModalFilterUPP filterProc)
{
	Str255 name;
	if (GetRemoteName (name, filterProc) && name[0]) {
		Connect (name);
	}
}
		
/* -----------------------------------------------------------------------------*/
void CloseRemote (WindowPtr win)
{
	WANListPtr r = (WANListPtr)GetWRefCon (win);
	if (r) {
		Disconnect ((long)r->remote);
//		gFeedback->Disconnect (r);
	}
}
