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
  
*/
#include <stdio.h>

#include "StreamToEvent.h"
#include "StreamDefs.h"
#include "msFunctions.h"

#ifdef WIN32
enum { false, true };
#endif

#define StreamCountAvail(f) (StreamLength(f->buff) - f->read)
#define StreamCountCont(f)  (StreamLength(f->buff) - f->read)
#define StreamAdjust(f, ptr, size) \
	f->read += size; \
	f->loc = ptr

/*===========================================================================
	internal functions prototypes
  =========================================================================== */
static int     UnknownParseMth  (msStreamBufferPtr f, MidiEvPtr e);
static int     Data4ParseMth    (msStreamBufferPtr f, MidiEvPtr e);
static int     Ext1ParseMth     (msStreamBufferPtr f, MidiEvPtr e);
static int     VarLenParseMth   (msStreamBufferPtr f, MidiEvPtr e);
static int     VarLenContMth    (msStreamBufferPtr f, MidiEvPtr e);
static MidiEvPtr StartReadBuffer(msStreamBufferPtr f, int * retcode);
static MidiEvPtr ReadNewEvent   (msStreamBufferPtr f, int * retcode);

/*===========================================================================
	external initialization functions
  =========================================================================== */
/*____________________________________________________________________________*/
void msStreamParseInit  (msStreamBufferPtr f, msStreamParseMethodTbl methTbl,
							void * buffer, unsigned short size)
{
  	f->buff = buffer;
	f->size = size;
	f->parse= methTbl;
	f->varLen= 0;
	f->curEv = 0;
	msStreamParseRewind(f);
}

/*____________________________________________________________________________*/
void msStreamParseReset (msStreamBufferPtr f)
{
	msStreamParseRewind(f);
	f->varLen = 0;
	if (f->curEv) MidiFreeEv(f->curEv);
	f->curEv = 0;
}

/*____________________________________________________________________________*/
void msStreamParseInitMthTbl (msStreamParseMethodTbl tbl)
{
	int i;
	for (i=0; i<256; i++) tbl[i] = UnknownParseMth;

	for (i=typeNote;i<=typeSongSel;i++)     tbl[i]= Data4ParseMth;

	tbl[typeSysEx]        = VarLenParseMth;
	tbl[typeStream]       = VarLenParseMth;
	
	for (i=typeClock;i<=typeReset;i++)          tbl[i]= Data4ParseMth;
	for (i=typePrivate;i<=typeDProcess;i++)     tbl[i]= Ext1ParseMth;
	for (i=typeQuarterFrame;i<=typeSeqNum;i++)  tbl[i]= Data4ParseMth;
	for (i=typeTextual;i<=typeCuePoint ;i++)    tbl[i]= VarLenParseMth;
	for (i=typeChanPrefix;i<=typeKeySign;i++)   tbl[i]= Data4ParseMth;

	tbl[typeSpecific]      = VarLenParseMth;
	tbl[typePortPrefix]    = Data4ParseMth;
	
	tbl[typeMidiOpen]      = VarLenParseMth;
	for (i=typeMidiOpenRes;i<=typeMidiConnect;i++) tbl[i]= Data4ParseMth;
	tbl[typeMidiSetName]   = VarLenParseMth;
	tbl[typeMidiSetInfo]   = Data4ParseMth;
#ifdef WIN32
    tbl[typeMidiSetFilter] = VarLenParseMth;
#else
    tbl[typeMidiSetFilter] = Data4ParseMth;
#endif
}

/*____________________________________________________________________________*/
static int CheckConsistency (msStreamHeaderPtr h, MidiEvPtr e)
{
	/* check for header magic value */
	if (h->magic != kStreamMagic) return kStreamInvalidHeader;
	/* check for continuation and params consistency */
	if ((h->cont && !e) || (!h->cont && e)) return kStreamInvalidParameter;
	return kStreamNoError;
}

/*____________________________________________________________________________*/
static MidiEvPtr StartReadBuffer (msStreamBufferPtr f, int * retcode)
{
	msStreamHeaderPtr h = (msStreamHeaderPtr)f->buff;
	MidiEvPtr e = f->curEv;

	if ((!h->cont && e) || (h->cont && !e)) {
		*retcode = kStreamParseError;
		return 0;
	}
	StreamAdjust(f, ++h, sizeof(msStreamHeader));
	if (e) {
		*retcode = f->parse[EvType(e)](f, e);
		switch (*retcode) {
			case kStreamNoError:
				f->curEv = 0;
				return e;
			case kStreamNoMoreData:
				f->curEv = e;
				break;
			default:
				MidiFreeEv(e);
		}
	}
	else return ReadNewEvent (f, retcode);
	return 0;
}

/*____________________________________________________________________________*/
static MidiEvPtr ReadNewEvent (msStreamBufferPtr f, int * retcode)
{
	if (StreamCountAvail(f) >= sizeof(EvCommonPart)) {
		EvCommonPartPtr common = (EvCommonPartPtr)f->loc;
		MidiEvPtr e = MidiNewEv(common->u.common.evType);
		if (!e) {
			*retcode = kStreamMemoryError;
			return 0;
		}
		else {
			EvCommonPartPtr ec = EventCommonPart(e);
			Link(e) = 0;
			*ec = *common++;
			StreamAdjust(f, common, sizeof(EvCommonPart));
			*retcode = f->parse[EvType(e)](f, e);
            if (!StreamCountAvail(f)) {
                msStreamParseRewind(f);
            }
			switch (*retcode) {
				case kStreamNoError: 
					f->curEv = 0;
					return e;
				case kStreamNoMoreData:
					f->curEv = e;
					break;
				default:
					MidiFreeEv(e);
			}
		}
	}
	else {
		msStreamParseRewind(f);
		*retcode = kStreamNoMoreData;
	}
	return 0;
}

/*____________________________________________________________________________*/
MidiEvPtr msStreamGetEvent (msStreamBufferPtr f, int * retcode)
{
	if (!retcode) {
		*retcode = kStreamInvalidParameter;
		return 0;
	}
	if (!f->read)
		return StartReadBuffer (f, retcode);
	else 
		return ReadNewEvent (f, retcode);
}

/*===========================================================================
			internal functions implementation
  =========================================================================== */
static int UnknownParseMth (msStreamBufferPtr f, MidiEvPtr e)
{
    msStreamParseReset (f);
	return kStreamUnknowParseMeth;
}

/*____________________________________________________________________________*/
static int Data4ParseMth (msStreamBufferPtr f, MidiEvPtr e)
{
	if (StreamCountAvail(f) >= sizeof(long)) {
		long * ptr = (long *)f->loc;
		long * data = (long *)EventData(e);
		*data = *ptr++;
		StreamAdjust(f, ptr, sizeof(long));
		return kStreamNoError;
	}
	msStreamParseRewind(f);
	return kStreamNoMoreData;
}

/*____________________________________________________________________________*/
static int Ext1ParseMth (msStreamBufferPtr f, MidiEvPtr e)
{
	if (StreamCountAvail(f) >= (sizeof(long) * 4)) {
		long * ptr = (long *)f->loc;
		long * data = (long *)LinkST(e);
		int i;
		for (i = 0; i < 4; i++)
			*data++ = *ptr++;
		StreamAdjust(f, ptr, (sizeof(long) * 4));
		return kStreamNoError;
	}
	msStreamParseRewind(f);
	return kStreamNoMoreData;
}

/*____________________________________________________________________________*/
static int VarLenParseMth (msStreamBufferPtr f, MidiEvPtr e)
{
	if (StreamCountAvail(f) >= sizeof(long)) {
		long * ptr = (long *)f->loc;
		f->varLen = *ptr++;
		StreamAdjust(f, ptr, sizeof(long));
		return VarLenContMth (f, e);
	}
	msStreamParseRewind(f);
	return kStreamNoMoreData;
}

/*____________________________________________________________________________*/
static int VarLenContMth (msStreamBufferPtr f, MidiEvPtr e)
{
	long *ptr = (long *)f->loc;
	
	while ((StreamCountAvail(f) >= VLCellSize) && f->varLen) {
		if (f->varLen >= VLCellSize) {
			MidiSEXPtr next = (MidiSEXPtr)MidiNewCell();

			if (!next) return kStreamMemoryError;
			else {
				MidiSEXPtr end = LinkSE(e);
				long *data = (long *)next->data;
				int i;
				for (i = 0; i < 3; i++)
					*data++ = *ptr++;
				StreamAdjust(f, ptr, VLCellSize);
				f->varLen -= VLCellSize;
				/* link the new cell to the end of the event */
				Link(next) = Link(end);
				LinkSE(e)  = next;
				Link(end)  = next;
			}
		}
		else return kStreamParseError;
	}
	if (f->varLen) {
		f->parse[EvType(e)] = VarLenContMth;
		msStreamParseRewind(f);
		return kStreamNoMoreData;
	}
	else {
		/* removes the first cell which hasn't been filled */
		MidiSEXPtr end = LinkSE(e);
		MidiSEXPtr first = Link(end);
		Link(end) = Link(first);
		MidiFreeCell ((MidiEvPtr)first);
		f->parse[EvType(e)] = VarLenParseMth;
		return kStreamNoError;
	}
}
