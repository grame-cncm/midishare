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


#include "MacFeedback.h"
#include "TMidiRemote.h"
#include "TInetAddress.h"

enum { kIPID = 2, kMaxLatencyID, kGroupID, kCurLatencyID, kLatOver, 
	   kMissingID,  kInThroughput, kOutThroughput };
enum { kMainSteps = 17, kOverSteps = 1, kTotalSteps = kMainSteps +kOverSteps, 
		  kSepWidth = 1 };

static RGBColor	white 	= { 0xffff, 0xffff, 0xffff };
static RGBColor	red 	= { 0xffff, 0, 0 };
static RGBColor	green 	= { 0, 0xffff, 0 };
static RGBColor	black 	= { 0, 0, 0 };

//_________________________________________________________________________________
static unsigned char * C2P (char *str) 
{
	static Str255 text;
	char * ptr = (char *)&text[1];
	short n = 0;
	while (*str) {
		*ptr++ = *str++;
		n++;
	}
	text[0] = n;
	return text;
}

//_________________________________________________________________________________
MacFeedback::MacFeedback (WindowPtr * win, MenuHandle remote)
{
	fWinH = win;
	fSaved = fEnlarged = false;
	fRemoteMenu = remote;
	fRemoteList = 0;
}

//_________________________________________________________________________________
void MacFeedback::SaveLocation (DialogPtr dlog)
{ 
	if (!fSaved && dlog) {
		fSavedRect = dlog->portRect;
		fSaved = true;
	}
}

//_________________________________________________________________________________
void MacFeedback::Enlarge (WindowPtr win)
{ 
	if (!fEnlarged && win) {
		DrawDialog ((DialogPtr)win);
		SaveLocation ((DialogPtr)win);
		short newh = fSavedRect.bottom - fSavedRect.top + 26;
		SizeWindow (win, fSavedRect.right - fSavedRect.left, newh, true);
		fEnlarged = true;
	}
}

//_________________________________________________________________________________
void MacFeedback::SetText (DialogPtr dlog, short item, Str255 text)
{
	Handle h; Rect box; short type;
	if (dlog) {
		GetDialogItem(dlog, item, &type, &h, &box);
		SetDialogItemText (h, text);
		Draw1Control ((ControlHandle)h);
	}
}

//_________________________________________________________________________________
void MacFeedback::SetText (Str255 text)
{	
	if (!fEnlarged) Enlarge (*fWinH);
	SetText (*fWinH, kFeedbackItem, text);
}

//_________________________________________________________________________________
void MacFeedback::Lookup (strPtr name)
{
	Str255 dst;
	SetText (MakeText (dst, "\pLooking for ", name));
}

//_________________________________________________________________________________
void MacFeedback::Connecting (char * name)
{
	Str255 dst;
	SetText (MakeText (dst, "\pConnecting to ", name));
}

//_________________________________________________________________________________
void MacFeedback::CheckLatency (char * name) 
{ 
	SetText ("\pChecking latency time... ");
}

//_________________________________________________________________________________
void MacFeedback::Complete (TMidiRemote * remote) 
{ 
	if (fEnlarged) SetText ("\pConnection complete. ");
	InsertMenuItem (fRemoteMenu, remote->GetName(), 0);
	WANListPtr r = (WANListPtr)NewPtr (sizeof (WANList));
	if (r) {
		r->next = fRemoteList;
		r->remote = remote;
		r->win = 0;
		r->in.level = r->out.level = 0;
		r->in.overflow = r->out.overflow = 0;
		r->in.pic = r->out.pic = 0;
		fRemoteList = r;
	}
	Clear ();
}

//_________________________________________________________________________________
void MacFeedback::CompleteDebug (Str255 name) 
{ 
	static TMidiRemote * foo = 0;
	SetText ("\pConnection complete. ");
	InsertMenuItem (fRemoteMenu, name, 0);
	WANListPtr r = (WANListPtr)NewPtr (sizeof (WANList));
	short n = *name + 1;
	if (r) {
		r->next = fRemoteList;
		r->remote = foo++;
		r->win = 0;
		unsigned char * ptr = r->name;
		while (n--) *ptr++ = *name++;
		fRemoteList = r;
	}
}

//_________________________________________________________________________________
void MacFeedback::SetValue (DialogPtr dlog, short item, long value)
{
	Str255 text;
	if (!dlog) return;
	if (item == kIPID) {
		SetText (dlog, item, C2P(TInetAddress::IP2String (value)));
	}
	else {
		NumToString (value, text);
		SetText (dlog, item, text);
	}
}

//_________________________________________________________________________________
void MacFeedback::Throughput (Rect *r, short lev, short overflow)
{
	short i, h = (r->bottom - r->top) / kTotalSteps;
	Rect box;
	SetRect (&box, r->left, r->bottom - h + kSepWidth, r->right, r->bottom);
	if (overflow) lev = kMainSteps;
	RGBForeColor (&green);
	for (i=0; i<lev; i++) {
		FillRect (&box, &qd.black);
		OffsetRect (&box, 0, 0-h);
	}
	if (overflow > kOverSteps) overflow = kOverSteps;
	RGBForeColor (&red);
	box.top = r->top;
	if (overflow) {
		FillRect (&box, &qd.black);
	}
	else {
		box.top = r->top;
		RGBForeColor (&white);
		FillRect (&box, &qd.black);	
	}
	RGBForeColor (&black);
}

//_________________________________________________________________________________
void MacFeedback::Throughput (WANListPtr r)
{
	TMidiRemote * remote = r->remote;
	short overflow, mtu = remote->MTU();

	short lev = remote->InThroughput (&overflow) * kMainSteps;
	lev = (lev / mtu) + ((lev % mtu) ? 1 : 0);
	Throughput (&r->in.rect, lev, overflow);

	lev = remote->OutThroughput (&overflow) * kMainSteps;
	lev = (lev / mtu) + ((lev % mtu) ? 1 : 0);
	Throughput (&r->out.rect, lev, overflow);
}

//_________________________________________________________________________________
void MacFeedback::DoIdle (WANListPtr r)
{
	if (r->win) {
		long t = TickCount();
		TMidiRemote * remote = r->remote;
		unsigned long missed = remote->Missing();
		short latency = remote->CurLatency ();
		short over = remote->MaxLatencyOver ();
		if (r->missed != missed) {
	 		SetValue (r->win, kMissingID, missed);
	 		r->missed = missed;
		}
		if (r->latency != latency) {
	 		SetValue (r->win, kCurLatencyID, latency);
	 		r->latency = latency;
		}
		if (r->latOver != over) {
	 		SetValue (r->win, kLatOver, over);
	 		r->latOver = over;
		}
		if (t > r->nextUpdate) {
			SetPort (r->win);
			Throughput (r);
			r->nextUpdate = t + 15;
		}
	}
}

//_________________________________________________________________________________
void MacFeedback::DoIdle ()
{
	WANListPtr remote = fRemoteList;
	while (remote) {
		if (remote->win) DoIdle(remote);
		remote = remote->next;
	}
}

//_________________________________________________________________________________
void MacFeedback::CloseReport (TMidiRemote * remote, CnxReportPtr report)
{
}

//_________________________________________________________________________________
void MacFeedback::Disconnect (WANListPtr r)
{
	if (r) Remove (r->remote);
}

//_________________________________________________________________________________
void MacFeedback::CloseInfos (long Wrefcon)
{
	WANListPtr ptr = (WANListPtr)Wrefcon;
	if (ptr && ptr->win) {
		DisposeWindow (ptr->win);
		ptr->win = 0;
	}
}

//_________________________________________________________________________________
void MacFeedback::Infos (short index)
{
	WANListPtr prev, ptr = Find (index, &prev);
	if (ptr) {
		if (ptr->win) SelectWindow (ptr->win);
		else {
			TMidiRemote * remote = ptr->remote; short itemType; Handle h;
			if (!remote)  return;

		 	ptr->win = GetNewDialog (kRemoteWinID, nil, (WindowPtr) -1);
			GetDialogItem(ptr->win, kInThroughput, &itemType, &h, &ptr->in.rect);
			ptr->in.pic = (PicHandle)h;
			InsetRect (&ptr->in.rect, 1, 1);
			GetDialogItem(ptr->win, kOutThroughput, &itemType, &h, &ptr->out.rect);
			ptr->out.pic = (PicHandle)h;
			InsetRect (&ptr->out.rect, 1, 1);
		 	SetWTitle (ptr->win, remote->GetName ());
		 	SetPort (ptr->win);
		 	TextFont (kFontIDGeneva); TextSize (10);
		 	SetWRefCon (ptr->win, (long)ptr);
		 	ptr->missed = remote->Missing ();
		 	ptr->latency = remote->CurLatency ();
		 	ptr->latOver = remote->MaxLatencyOver ();
		 	ptr->nextUpdate = 0;
		 	SetValue (ptr->win, kIPID, remote->GetID ());
		 	SetValue (ptr->win, kMaxLatencyID, remote->MaxLatency ());
		 	SetValue (ptr->win, kGroupID, remote->GroupTime ());
		 	SetValue (ptr->win, kCurLatencyID, ptr->latency);
		 	SetValue (ptr->win, kLatOver, ptr->latOver);
		 	SetValue (ptr->win, kMissingID, ptr->missed);
			ShowWindow (ptr->win);
		}
	}
}

//_________________________________________________________________________________
void MacFeedback::Remove (WANListPtr item, WANListPtr prev)
{
	if (prev) prev->next = item->next;
	else fRemoteList = item->next;
	DisposePtr((Ptr)item);
}

//_________________________________________________________________________________
WANListPtr MacFeedback::Find (TMidiRemote * item, WANListPtr *prev, short *index)
{
	*prev = 0; *index = 1;
	WANListPtr ptr = fRemoteList;
	while (ptr) {
		if (ptr->remote == item)
			return ptr;
		*prev = ptr;
		ptr = ptr->next;
		*index += 1;
	}
	return 0;
}

//_________________________________________________________________________________
WANListPtr MacFeedback::Find (short index, WANListPtr *prev)
{
	*prev = 0;
	WANListPtr ptr = fRemoteList;
	while (index-- && ptr)
		ptr = ptr->next;
	return ptr;
}

//_________________________________________________________________________________
void MacFeedback::Remove (TMidiRemote * remote)
{
	short index;
	WANListPtr prev, ptr = Find (remote, &prev, &index);
	if (ptr) {
		if (ptr->win) DisposeWindow (ptr->win);
		DeleteMenuItem (fRemoteMenu, index);
		Remove (ptr, prev);
	}
}

//_________________________________________________________________________________
void MacFeedback::Clear ()
{
	DialogPtr dlog = *fWinH; Rect r = { 0 };
	if (fSaved && dlog && fEnlarged) {
		SetText ("\p");
		SizeWindow (*fWinH, fSavedRect.right - fSavedRect.left, 
							fSavedRect.bottom - fSavedRect.top, true);
		fEnlarged = false;
	}
}

//_________________________________________________________________________________
unsigned char *	MacFeedback::MakeText (Str255 dst, Str255 t1, Str255 t2)
{
	short n1 = *t1++, n2 = *t2++;
	unsigned char * ptr = dst;
	if (n1 + n2 > 255) n2 = 255 - n1;
	*ptr++ = n1 + n2;
	while (n1--) *ptr++ = *t1++;
	while (n2--) *ptr++ = *t2++;	
	return dst;
}

//_________________________________________________________________________________
unsigned char *	MacFeedback::MakeText (Str255 dst, Str255 t1, char * t2)
{
	short n1 = *t1++, n2 = 0, max = 255 - n1;
	unsigned char * ptr = dst;
	*ptr++ = n1;
	while (n1--) *ptr++ = *t1++;
	while (*t2 && (n2 < max)) {
		*ptr++ = *t2++;
		n2++;
	}
	dst[0] += n2;
	return dst;
}
