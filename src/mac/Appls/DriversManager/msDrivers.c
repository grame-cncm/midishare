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


#include <Lists.h>
#include <Memory.h>
#include <QuickDraw.h>
#include <Types.h>
#include <Windows.h>

#include "MidiShare.h"
#include "msDrivers.h"
#include "msDrvAppl.h"

/* global variables	*/
Boolean 	chgDriver, chgCon;
short		myRefNum;

ListHandle 	theList[listSize];
short 		portSelected = -1;
char		PortsMap[32];
char		SelectedSlotMap[32];

/* local variables	*/
static char	InListMap[64];
static char	OutListMap[64];

/* -----------------------------------------------------------------------------*/
typedef void (* ApplySlotsProcPtr) (SlotRefNum sref, TSlotInfos * slotInfo, long refcon);
typedef struct {
	short 	port;
	Cell 	src;
	Cell 	dst;
} SlotsProcInfo;

static void ApplySlots (ApplySlotsProcPtr f, long refcon);


/* -----------------------------------------------------------------------------*/
/* internal functions implementation                                            */
/* -----------------------------------------------------------------------------*/
static void ResetPortMap ()
{
	short p;
	for (p = 0; p < 32; p++) { PortsMap[p] = 0; }
}

/* -----------------------------------------------------------------------------*/
static void ResetSlotsMap ()
{
	short p;
	for (p = 0; p < 32; p++) { SelectedSlotMap[p] = 0; }
}

/* -----------------------------------------------------------------------------*/
static void InvalPortArea ()
{
	short top = topPortArea(myWindow);
	Rect r;

	SetRect (&r, kPortLeft, top, kPortLeft + kPortArea, top + kPortArea);
	InvalRect (&r);
}

/* -----------------------------------------------------------------------------*/
static short ListIndexToDrvIndex (short index, ListHandle list)
{
	char * map = (list == theList[SrcL]) ? InListMap : OutListMap;
	short i = 1;
	for (i = 0; i < 64; i++, map++) {
		if (index < *map) return i+1;
	}
	return -1;
}

/* -----------------------------------------------------------------------------*/
static short ListIndexToSlotIndex (short index, ListHandle list)
{
	char * map = (list == theList[SrcL]) ? InListMap : OutListMap;
	short drvIndex = ListIndexToDrvIndex (index, list);
	if (drvIndex > 0) {
		short offset = (drvIndex > 1) ? map[drvIndex-2] : 0;
		return index - offset + 1;
	}
	return -1;
}

/* -----------------------------------------------------------------------------*/
static SlotRefNum GetIndSlot (short ref, short index, short direction)
{
	TDriverInfos drvInfo; TSlotInfos slotInfo; SlotRefNum sref;
	short i;
	
	if (index && MidiGetDriverInfos (ref, &drvInfo)) {
		for (i = 1; i <= drvInfo.slots; i++) {
			sref = MidiGetIndSlot (ref, i);
			if (MidiGetSlotInfos (sref, &slotInfo)) {
				if (slotInfo.direction & direction) {
					if (! --index)
						return sref;					
				}
			}
		}
	}
	sref.slotRef = -1;
	return sref;
}

/* -----------------------------------------------------------------------------*/
static void RefreshPortProc (SlotRefNum sref, TSlotInfos * slotInfo, long refcon)
{
	short i;
	for (i = 0; i < 32; i++) PortsMap[i] |= slotInfo->cnx[i];
 
}

/* -----------------------------------------------------------------------------*/
static void Cell2Slot (ListHandle list, Cell * c, SlotRefNum *sref)
{
	short i, ref;
	short direction =  (list == theList[SrcL]) ? MidiInputSlot : MidiOutputSlot;
	
	ref = MidiGetIndDriver (ListIndexToDrvIndex (c->v, list));
	if (ref < 0) 	sref->slotRef = -1;
	i = ListIndexToSlotIndex(c->v, list);
	if (i <= 0) 	sref->slotRef = -1;
	*sref = GetIndSlot (ref, i, direction);
}

/* -----------------------------------------------------------------------------*/
static SlotRefNum LastClickedSlot (ListHandle list, Cell * c)
{
	SlotRefNum sref;
	*c = LLastClick (list);
	Cell2Slot (list, c, &sref);
	return sref;
}

/* -----------------------------------------------------------------------------*/
static SlotRefNum LastSelectedSlot (ListHandle list, Cell * c)
{
	SlotRefNum sref;
	sref.slotRef = -1;
	c->h = c->v = 0;
	if (LGetSelect (true, c, list))
		Cell2Slot (list, c, &sref);
	return sref;
}

/* -----------------------------------------------------------------------------*/
static pascal void RcvAlarm (short refnum)
{
	MidiFlushEvs (refnum);
}

/* -----------------------------------------------------------------------------*/
static pascal void ApplAlarm (short refnum, long code)
{
	switch (code & 0xffff) {

        case MIDIOpenDriver:
        case MIDICloseDriver:       	
        case MIDIAddSlot:
        case MIDIRemoveSlot:
        case MIDIChgSlotName:
			chgDriver = true;
			portSelected = -1;
        	break;
        	
        case MIDIChgSlotConnect:
			chgCon = true;
			break;
	}
}

/* -----------------------------------------------------------------------------*/
static void ClearSelectInList (ListHandle list)
{
	Cell cell;
	cell.h = cell.v = 0;
	while (LGetSelect (true, &cell, list)) {
		LSetSelect (false, cell, list);
	}
}

/* -----------------------------------------------------------------------------*/
static void ClearSelections ()
{
	ClearSelectInList (theList[SrcL]);
	ClearSelectInList (theList[DstL]);
}

/* -----------------------------------------------------------------------------*/
static void ApplySlots (ApplySlotsProcPtr f, long refcon)
{
	TDriverInfos drvInfo; TSlotInfos slotInfo; SlotRefNum sref;
	short ref, i, j, n = MidiCountDrivers();

	for (i = 1; i <= n; i++) {
		ref = MidiGetIndDriver (i);
		if (MidiGetDriverInfos (ref, &drvInfo)) {
			for (j = 1; j <= drvInfo.slots; j++) {
				sref = MidiGetIndSlot (ref, j);
				if (MidiGetSlotInfos (sref, &slotInfo)) {
					f (sref, &slotInfo, refcon);
				}
			}
		}
	}
}

/* -----------------------------------------------------------------------------*/
static void ShowSrcDestProc (SlotRefNum sref, TSlotInfos * slotInfo, long refcon)
{
	SlotsProcInfo * info = (SlotsProcInfo *)refcon;
	Boolean cnx = IsAcceptedBit(slotInfo->cnx, info->port);
	if (slotInfo->direction & MidiInputSlot) {
		LSetSelect (cnx, info->src, theList[SrcL]);
		info->src.v++;
	}
	if (slotInfo->direction & MidiOutputSlot) {
		LSetSelect (cnx, info->dst, theList[DstL]);
		info->dst.v++;
	}
}

/* -----------------------------------------------------------------------------*/
static ListHandle GetSelectedSlot (Cell *c)
{
	c->h = c->v = 0;
	if (LGetSelect (true, c, theList[SrcL]))
		return theList[SrcL];
	c->h = c->v = 0;
	if (LGetSelect (true, c, theList[DstL]))
		return theList[DstL];
	return 0;
}

/* -----------------------------------------------------------------------------*/
static void ListMove (char direction)
{
	Cell c, next; ListHandle list = GetSelectedSlot (&c);
	if (!list) return;
	
	next = c;
	switch (direction) {
		case kUpArrowCharCode:		next.v -= 1;
			break;
		case kDownArrowCharCode:	
			if (!LNextCell(false, true, &next, list)) next.v = -1;
			break;
	}
	if (next.v >= 0) {
		LSetSelect (false, c, list);
		LSetSelect (true, next, list);
		LAutoScroll (list);
		ListPorts (list, false);
	} 
}

/* -----------------------------------------------------------------------------*/
/* external functions                                                           */
/* -----------------------------------------------------------------------------*/
Boolean SetupMidi(void)
{
	myRefNum = MidiOpen (ApplName);
	if (myRefNum == MIDIerrSpace) return false;

	chgDriver = chgCon = false;
	MidiSetApplAlarm (myRefNum, ApplAlarm);
	MidiSetRcvAlarm (myRefNum, RcvAlarm);
	return true;
}

/* -----------------------------------------------------------------------------*/
void CloseMSConnect (void)
{
	short i;
	MidiClose (myRefNum);
	for (i = 0; i < listSize; i++) {
		LDispose(theList[i]);
	}
}

/* -----------------------------------------------------------------------------*/
Boolean ChangeConnect (ListHandle list)
{
	Cell c;
	SlotRefNum sref = LastClickedSlot (list, &c);
	
	if (portSelected < 0) return false;	
	if (sref.slotRef < 0) return false;
	MidiConnectSlot (portSelected, sref, LGetSelect (false, &c, list));
	RefreshPortMap ();
	return true;
}

/* -----------------------------------------------------------------------------*/
void MoveSelection (char direction)
{
	short port = -1;
	if (portSelected < 0) {
		ListMove (direction);
		return;
	}
	
	switch (direction) {
		case kLeftArrowCharCode:	port = portSelected - 1;
			break;
		case kRightArrowCharCode:	port = portSelected + 1;
			break;
		case kUpArrowCharCode:		port = portSelected - 16;
			break;
		case kDownArrowCharCode:	port = portSelected + 16;
			break;
	}
	if ((port >= 0) && (port <= 255)) {
		portSelected = port;
		ShowSrcDest (port);
		InvalPortArea ();
	}
}

/* -----------------------------------------------------------------------------*/
void FillLists (void)
{
	TDriverInfos drvInfo; TSlotInfos slotInfo; SlotRefNum sref;
	Cell srcCell, dstCell; Boolean visible;
	short ref, i, j, p, n = MidiCountDrivers();
	
	srcCell.h = srcCell.v = dstCell.h = dstCell.v = 0;
	LDelRow(0, 0, theList[SrcL]);
	LDelRow(0, 0, theList[DstL]);
	ResetPortMap ();
	for (i = 1; i <= n; i++) {
		ref = MidiGetIndDriver (i);
		if (MidiGetDriverInfos (ref, &drvInfo)) {
			for (j = 1; j <= drvInfo.slots; j++) {
				sref = MidiGetIndSlot (ref, j);
				if (MidiGetSlotInfos (sref, &slotInfo)) {
					visible = false;
					if (slotInfo.direction & MidiInputSlot) {
						LAddRow(1, srcCell.v, theList[SrcL]);
						LSetCell(&slotInfo.name[1], slotInfo.name[0], srcCell, theList[SrcL]);
						srcCell.v++; visible = true;
					}
					if (slotInfo.direction & MidiOutputSlot) {
						LAddRow(1, dstCell.v, theList[DstL]);
						LSetCell(&slotInfo.name[1], slotInfo.name[0], dstCell, theList[DstL]);
						dstCell.v++; visible = true;
					}
					if (visible) {
						for (p = 0; p < 32; p++) PortsMap[p] |= slotInfo.cnx[p];
					}
				}
			}
			if (i <= 64) {
				InListMap[i-1] = srcCell.v;
				OutListMap[i-1] = dstCell.v;
			}
		}
	}
	InvalPortArea();
	LUpdate(myWindow->visRgn, theList[SrcL]);
	LUpdate(myWindow->visRgn, theList[DstL]);
}

/* -----------------------------------------------------------------------------*/
void SelectPort (short port, Boolean on)
{
	if (on) {
		portSelected = port;
		ResetSlotsMap ();
		ShowSrcDest (port);
	}
	else {
		ClearSelections ();
		portSelected = -1;
	}
	ShowEditMode (myWindow, on);
	InvalPortArea ();
}

/* -----------------------------------------------------------------------------*/
void ShowSrcDest (short port)
{
	SlotsProcInfo info;
	info.port = port;
	info.src.h = info.src.v = info.dst.h = info.dst.v = 0;
	ApplySlots (ShowSrcDestProc, (long)&info);
}

/* -----------------------------------------------------------------------------*/
Boolean ListPorts (ListHandle list, Boolean fromClick)
{
	TSlotInfos slotInfo; short i; Cell c;
	ListHandle alt = (list == theList[SrcL]) ? theList[DstL] : theList[SrcL];
	SlotRefNum sref = fromClick ? LastClickedSlot (list, &c) : LastSelectedSlot(list, &c);
	
	if (sref.slotRef < 0) goto reset;
	if (LGetSelect (false, &c, list)) {
		if (MidiGetSlotInfos (sref, &slotInfo)) {
			for (i = 0; i < 32; i++) {
				SelectedSlotMap[i] = slotInfo.cnx[i];
			}
		}
		ClearSelectInList (alt);
		InvalPortArea ();
		return true;
	}

reset:
	ClearSelectInList (alt);
	ResetSlotsMap ();
	InvalPortArea ();
	return false;
}

/* -----------------------------------------------------------------------------*/
short PointToDrvIndex (WindowPtr win, Point p, ListHandle list)
{
	Cell c; Rect r; short i;
	c.h = c.v = 0;
	for (i = 0; i < 64; i++, c.v++) {
		LRect (&r, c, list);
		if (PtInRect(p, &r)) {
			return ListIndexToDrvIndex (i, list);
		}
	}
	return -1;
}

/* -----------------------------------------------------------------------------*/
short PointToPort (WindowPtr win, Point p)
{
	short l, c, w = kPortWidth + kPortSep; 
	short top = topPortArea(win);
	Rect r;

	SetRect (&r, kPortLeft, top, kPortLeft + kPortArea, top + kPortArea);
	if (PtInRect (p, &r)) {
		p.h -= kPortLeft;
		p.v -= top;
		if (((p.v % w) < kPortWidth) && ((p.h % w) < kPortWidth)) {
			l = p.v / w;
			c = p.h / w;
			return (l * 16) + c;
		}
	}
	return -1;
}

/* -----------------------------------------------------------------------------*/
void RefreshPortMap ()
{
	ResetPortMap ();
	ApplySlots (RefreshPortProc, 0);
	InvalPortArea ();
}
