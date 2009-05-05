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

#include <stdio.h>
#include <Carbon/Carbon.h>

#include "RadioHDFeedback.h"
#include "TMidiRemote.h"
#include "TInetAddress.h"
#include "MidiTools.h"

enum {
	kBoxW = 176,
	kFlashX = 21,	kFlashY = 36, 	kFlashW = 5,
	kLineX = 8,		kLineY = 174,	kLineH = 13,
	kBarX = 5,		kBarY = 253,
	kBarH = 30,		kBarW = 11,
	kBarSteps = 5,	kBarSpace = 1
};

//static RGBColor	white 	= { 0xffff, 0xffff, 0xffff };
static RGBColor	red 	= { 0xffff, 0, 0 };
static RGBColor	yellow 	= { 0xffff, 0xffff, 0 };
static RGBColor	hdcolor = { 0xef00, 0xc900, 0x9200 };
static RGBColor	green 	= { 0, 0xffff, 0 };
//static RGBColor	blue 	= { 0, 0, 0xffff };
static RGBColor	black 	= { 0, 0, 0 };
static Pattern gblack;

//_________________________________________________________________________________
static pascal void dontSendUDP (short ref) { MidiFlushEvs (ref); }

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
RadioHDFeedback::RadioHDFeedback (WindowRef win)
{
	short i;
	for (i = 0; i < kMaxText; i++)
		fText[i] = 0;
	for (i = 0; i < kChanCount; i++)
		fChanValues[i] = fCurChanValues[i] = 0;

	GetQDGlobalsBlack (&gblack);
	fWinH = win;
	fServer = 0;
	fActivityState = false;
	CurrentReset ();
	Open ("RDH-monitor");
	SetApplAlarm (true);
	fChanTask.Schedule (MidiGetTime() + kChanUpdateRate, 
					GetRefNum(), (long)this, kChanUpdateRate);
	fActivityTask.Schedule (MidiGetTime() + kActivityUpdateRate, 
					GetRefNum(), (long)this, kActivityUpdateRate);
#if defined(macintosh) && defined(MidiSharePPC_68k)
	fUPPRcvAlarm = NewRcvAlarmPtr(dontSendUDP);
#endif
}

//_________________________________________________________________________________
RadioHDFeedback::~RadioHDFeedback ()
{
#if defined(macintosh) && defined(MidiSharePPC_68k)
	if (fUPPRcvAlarm) DisposeRoutineDescriptor(fUPPRcvAlarm);
#endif
}

//_________________________________________________________________________________
unsigned char *	RadioHDFeedback::MakeText (Str255 dst, char * t1, char * t2)
{
	sprintf ((char *)&dst[1], "%s%s", t1, t2);
	dst[0] = strlen ((char *)&dst[1]);
	return dst;
}

//_________________________________________________________________________________
unsigned char *	RadioHDFeedback::Time2Text (Str255 dst, short sec, short min, short hrs)
{
	sprintf ((char *)&dst[1], "%02d %02d %02d", (int)hrs, (int)min, (int)sec);
	dst[0] = strlen ((char *)&dst[1]);
	return dst;
}

//_________________________________________________________________________________
void RadioHDFeedback::Lookup (strPtr name)
{
	static Str255 dst;
	TextLine (0, MakeText (dst, "Looking for ", name), &green, 0);
}

//_________________________________________________________________________________
void RadioHDFeedback::Connecting (char * name)
{
	static Str255 dst;
	TextLine (1, MakeText (dst, "Connecting to ", name), &green, 0);
}

//_________________________________________________________________________________
void RadioHDFeedback::CheckLatency (char * name) 
{ 
	TextLine (2, "\pChecking latency time... ", &green, 0);
}

//_________________________________________________________________________________
void RadioHDFeedback::Complete (TMidiRemote * remote) 
{ 
	extern char * radio_hd_server;
	fServer = remote;
	TextLine (3, "\pConnection complete. ", &green, 0);
	short ref = MidiGetNamedAppl (radio_hd_server);
	if (ref > 0) {
#if defined(macintosh) && defined(MidiSharePPC_68k)
		MidiSetRcvAlarm (ref, fUPPRcvAlarm);
#else
		MidiSetRcvAlarm (ref, dontSendUDP);
#endif
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
	for (short i = 0; i < kChanCount; i++)
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
	static Str255 clkStr;
	unsigned long sec = (currtime - fCurrent.clockOffset) / 1000;
	unsigned long min = sec / 60;
	short hrs = min / 60;
	sec %= 60;
	min %= 60;
	Clear (3);
	TextLine (3, Time2Text(clkStr, sec, min, hrs), &yellow, kCenter);
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
	char * ptr = text; short i, len = MidiCountFields(e); 
	for (i = 0; i < len; i++)
		*ptr++ = MidiGetField (e, i);
	*ptr = 0;
}

//_________________________________________________________________________________
void RadioHDFeedback::ScanInput ()
{
	for (short i = 0; i < kChanCount; i++)
		fChanValues[i] = 0;

	char text[255]; MidiEvPtr e = GetEv();
	while (e) {
		switch (EvType(e)) {
			case typeNote:
			case typeKeyOn:
				if (Vel(e) > fChanValues [Chan(e)])
					fChanValues [Chan(e)] = Vel(e);
				break;
			case typeSeqName:
				Ev2Text (e, (char *)&fCurrent.title[1]);
				fCurrent.title[0] = strlen ((char *)&fCurrent.title[1]);
				break;
			case typeCopyright:
				Ev2Text (e, text);
				sprintf ((char *)&fCurrent.from[1], "from:%s", text);
				fCurrent.from[0] = strlen ((char *)&fCurrent.from[1]);
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
	GrafPtr curPort;
	GetPort (&curPort);
	SetPort (GetWindowPort(fWinH));
	long n = CountDTasks ();
	while (n--)
		Exec1DTask ();
	ScanInput ();
	DrawChans ();
	if (fCurrent.complete) {
		StartClock ();
		Clear ();
		TextLine (0, "\pYou are listening to", &yellow, kCenter);
		TextLine (1, fCurrent.title, &yellow, kCenter);
		TextLine (2, fCurrent.from, &yellow, kCenter);
		fCurrent.complete = false;
	}
	else if (fCurrent.end) {
		Clear (0);
		TextLine (0, "\pYou have listened to", &yellow, kCenter);
		fCurrent.end = false;
	}
	SetPort (curPort);
}

//_________________________________________________________________________________
void RadioHDFeedback::Remove (TMidiRemote * remote)
{
	fServer = 0;
	fClockTask.Forget ();
	Clear ();
	TextLine (0, "\p____________________________", &red, kCenter);
	TextLine (1, "\pYou have been disconnected !", &red, kCenter);
	TextLine (2, "\pQuit and try again.", &red, kCenter);
	TextLine (3, "\p____________________________", &red, kCenter);
}

//_________________________________________________________________________________
void RadioHDFeedback::Clear (short line)
{
	Rect box; 
	short bottom = kLineY + (kLineH * line);
	short x = kLineX - 3;
	SetRect (&box, x, bottom - kLineH+2, x + kBoxW, bottom + 2);
	RGBForeColor (&black);
	FillRect (&box, &gblack);
	fText[line] = 0;
}

//_________________________________________________________________________________
void RadioHDFeedback::Failed (short reason)
{
	Clear ();
	TextLine (0, "\p___________________________", &red, kCenter);
	switch (reason) {
		case FeedbackProvider::kNotResponding:
			TextLine (1, "\pServer is not responding !", &red, kCenter);
			TextLine (2, "\pQuit and try again.", &red, kCenter);
			break;
		case FeedbackProvider::kCnxRefused:
			TextLine (1, "\pConnection has been refused !", &red, kCenter);
			TextLine (2, "\pTry again later please.", &red, kCenter);
			break;
		default:
			TextLine (1, "\pThe server is unreachable !", &red, kCenter);
			TextLine (2, "\pTry again later.", &red, kCenter);
	}
	TextLine (3, "\p___________________________", &red, kCenter);
}

//_________________________________________________________________________________
void RadioHDFeedback::Clear ()
{
	short i;
	for (i = 0; i < kMaxText; i++) Clear (i);
}

//_________________________________________________________________________________
void RadioHDFeedback::TextLine (short line, const Str255 text, RGBColor * color, short justify)
{
	short x = kLineX, y = kLineY + (kLineH * line);
	short w = TextWidth (text, 1 , text[0]);
	fText[line] = text;
	fCurTextColor[line] = color;
	fJustify[line] = justify;
	switch (justify) {
		case kCenter:
			x = (kBoxW > w) ? kLineX + (kBoxW - w) / 2 : kLineX;
			break;
		case kRight:
			x = (kBoxW > w) ? kLineX + kBoxW - w : kLineX;
			break;
	}
	MoveTo (x, y);
	RGBForeColor (color);
	DrawText (text, 1 , text[0]);
}

//_________________________________________________________________________________
void RadioHDFeedback::ActivityFlash (Boolean on)
{
	Rect box;
	SetRect (&box, kFlashX, kFlashY, kFlashX + kFlashW, kFlashY + kFlashW);
	RGBForeColor (on ? &red : &black);
	FillOval (&box, &gblack);
}

//_________________________________________________________________________________
void RadioHDFeedback::ChannelActivity (short chan, short level)
{
	Rect box;
	short i, x = kBarX + (kBarW * chan);
	SetRect (&box, x+1, kBarY - kBarH, x + kBarW - 1, kBarY);
	RGBForeColor (&black);
	FillRect (&box, &gblack);
	if (level) {
		RGBForeColor (&green);
		level /= kVelStep; 
		level += 1;
		SetRect (&box, x+1, kBarY - 4, x + kBarW - 1, kBarY);
		for (i = 0; i < level; i++) {
			FillRect (&box, &gblack);
			OffsetRect (&box, 0, -5);
			if (i > 3) RGBForeColor (&red);
		}
	}
}

//_________________________________________________________________________________
void RadioHDFeedback::DrawLines ()
{
	int i; short x = kBarX + kBarW;
	RGBForeColor (&hdcolor);
	for (i=1; i<16; i++) {
		MoveTo (x, kBarY);
		LineTo (x, kBarY - kBarH);
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
