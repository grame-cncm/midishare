/*

  Copyright © Grame 1999

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

#include "EventToStream.h"
#include "StreamDefs.h"
#include "msEvents.h"

#define StreamFreeSpace(f) (f->size - StreamLength(f->buff))
#define StreamAdjust(f, ptr, size) \
	StreamLength(f->buff) += size; \
	f->loc = ptr

/*===========================================================================
	internal functions prototypes
  =========================================================================== */
static Boolean     VarLenContinuation   (Ev2StreamPtr f, MidiEvPtr e);

static Boolean     NullLinearizeMth     (Ev2StreamPtr f, MidiEvPtr e);
static Boolean     Data4LinearizeMth    (Ev2StreamPtr f, MidiEvPtr e);
static Boolean     Ext1LinearizeMth     (Ev2StreamPtr f, MidiEvPtr e);
static Boolean     TaskLinearizeMth     (Ev2StreamPtr f, MidiEvPtr e);
static Boolean     VarLenLinearizeMth   (Ev2StreamPtr f, MidiEvPtr e);


/*===========================================================================
		initialization functions
  =========================================================================== */
void msStreamInitMthTbl (msStreamMthTbl lin)
{
	int	i;
	for (i=0; i<256; i++) lin[i] = NullLinearizeMth;

	for (i=typeNote;i<=typeSongSel;i++)     lin[i]= Data4LinearizeMth;

	lin[typeSysEx]        = VarLenLinearizeMth;
	lin[typeStream]       = VarLenLinearizeMth;
	
	for (i=typeClock;i<=typeReset;i++)          lin[i]= Data4LinearizeMth;
	for (i=typePrivate;i<typeProcess;i++)       lin[i]= Ext1LinearizeMth;
	for (i=typeProcess;i<=typeDProcess;i++)     lin[i]= Ext1LinearizeMth;
	for (i=typeQuarterFrame;i<=typeSeqNum;i++)  lin[i]= Data4LinearizeMth;
	for (i=typeTextual;i<=typeCuePoint ;i++)    lin[i]= VarLenLinearizeMth;
	for (i=typeChanPrefix;i<=typeKeySign;i++)   lin[i]= Data4LinearizeMth;

	lin[typeSpecific]      = VarLenLinearizeMth;
	lin[typePortPrefix]    = Data4LinearizeMth;
	
/*  alarms are handled localy both in client and server modes */
	lin[typeRcvAlarm]      = NullLinearizeMth;
	lin[typeApplAlarm]     = Data4LinearizeMth;

	lin[typeMidiOpen]      = VarLenLinearizeMth;
	for (i=typeMidiOpenRes;i<=typeMidiConnect;i++) lin[i]= Data4LinearizeMth;
	lin[typeMidiSetName]   = VarLenLinearizeMth;
	lin[typeMidiSetInfo]   = Data4LinearizeMth;
#ifdef WIN32
    lin[typeMidiSetFilter] = VarLenLinearizeMth;
#else
    lin[typeMidiSetFilter] = Data4LinearizeMth;
#endif
}

void msStreamInit (Ev2StreamPtr f, msStreamMthTbl lin)
{
   	f->buff     = 0;         	/* the linearization buffer ptr	*/
	f->size     = 0;         	/* the buffer size	            */
	f->lin      = lin;
	f->serial   = 0;
	msStreamReset (f);
}

void msStreamReset (Ev2StreamPtr f)
{
	f->loc       = f->buff;         /* the current write location   */
	f->cont      = 0;
	f->nextCell  = 0;
	f->nextCount = 0;
}

/*===========================================================================
		 external functions implementation
  =========================================================================== */
void msStreamStart (Ev2StreamPtr f, void *buffer, unsigned short size)
{
	msStreamHeaderPtr h = (msStreamHeaderPtr)buffer;
   	f->buff     = buffer;         	/* the linearization buffer ptr	*/
	f->size     = size;         	/* the buffer size	            */	
	f->loc       = f->buff;         /* the current write location   */
	h->magic = kStreamMagic;
	h->cont  = 0;
	h->len   = 0;
	h->serial = f->serial++;
	StreamAdjust(f, ++h, sizeof(msStreamHeader));
}

Boolean msStreamPutEvent (Ev2StreamPtr f, MidiEvPtr e)
{
	if (StreamFreeSpace(f) >= sizeof(EvCommonPart)) {
		EvCommonPartPtr c = EventCommonPart(e);
		EvCommonPartPtr ptr = (EvCommonPartPtr)f->loc;
		*ptr++ = *c;
		StreamAdjust(f, ptr, sizeof(EvCommonPart));
		return (f->lin[EvType(e)]) (f, e);
	}
	else {
		f->nextCell = e;
		f->nextCount = 0;
		f->cont = 0;
	}
	return false;
}

Boolean	msStreamContEvent (Ev2StreamPtr f)
{
	MidiEvPtr e = f->nextCell;
	msStreamMthPtr cont = f->cont;
	long c = f->nextCount;
	
	msStreamStart (f, f->buff, f->size);
	if (!e) return true;
	else if (cont) {
		msStreamHeaderPtr h = (msStreamHeaderPtr)f->buff;
		h->cont  = (unsigned short)c;
		return cont (f, e);
	}
	else return msStreamPutEvent (f, e);
}
		
/*===========================================================================
		internal functions implementation
  =========================================================================== */
//___________________________________________________________________________
//	linearization methods					
//___________________________________________________________________________
static Boolean NullLinearizeMth (Ev2StreamPtr f, MidiEvPtr e)
{
	return true;
}

static Boolean Data4LinearizeMth (Ev2StreamPtr f, MidiEvPtr e)
{
	if (StreamFreeSpace(f) >= sizeof(long)) {
		long * ptr = (long *)f->loc;
		long * data = (long *)EventData(e);
		*ptr++ = *data;
		StreamAdjust(f, ptr, sizeof(long));
		return true;
	}
	else {
		f->nextCell = e;
		f->nextCount = 4;
		f->cont = Data4LinearizeMth;
	}
	return false;
}

static Boolean Ext1LinearizeMth (Ev2StreamPtr f, MidiEvPtr e)
{
	if (StreamFreeSpace(f) >= (sizeof(long) * 4)) {
		long * ptr = (long *)f->loc;
		long * data = (long *)LinkST(e);
		int i;
		for (i = 0; i < 4; i++)
			*ptr++ = *data++;
		StreamAdjust(f, ptr, (sizeof(long) * 4));
		return true;
	}
	else {
		f->nextCell = e;
		f->nextCount = (sizeof(long) * 4);
		f->cont = Ext1LinearizeMth;
	}
	return false;
}

static Boolean TaskLinearizeMth (Ev2StreamPtr f, MidiEvPtr e)
{
	if (StreamFreeSpace(f) >= sizeof(MidiEvPtr)) {
		MidiEvPtr * ptr = (MidiEvPtr *)f->loc;
#ifdef MSKernel
		MidiSTPtr ext = LinkST(e);
		*ptr++ = (MidiEvPtr)ext->val[0];
#else
        *ptr++ = e;
#endif
		StreamAdjust(f, ptr, sizeof(MidiEvPtr));
		return true;
	}
    f->cont = TaskLinearizeMth;
	return false;
}

static inline long CountCells (MidiEvPtr e)
{
	long count= 1;
	MidiSEXPtr ext = Link(LinkSE(e));	/* first event extension              */
	while( ext != LinkSE(e)) {		    /* while not the last cell            */
		count++;
		ext= ext->link;		            /* next extension cell                */
	}
	return count;
}

//___________________________________________________________________________
static Boolean VarLenContinuation (Ev2StreamPtr f, MidiEvPtr e)
{
	MidiSEXPtr sx = (MidiSEXPtr)e;
	long *ptr = (long *)f->loc;
	int i;

	while (f->nextCount) {
		if (StreamFreeSpace(f) >= VLCellSize) {
			long *data = (long *)sx->data;
			for (i = 0; i < 3; i++)
				*ptr++ = *data++;
			StreamAdjust(f, ptr, VLCellSize);
			f->nextCount -= VLCellSize;
			sx = sx->link;
		}
		else {
			f->nextCell = (MidiEvPtr)sx;
			f->cont = VarLenContinuation;
			return false;	
		}
	}
	return true;
}

static Boolean VarLenLinearizeMth (Ev2StreamPtr f, MidiEvPtr e)
{
    long len = CountCells (e) * VLCellSize;
	if (StreamFreeSpace(f) >= sizeof(long)) {
		MidiEvPtr firstcell = (MidiEvPtr)Link(LinkSE(e));
		long * ptr = (long *)f->loc;
		*ptr++ = f->nextCount = len;
		StreamAdjust(f, ptr, sizeof(long));
		return VarLenContinuation (f, firstcell);
	}
	else {
		f->nextCell = e;
		f->nextCount = len;
		f->cont = VarLenLinearizeMth;
	}
	return false;
}
