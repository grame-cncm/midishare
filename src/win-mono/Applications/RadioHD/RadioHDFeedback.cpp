/*

  Copyright © Grame 2001

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

#include <Windows.h>
#include <Wingdi.h>

#include "RadioHDFeedback.h"
#include "TMidiRemote.h"
#include "TInetAddress.h"
#include "MidiTools.h"

enum {
	kBoxW = 176,
	kFlashX = 20,	kFlashY = 35, 	kFlashW = 6,
	kLineX = 8,		kLineY = 176,	kLineH = 13,
	kBarX = 5,		kBarY = 252,
	kBarH = 31,		kBarW = 11,
	kBarSteps = 5,	kBarSpace = 1
};

#define	cwhite 	 RGB( 0xff, 0xff, 0xff )
#define	cred 	 RGB( 0xff, 0, 0 )
#define	cyellow  RGB( 0xff, 0xff, , 0 )
#define	chdcolor RGB( 0xef, 0xc9, 0x92 )
#define	cgreen 	 RGB( 0, 0xff, 0 )
#define	cblue 	 RGB( 0, 0, 0xff )
#define	cblack 	 RGB( 0, 0, 0 )

//_________________________________________________________________________________
static void MSALARMAPI dontSendUDP (short ref) { MidiFlushEvs (ref); }

//_________________________________________________________________________________
void TChanDTask::Run (long date, short refNum, long a1,long a2)
{
	RadioHDFeedback * f = (RadioHDFeedback *)a1;
	f->ChanTask ();
	Schedule (MidiGetTime() + a2, refNum, a1, a2);
}

//_________________________________________________________________________________
void TActivityDTask::Run (long date, short refNum, long a1,long a2)
{
	RadioHDFeedback * f = (RadioHDFeedback *)a1;
	f->ActivityTask ();
	Schedule (MidiGetTime() + a2, refNum, a1, a2);
}

//_________________________________________________________________________________
void TClockDTask::Run (long date, short refNum, long a1,long a2)
{
	RadioHDFeedback * f = (RadioHDFeedback *)a1;
	unsigned long currtime = MidiGetTime();
	f->ClockTask (currtime);
	Schedule (currtime + 1000, refNum, a1, a2);
}

//_________________________________________________________________________________
RadioHDFeedback::RadioHDFeedback (HWND win)
{
	short i;
	for (i = 0; i < kMaxText; i++)
		fText[i] = 0;
	for (i = 0; i < kChanCount; i++)
		fChanValues[i] = fCurChanValues[i] = 0;

	fWinH = win;
	fWinDC = GetDC (win);
	white	= CreateSolidBrush (RGB( 0xff, 0xff, 0xff ));
	red		= CreateSolidBrush (RGB( 0xff, 0, 0 ));
	yellow	= CreateSolidBrush (RGB( 0xff, 0xff, 0 ));
	hdcolor = CreateSolidBrush (RGB( 0xef, 0xc9, 0x92 ));
	green 	= CreateSolidBrush (RGB( 0, 0xff, 0 ));
	blue 	= CreateSolidBrush (RGB( 0, 0, 0xff ));
	black 	= CreateSolidBrush (RGB( 0, 0, 0 ));
	phdcolor= CreatePen (PS_SOLID, 1, chdcolor);

	fServer = 0;
	fActivityState = false;
	CurrentReset ();
	Open ("RDH-monitor");
	SetApplAlarm (true);
	fChanTask.Schedule (MidiGetTime() + kChanUpdateRate, 
					GetRefNum(), (long)this, kChanUpdateRate);
	fActivityTask.Schedule (MidiGetTime() + kActivityUpdateRate, 
					GetRefNum(), (long)this, kActivityUpdateRate);
}

//_________________________________________________________________________________
RadioHDFeedback::~RadioHDFeedback ()
{
}

//_________________________________________________________________________________
char *	RadioHDFeedback::MakeText (char * dst, char * t1, char * t2)
{
	wsprintf (dst, "%s%s", t1, t2);
	return dst;
}

//_________________________________________________________________________________
char *	RadioHDFeedback::Time2Text (char * dst, short sec, short min, short hrs)
{
	wsprintf (dst, "%02d:%02d:%02d", (int)hrs, (int)min, (int)sec); 
	return dst;
}

//_________________________________________________________________________________
void RadioHDFeedback::Lookup (strPtr name)
{
	static char dst[256];
	TextLine (0, MakeText (dst, "Looking for ", name), cgreen, 0);
}

//_________________________________________________________________________________
void RadioHDFeedback::Connecting (char * name)
{
	static char dst[256];
	TextLine (1, MakeText (dst, "Connecting to ", name), cgreen, 0);
}

//_________________________________________________________________________________
void RadioHDFeedback::CheckLatency (char * name) 
{ 
	TextLine (2, "Checking latency time... ", cgreen, 0);
}

//_________________________________________________________________________________
void RadioHDFeedback::Complete (TMidiRemote * remote) 
{ 
	extern char * radio_hd_server;
	fServer = remote;
	TextLine (3, "Connection complete. ", cgreen, 0);
	short ref = MidiGetNamedAppl (radio_hd_server);
	if (ref > 0) {
		MidiSetRcvAlarm (ref, dontSendUDP);
		MidiConnect (ref, 0, true);
	}
}

//_________________________________________________________________________________
void RadioHDFeedback::DoRedraw ()
{
	short i;
	for (i = 0; i < kMaxText; i++) {
		if (fText[i])
			TextLine (i, fText[i], fCurTextColor[i], fJustify[i]);
		else Clear (i);
	}
	for (i = 0; i < kChanCount; i++)
		ChannelActivity (i, fCurChanValues[i]);
	ActivityFlash (fActivityState);
	DrawLines ();
}

//_________________________________________________________________________________
void RadioHDFeedback::CurrentReset()
{
	fCurrent.title[0] = 0;
	fCurrent.from[0] = 0;
	fCurrent.clockOffset = 0;
	fCurrent.complete = fCurrent.end = false;
}

//_________________________________________________________________________________
void RadioHDFeedback::StartClock ()
{
	fClockTask.Schedule (MidiGetTime(), GetRefNum(), (long)this, 0);
}

//_________________________________________________________________________________
void RadioHDFeedback::ClockTask (unsigned long currtime)
{
	static char clkStr[256];
	unsigned long sec = (currtime - fCurrent.clockOffset) / 1000;
	unsigned long min = sec / 60;
	short hrs = min / 60;
	sec %= 60;
	min %= 60;
	Clear (3);
	TextLine (3, Time2Text(clkStr, sec, min, hrs), cyellow, kCenter);
}

//_________________________________________________________________________________
void RadioHDFeedback::ChanTask ()
{
	for (short i = 0; i < kChanCount; i++) {
		if (fCurChanValues[i]) {
			if (fCurChanValues[i] > kVelStep)
				fCurChanValues[i] -= kVelStep;
			else fCurChanValues[i] = 0;
			ChannelActivity (i, fCurChanValues[i]);
		}	
	}
}

//_________________________________________________________________________________
void RadioHDFeedback::ActivityTask ()
{
	if (fServer) {
		short over;
		if (fServer->InThroughput(&over)) {
			if (!fActivityState)
				ActivityFlash (fActivityState = true);
		}
		else if (fActivityState)
				ActivityFlash (fActivityState = false);
	}
}

//_________________________________________________________________________________
void RadioHDFeedback::DrawChans ()
{
	for (short i = 0; i < kChanCount; i++) {
		if (fChanValues[i] && (fChanValues[i] != fCurChanValues[i])) {
			ChannelActivity (i, fChanValues[i]);
			fCurChanValues[i] = fChanValues[i];
		}	
	}
}

//_________________________________________________________________________________
void RadioHDFeedback::Ev2Text (MidiEvPtr e, char * text)
{
	char * ptr = text; short i; short len;
//	*ptr++ = MidiCountFields (e);
	len = MidiCountFields (e);
	for (i = 0; i < len; i++)
		*ptr++ = MidiGetField (e, i);
	*ptr = 0;
}

//_________________________________________________________________________________
void RadioHDFeedback::ScanInput ()
{
	for (short i = 0; i < kChanCount; i++)
		fChanValues[i] = 0;

	char text[256]; MidiEvPtr e = GetEv();
	while (e) {
		switch (EvType(e)) {
			case typeNote:
			case typeKeyOn:
				if (Vel(e) > fChanValues [Chan(e)])
					fChanValues [Chan(e)] = Vel(e);
				break;
			case typeSeqName:
				Ev2Text (e, fCurrent.title);
				break;
			case typeCopyright:
				Ev2Text (e, text);
				MakeText (fCurrent.from, "from: ", text);
				break;
			case typeTempo:
				fCurrent.clockOffset = MidiGetTime() - Tempo(e);
				fCurrent.complete = true;
				break;
			case typeEndTrack:
				fClockTask.Forget ();
				fCurrent.end = true;
				break;
		}
		MidiFreeEv (e);
		e = GetEv();
	}
}

//_________________________________________________________________________________
void RadioHDFeedback::DoIdle ()
{
	long n = CountDTasks ();
	while (n--)
		Exec1DTask ();
	ScanInput ();
	DrawChans ();
	if (fCurrent.complete) {
		StartClock ();
		Clear ();
		TextLine (0, "You are listening to", cyellow, kCenter);
		TextLine (1, fCurrent.title, cyellow, kCenter);
		TextLine (2, fCurrent.from, cyellow, kCenter);
		fCurrent.complete = false;
	}
	else if (fCurrent.end) {
		Clear (0);
		TextLine (0, "You have listened to", cyellow, kCenter);
		fCurrent.end = false;
	}
}

//_________________________________________________________________________________
void RadioHDFeedback::Remove (TMidiRemote * remote)
{
	fServer = 0;
	fClockTask.Forget ();
	Clear ();
	TextLine (0, "____________________________", cred, kCenter);
	TextLine (1, "You have been disconnected !", cred, kCenter);
	TextLine (2, "Quit and try again.", cred, kCenter);
	TextLine (3, "____________________________", cred, kCenter);
}

//_________________________________________________________________________________
void RadioHDFeedback::Clear (short line)
{
	RECT box; 
	short bottom = kLineY + (kLineH * line);
	short x = kLineX - 3;
	SetRect (&box, x, bottom - kLineH+2, x + kBoxW-2, bottom + 2);
	FillRect (fWinDC, &box, black);
	fText[line] = 0;
}

//_________________________________________________________________________________
void RadioHDFeedback::Failed (short reason)
{
	extern Boolean doneFlag;
	doneFlag = true;
}

//_________________________________________________________________________________
void RadioHDFeedback::Clear ()
{
	short i;
	for (i = 0; i < kMaxText; i++) Clear (i);
}

//_________________________________________________________________________________
void RadioHDFeedback::TextLine (short line, char * text, COLORREF color, short justify)
{
	extern HFONT gFont;
	RECT r;
	short bottom = kLineY + (kLineH * line);
	short x = kLineX - 3;
	SetRect (&r, x, bottom - kLineH+2, x + kBoxW, bottom + 2);
	
	fText[line] = text;
	fCurTextColor[line] = color;
	fJustify[line] = justify;
	SetBkMode (fWinDC, TRANSPARENT);
	SetTextColor(fWinDC, color);
	SelectObject (fWinDC, gFont);
	DrawText (fWinDC, text, strlen (text), &r , DT_CENTER);
}

//_________________________________________________________________________________
void RadioHDFeedback::ActivityFlash (Boolean on)
{
	SelectObject(fWinDC, on ? red : black);
	Ellipse (fWinDC, kFlashX, kFlashY, kFlashX + kFlashW, kFlashY + kFlashW);
}

//_________________________________________________________________________________
void RadioHDFeedback::ChannelActivity (short chan, short level)
{
	RECT box;
	short i, x = kBarX + (kBarW * chan);
	SetRect (&box, x+1, kBarY - kBarH, x + kBarW - 1, kBarY);
	FillRect (fWinDC, &box, black);
	if (level) {
		HBRUSH brush = green;
		level /= kVelStep; 
		level += 1;
		SetRect (&box, x+1, kBarY - 4, x + kBarW - 1, kBarY);
		for (i = 0; i < level; i++) {
			FillRect (fWinDC, &box, brush);
			OffsetRect (&box, 0, -5);
			if (i > 3) brush = red;
		}
	}
}

//_________________________________________________________________________________
void RadioHDFeedback::DrawLines ()
{
	int i; short x = kBarX + kBarW;
	SelectObject (fWinDC, phdcolor);
	for (i=1; i<16; i++) {
		MoveToEx (fWinDC, x, kBarY, NULL);
		LineTo (fWinDC, x, kBarY - kBarH);
		x += kBarW;
	}
}

//_________________________________________________________________________________
void RadioHDFeedback::ApplAlarm (short refnum, short srcRefNum, short changeCode)
{
	extern char * radio_hd_server;
	if (changeCode == MIDIChgConnect) {
		short ref = MidiGetNamedAppl (radio_hd_server);
		if (ref > 0) {
			if (!MidiIsConnected (ref, GetRefNum())) {
				MidiConnect (ref, GetRefNum(), true);
			}
		}
	}
}
