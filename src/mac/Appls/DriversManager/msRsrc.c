/*

  Copyright © Grame 2000

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
  grame@rd.grame.fr

*/

#include <MacWindows.h>
#include <Memory.h>
#include <Resources.h>
#include <Types.h>

#include "msRsrc.h"

/* Constants definition */
enum { 	kWinPosRsrc = 'wipo', kWinposID = 128 };

//_______________________________________________________________________________
OSErr WriteRsrc	(Handle rsrc, ResType type, short ID, short	resFile)
{
	OSErr err; Handle h;
	short saved;
	
	saved = CurResFile();
	UseResFile( resFile);
	if ((err= ResError())!=noErr)	return err;
	
	h= GetResource (type, ID);
	if (h) RemoveResource (h);
	HLock (rsrc);
	AddResource( rsrc, type, ID, "\p");
	err= ResError();
	if( err== noErr) {
		UpdateResFile( resFile);
		err= ResError();
	}
	HUnlock( rsrc);
	UseResFile( saved);
	return err;
}

//_______________________________________________________________________________
Boolean ReadPos (Point *p)
{
	Handle h = GetResource (kWinPosRsrc, kWinposID);
	if (h) {
		*p = *((Point *)*h);
		ReleaseResource (h);
		return true;
	}
	return false;
}

//_______________________________________________________________________________
void WritePos (WindowPtr win, short	resFile)
{
	Handle h = NewHandle (sizeof(Point));
	if (h) {
		Point *p = (Point *)*h;
  		SetPort(win);
		p->h = win->portRect.left;
		p->v = win->portRect.top;
		LocalToGlobal(p);
		WriteRsrc (h, kWinPosRsrc, kWinposID, resFile);
	}
}

//_______________________________________________________________________________
void AdjustLocation (WindowPtr win)
{
	Point pt;
	
	pt.h = win->portRect.left + 6;	// 6 is enough to grab
	pt.v = win->portRect.top + 6;	// 6 is enough to grab
	LocalToGlobal(&pt);
	
	if (!PtInRgn( pt, GetGrayRgn())) {
		MoveWindow (win, qd.screenBits.bounds.left + 50,	qd.screenBits.bounds.top  + 50, false);
	}
}

	

