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
  grame@grame.fr

*/

#include "MidiShare.h"
#include "msDisplay.h"

/* -----------------------------------------------------------------------------*/
#define kMaxDisplay		21

short myRefNum = 0;
unsigned char * evNames[256];
RgnHandle 	scrollRgn;
Rect 		scrollRect;
GrafPort	myPort;
FontInfo	myFInfo;

extern WindowPtr myWindow;

/* -----------------------------------------------------------------------------*/
static void InitEvNames (unsigned char *tbl[])
{
	short i;
	for (i = typePrivate; i <= typeDead; i++)
		tbl[i] = 	"\p Undef  ";
	tbl[typeNote] 		= "\pNote   ";
	tbl[typeKeyOn] 		= "\pKeyOn  ";
	tbl[typeKeyOff] 	= "\pKeyOff ";
	tbl[typeKeyPress] 	= "\pKPress ";
	tbl[typeCtrlChange] = "\pCtrlCh ";
	tbl[typeProgChange] = "\pProgCh ";
	tbl[typeChanPress] 	= "\pCPress ";
	tbl[typePitchWheel] = "\pPWheel ";
	tbl[typeSongPos] 	= "\pSgPos  ";
	tbl[typeSongSel] 	= "\pSgSel  ";
	tbl[typeClock] 		= "\pClock  ";
	tbl[typeStart]	 	= "\pStart  ";
	tbl[typeContinue]	= "\pCont   ";
	tbl[typeStop]	 	= "\pStop   ";
	tbl[typeTune] 		= "\pTune   ";
	tbl[typeActiveSens] = "\pASens  ";
	tbl[typeReset]	 	= "\pReset  ";
	tbl[typeSysEx]	 	= "\pSysEx  ";
	tbl[typeStream] 	= "\pStream ";
	tbl[typeQuarterFrame]= "\pQFrame ";

	tbl[typeCtrl14b] 	= "\pCtrl14 ";
	tbl[typeNonRegParam]= "\pNRgPar ";
	tbl[typeRegParam] 	= "\pRegPar ";

	tbl[typeSeqNum] 	= "\pSqNum  ";
	tbl[typeTextual] 	= "\pText   ";
	tbl[typeCopyright] 	= "\p(©)    ";
	tbl[typeSeqName] 	= "\pSqName ";
	tbl[typeInstrName] 	= "\pInstr  ";
	tbl[typeLyric] 		= "\pLyric  ";
	tbl[typeMarker] 	= "\pMarker ";
	tbl[typeCuePoint] 	= "\pCuePt  ";
	tbl[typeChanPrefix] = "\pChPrfx ";
	tbl[typeEndTrack] 	= "\pEndTrk ";
	tbl[typeTempo] 		= "\pTempo  ";
	tbl[typeSMPTEOffset] = "\pSMPTE  ";

	tbl[typeTimeSign] 	= "\pTimeSg ";
	tbl[typeKeySign] 	= "\pKeySg  ";
	tbl[typeSpecific] 	= "\pSpecif ";

	tbl[typeDead] 		= "\pDead   ";
}

/* -----------------------------------------------------------------------------*/
static void OffScreenInit ()
{
	SetRect(&scrollRect,11, 24, 274, 278);
	scrollRgn = NewRgn ();
	OpenPort(&myPort);
	myPort.portBits.baseAddr = NewPtr(40*40*8);
	myPort.portBits.rowBytes = 40;
	myPort.portBits.bounds = scrollRect;
	SetPort (&myPort);
	TextFont (kFontIDMonaco);  TextSize(9);
	TextMode (srcCopy); PenNormal();
	EraseRect (&scrollRect);
	MoveTo(15,21);
	GetFontInfo (&myFInfo);
}

/* -----------------------------------------------------------------------------*/
static void NewLine ()
{
	Point p; short d;
	GetPen (&p);
	d = p.v+12+myFInfo.descent+myFInfo.leading;
	if (d > scrollRect.bottom) {
		ScrollRect (&scrollRect, 0, -12, scrollRgn);
		MoveTo (15, p.v);
	} 
	else {
		MoveTo (15, p.v + 12);
	}
}

/* -----------------------------------------------------------------------------*/
static void TimeToString (unsigned long t, Str255 s)
{
	s[12] = '0' + (t % 10); t /= 10;
	s[11] = '0' + (t % 10); t /= 10;
	s[10] = '0' + (t % 10); t /= 10;
	
	s[8] = '0' + (t % 10); t /= 10;
	s[7] = '0' + (t % 6); 	t /= 6;
	
	s[5] = '0' + (t % 10); t /= 10;
	s[4] = '0' + (t % 6); 	t /= 6;
	
	s[2] = '0' + (t % 10); t /= 10;
	s[1] = '0' + (t % 10); t /= 10;
	s[9] = s[3] = s[6] =':';
	s[13] = ' ';
	s[0] = 13;
}

/* -----------------------------------------------------------------------------*/
static void Append (Str255 to, Str255 str, char after)
{
	unsigned char * src, *dst;
	short i, n = str[0];
	short offset = to[0]+1;
	to[0] += n;
	src = &str[1];
	dst = &to[offset];
	for (i=0; i<n; i++) {
		*dst++ = *src++;
	}
	if (after) {
		*dst = after;
		to[0] += 1;
	}
}

#define AppendChar(str,c)	str[0]+=1; str[str[0]]=c
/* -----------------------------------------------------------------------------*/
static void EvToString (MidiEvPtr e, Str255 str)
{
	char c1, c2; Str255 tmp; unsigned char * fill; long n, i, pos;
	
	if ((EvType(e) >= typeText) && (EvType(e) <= typeCuePoint)) {
		c1 = c2 = '"';
	} else {
		c1 = '(';
		c2 = ')';
	}
	TimeToString (Date(e), str);
	NumToString (Port(e), tmp);
	fill = (Port(e) < 10) ? "\p  " : (Port(e) < 100) ? "\p " : "\p";
	Append (str, fill, 0);
	Append (str, tmp, '/');

	NumToString (Chan(e), tmp);
	Append (str, tmp, ' ');
	if (Chan(e) < 10) AppendChar(str,' ');
	Append (str, evNames[EvType(e)], c1);
	
	n = MidiCountFields(e)-1;
	pos = str[0] + 1;
	if ((EvType(e) >= typeText) && (EvType(e) <= typeCuePoint)) {
		for (i=0; (i<=n) && (pos<128); i++) {
			char c = MidiGetField (e, i);
			str[pos++] = (c < 32) ? 'É' : c;
			str[0]+= 1;
		}
	} 
	else {
		for (i=0; i<=n; i++) {
			char c = (i<n) ? ' ' : 0;
			NumToString (MidiGetField (e, i), tmp);
			if ((128 - str[0]) < tmp[0]) break;
			Append (str, tmp, c);
		}
	}
	AppendChar(str,c2);
}

/* -----------------------------------------------------------------------------*/
Boolean SetupMidi (Str255 applName)
{
	InitEvNames (evNames);
	OffScreenInit ();
	myRefNum = MidiOpen (applName);
	if (myRefNum == MIDIerrSpace) return false;
	MidiConnect (0, myRefNum, true);
	return true;
}

/* -----------------------------------------------------------------------------*/
void CloseMidi ()
{
	if (myRefNum > 0) MidiClose (myRefNum);
	myRefNum = 0;
	DisposeRgn (scrollRgn);
}

#define DrawMyContent()  CopyBits (&myPort.portBits, &(myWindow->portBits), &scrollRect, &scrollRect, srcCopy, 0)

/* -----------------------------------------------------------------------------*/
void DoIdle ()
{
	MidiEvPtr e; Str255 str;
	long i, max, n = MidiCountEvs(myRefNum);
	
	while (n > kMaxDisplay) {
		MidiFreeEv (MidiGetEv(myRefNum));
		n--;
	}
	if (n > 0) {
		max  = (n > 4) ? 4 : n;
		SetPort (&myPort);
		for (i=0; i<max; i++) {
			e = MidiGetEv(myRefNum);
			EvToString (e, str);
			MidiFreeEv (e);
			NewLine ();
			DrawString (str);
		}								
		SetPort(myWindow);
		DrawMyContent();
	}
}
