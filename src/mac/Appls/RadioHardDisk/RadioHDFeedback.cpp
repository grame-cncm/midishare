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

static RGBColor	white 	= { 0xffff, 0xffff, 0xffff };
static RGBColor	red 	= { 0xffff, 0, 0 };
static RGBColor	yellow 	= { 0xffff, 0xffff, 0 };
static RGBColor	hdcolor = { 0xef00, 0xc900, 0x9200 };
static RGBColor	green 	= { 0, 0xffff, 0 };
static RGBColor	blue 	= { 0, 0, 0xffff };
static RGBColor	black 	= { 0, 0, 0 };

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
RadioHDFeedback::RadioHDFeedback (WindowPtr win)
{
	short i;
	for (i = 0; i < kMaxText; i++)
		fText[i] = 0;
	for (i = 0; i < kChanCount; i++)
		fChanValues[i] = fCurChanValues[i] = 0;

	fWinH = win;
	fServer = 0;
	fActivityState = false;
	CurrentReset ();
	Open ("\pRDH-monitor");
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
unsigned char *	RadioHDFeedback::MakeText (Str255 dst, Str255 t1, Str255 t2)
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
unsigned char *	RadioHDFeedback::MakeText (Str255 dst, Str255 t1, char * t2)
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

//_________________________________________________________________________________
unsigned char *	RadioHDFeedback::Val2Text (Str255 dst, short val)
{
	if (val < 10) {
		dst[0] = 2;
		dst[1] = '0';
		dst[2] = '0' + val;
	}
	else NumToString (val, dst);
	return dst;
}

//_________________________________________________________________________________
unsigned char *	RadioHDFeedback::Time2Text (Str255 dst, short sec, short min, short hrs)
{
	Str255 str1, str2;
	Val2Text (str1, hrs);
	Val2Text (str2, min);
	str1[0]++; str2[0]++;
	str1[str1[0]] = str2[str2[0]] = ' ';
	MakeText (dst, str1, str2);
	Val2Text (str1, sec);
	MakeText (dst, dst, str1);
	return dst;
}

//_________________________________________________________________________________
void RadioHDFeedback::Lookup (strPtr name)
{
	static Str255 dst;
	TextLine (0, MakeText (dst, "\pLooking for ", name), &green, 0);
}

//_________________________________________________________________________________
void RadioHDFeedback::Connecting (char * name)
{
	static Str255 dst;
	TextLine (1, MakeText (dst, "\pConnecting to ", name), &green, 0);
}

//_________________________________________________________________________________
void RadioHDFeedback::CheckLatency (char * name) 
{ 
	TextLine (2, "\pChecking latency time... ", &green, 0);
}

//_________________________________________________________________________________
void RadioHDFeedback::Complete (TMidiRemote * remote) 
{ 
	extern unsigned char * radio_hd_server;
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
void RadioHDFeedback::Ev2Text (MidiEvPtr e, Str255 text)
{
	unsigned char * ptr = text; short i;
	*ptr++ = MidiCountFields (e);
	for (i = 0; i < text[0]; i++)
		*ptr++ = MidiGetField (e, i);
}

//_________________________________________________________________________________
void RadioHDFeedback::ScanInput ()
{
	for (short i = 0; i < kChanCount; i++)
		fChanValues[i] = 0;

	Str255 text; MidiEvPtr e = GetEv();
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
				MakeText (fCurrent.from, "\pfrom:", text);
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
	SetPort (fWinH);
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
	FillRect (&box, &qd.black);
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
void RadioHDFeedback::TextLine (short line, Str255 text, RGBColor * color, short justify)
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
	FillOval (&box, &qd.black);
}

//_________________________________________________________________________________
void RadioHDFeedback::ChannelActivity (short chan, short level)
{
	Rect box;
	short i, x = kBarX + (kBarW * chan);
	SetRect (&box, x+1, kBarY - kBarH, x + kBarW - 1, kBarY);
	RGBForeColor (&black);
	FillRect (&box, &qd.black);
	if (level) {
		RGBForeColor (&green);
		level /= kVelStep; 
		level += 1;
		SetRect (&box, x+1, kBarY - 4, x + kBarW - 1, kBarY);
		for (i = 0; i < level; i++) {
			FillRect (&box, &qd.black);
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
	extern unsigned char * radio_hd_server;
	if (changeCode == MIDIChgConnect) {
		short ref = MidiGetNamedAppl (radio_hd_server);
		if (ref > 0) {
			if (!MidiIsConnected (ref, GetRefNum())) {
				MidiConnect (ref, GetRefNum(), true);
			}
		}
	}
}
