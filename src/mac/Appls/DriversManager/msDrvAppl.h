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

#ifndef __msDrvAppl__
#define __msDrvAppl__

#include <Lists.h>
#include <Types.h>
#include <Windows.h>

/* input and output slots lists */
enum { SrcL, DstL, listSize };   

/* Application name	*/
#define ApplName 		"\pmsDrivers"
enum { 	kListWidth=125, kEdgeWidth=10, kPortWidth=8, kPortSep=3, 
		kPortArea= ((kPortWidth + kPortSep) * 15) + kPortWidth,
		kPortLeft = 5 + kListWidth + kEdgeWidth,
		kMinHeight = kPortArea + 60 };
#define topPortArea(win) (30 + (win->portRect.bottom - win->portRect.top - kPortArea - 40) / 2)


/* global variables	*/
extern WindowPtr	myWindow;
extern Boolean 		chgDriver, chgCon;

extern ListHandle	theList[listSize];
extern short 		portSelected;
extern char			PortsMap[32];
extern char			SelectedSlotMap[32];

static inline Boolean Busy (short port) { return IsAcceptedBit(PortsMap, port); }

void ShowEditMode (WindowPtr win, Boolean on);

#endif





	

