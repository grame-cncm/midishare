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

#include "EventToUDPStream.h"

#ifndef false
enum {false, true };
#endif

/*===========================================================================
	internal functions prototypes
  =========================================================================== */

static void     NormalContinuation     (Ev2UDPStreamPtr f);
static void     VarLenContinuation     (Ev2UDPStreamPtr f);

static void     NullLinearizeMth       (MidiEvPtr e, Ev2UDPStreamPtr f);
static void     Linearize0Byte         (MidiEvPtr e, Ev2UDPStreamPtr f);
static void     Linearize1Byte         (MidiEvPtr e, Ev2UDPStreamPtr f);
static void     Linearize2Bytes        (MidiEvPtr e, Ev2UDPStreamPtr f);
static void     Linearize4Bytes        (MidiEvPtr e, Ev2UDPStreamPtr f);
static void     LinearizeVarLen        (MidiEvPtr e, Ev2UDPStreamPtr f);
static void     Linearize1Short        (MidiEvPtr e, Ev2UDPStreamPtr f);
static void     Linearize2Shorts       (MidiEvPtr e, Ev2UDPStreamPtr f);
static void     Linearize2Bytes1Short  (MidiEvPtr e, Ev2UDPStreamPtr f);
static void     Linearize1Long         (MidiEvPtr e, Ev2UDPStreamPtr f);


/*===========================================================================
		initialization functions
  =========================================================================== */
void UDPStreamInitMthTbl (UDPLinearizeMthTbl lin)
{
	int	i;
	
	for (i=0; i<256; i++) lin[i] = NullLinearizeMth;

	lin[typeNote]		= Linearize2Bytes1Short;
		
	lin[typeKeyOn]		= Linearize2Bytes;
	lin[typeKeyOff]		= Linearize2Bytes;
	lin[typeKeyPress] 	= Linearize2Bytes;
	lin[typeCtrlChange]	= Linearize2Bytes;
	lin[typeProgChange]	= Linearize1Byte;
	lin[typeChanPress]	= Linearize1Byte;
	lin[typePitchWheel]	= Linearize2Bytes;
		
	lin[typeSongPos]	= Linearize2Bytes;
	lin[typeSongSel]	= Linearize1Byte;
	lin[typeClock]		= Linearize0Byte;
	lin[typeStart]		= Linearize0Byte;
	lin[typeContinue]	= Linearize0Byte;
	lin[typeStop]		= Linearize0Byte;
		
	lin[typeTune]		= Linearize0Byte;
	lin[typeActiveSens]	= Linearize0Byte;
	lin[typeReset]		= Linearize0Byte;
	
	lin[typeSysEx]		= LinearizeVarLen;
	lin[typeStream]		= LinearizeVarLen;
		
	lin[typeQuarterFrame] = Linearize2Bytes;

	lin[typeCtrl14b]	= Linearize2Shorts;
	lin[typeNonRegParam]= Linearize2Shorts;
	lin[typeRegParam]	= Linearize2Shorts;

	lin[typeSeqNum]		= Linearize1Short;
	lin[typeTextual]	= LinearizeVarLen;
	lin[typeCopyright]	= LinearizeVarLen;
	lin[typeSeqName]	= LinearizeVarLen;
	lin[typeInstrName]	= LinearizeVarLen;
	lin[typeLyric]		= LinearizeVarLen;
	lin[typeMarker]		= LinearizeVarLen;
	lin[typeCuePoint]	= LinearizeVarLen;
	lin[typeChanPrefix]	= Linearize1Byte;
	lin[typeEndTrack]	= Linearize0Byte;
	lin[typeTempo]		= Linearize1Long;
	lin[typeSMPTEOffset]= Linearize4Bytes;

	lin[typeTimeSign]	= Linearize4Bytes;
	lin[typeKeySign]	= Linearize2Bytes;
	lin[typeSpecific]	= LinearizeVarLen;

}

void UDPStreamInit (Ev2UDPStreamPtr f, UDPLinearizeMthTbl lin)
{
	f->cont     = NormalContinuation;
	f->lin      = lin;
	f->nextCell = 0;
	f->count	= 0;
}

void UDPStreamReset (Ev2UDPStreamPtr f)
{
	MidiEvPtr next, e = (MidiEvPtr)f->nextCell;
	while (e) {
		next = Link(e);
		MidiFreeCell(e);
		e = next;
	}
	f->cont     = NormalContinuation;
	f->nextCell = 0;
	f->count    = 0;
}

/*===========================================================================
		 external functions implementation
  =========================================================================== */
MidiEvPtr UDPStreamPutEvent (Ev2UDPStreamPtr f, MidiEvPtr e)
{
	if (f->count)  return e;
	(f->lin[EvType(e)]) (e, f);
	return 0;
}

Boolean	UDPStreamGetByte (Ev2UDPStreamPtr f, Byte *code)
{	
 	if (!f->count) return false;
	*code = f->data[f->count--];
	if (!f->count) (*f->cont)( f);
	return true;
}
		
/*===========================================================================
		internal functions implementation
  =========================================================================== */

//___________________________________________________________________________
//	continuations					
//___________________________________________________________________________
static void NormalContinuation (Ev2UDPStreamPtr f)
{
	f->count= 0;
}

static void VarLenContinuation (Ev2UDPStreamPtr f)
{
	MidiSEXPtr	c;
	int			i=0, j;
	
	c = f->nextCell;
	f->nextCell = c->link;
	if (!f->nextCell) {
		f->cont = NormalContinuation;
		f->count = j = c->data[11];
		while (j) {
			f->data[j--] = c->data[i++];
		}
	} else {
		f->count = j = 12;
		while (j) {
			f->data[j--] = c->data[i++];
		}
	}
	MidiFreeCell((MidiEvPtr)c);	
}

//___________________________________________________________________________
//	linearization methods					
//___________________________________________________________________________
#define LinearizeDate(i, e, f)		LinearizeShort (i, Date(e), f)

static inline void LinearizeLong (short i, long v, Ev2UDPStreamPtr f)
{
	char *ptr = (char *)&v;
#if macintosh
	f->data[i--] = *ptr++;
	f->data[i--] = *ptr++;
	f->data[i--] = *ptr++;
	f->data[i] = *ptr;
#else
	i -= 3;
	f->data[i++] = *ptr++;
	f->data[i++] = *ptr++;
	f->data[i++] = *ptr++;
	f->data[i] = *ptr;
#endif
}

static inline void LinearizeShort (short i, short v, Ev2UDPStreamPtr f)
{
	char *ptr = (char *)&v;
#if macintosh
	f->data[i--] = *ptr++;
	f->data[i] = *ptr;
#else
	f->data[i-1] = *ptr++;
	f->data[i] = *ptr;
#endif
}

static inline void LinearizeCommon (short i, MidiEvPtr e, Ev2UDPStreamPtr f)
{
	f->data[i--] = EvType(e);
	LinearizeDate (i, e, f);
	i -= kDateSize;
	f->data[i--] = Port(e);
	f->data[i] = Chan(e);
}

static inline void LinearizeVarLenCommon (short i, short len, MidiEvPtr e, Ev2UDPStreamPtr f)
{
	LinearizeShort (i, len, f);
	LinearizeCommon (i-2, e, f);
}

static inline void LinearizeFixedCommon (short i, MidiEvPtr e, Ev2UDPStreamPtr f)
{
	LinearizeShort (i, i-kLenSize, f);
	LinearizeCommon (i-2, e, f);
}

static void NullLinearizeMth (MidiEvPtr e, Ev2UDPStreamPtr f)
{
	MidiFreeEv (e);
}

static void Linearize0Byte (MidiEvPtr e, Ev2UDPStreamPtr f)
{
	f->count = kCommonSize;
	LinearizeFixedCommon (kCommonSize, e, f);
	MidiFreeEv (e);
}

static void Linearize1Byte (MidiEvPtr e, Ev2UDPStreamPtr f)
{
	f->count = kCommonSize + 1;
	LinearizeFixedCommon (kCommonSize + 1, e, f);
	f->data[1] = Data(e)[0];
	MidiFreeEv (e);
}

static void Linearize2Bytes (MidiEvPtr e, Ev2UDPStreamPtr f)
{
	f->count = kCommonSize + 2;
	LinearizeFixedCommon (kCommonSize + 2, e, f);
	f->data[2] = Data(e)[0];
	f->data[1] = Data(e)[1];
	MidiFreeEv (e);
}

static void Linearize4Bytes (MidiEvPtr e, Ev2UDPStreamPtr f)
{
	f->count = kCommonSize + 4;
	LinearizeFixedCommon (kCommonSize + 4, e, f);
	f->data[4] = Data(e)[0];
	f->data[3] = Data(e)[1];
	f->data[2] = Data(e)[2];
	f->data[1] = Data(e)[3];
	MidiFreeEv (e);
}

static void Linearize1Short (MidiEvPtr e, Ev2UDPStreamPtr f)
{
	f->count = kCommonSize + 2;
	LinearizeFixedCommon (kCommonSize + 2, e, f);
	LinearizeShort (2, e->info.param.num, f);
	MidiFreeEv (e);
}

static void Linearize2Shorts (MidiEvPtr e, Ev2UDPStreamPtr f)
{
	f->count = kCommonSize + 4;
	LinearizeFixedCommon (kCommonSize + 4, e, f);
	LinearizeShort (4, e->info.param.val, f);
	LinearizeShort (2, e->info.param.num, f);
	MidiFreeEv (e);
}

static void Linearize2Bytes1Short (MidiEvPtr e, Ev2UDPStreamPtr f)
{
	f->count = kCommonSize + 4;
	LinearizeFixedCommon (kCommonSize + 4, e, f);
	f->data[4] = Data(e)[0];
	f->data[3] = Data(e)[1];
	LinearizeShort (2, e->info.param.val, f);
	MidiFreeEv (e);
}

static void Linearize1Long (MidiEvPtr e, Ev2UDPStreamPtr f)
{
	f->count = kCommonSize + 4;
	LinearizeFixedCommon (kCommonSize + 4, e, f);
	LinearizeLong (4, Tempo(e), f);
	MidiFreeEv (e);
}

static void LinearizeVarLen (MidiEvPtr e, Ev2UDPStreamPtr f)
{
	MidiSEXPtr	last, first; long len;
	
	len = MidiCountFields(e) + kCommonSize - kLenSize;
	last = e->info.linkSE;
	first = last->link;
	last->link = 0;
	if (first == last) {
		int i = 0, j = first->data[11];
		f->count = kCommonSize + j;
		LinearizeVarLenCommon (f->count, len, e, f);
		while(j>0) f->data[j--] = first->data[i++];
	} else {
		int i = 0, j = 12;
		f->count = kCommonSize + 12;
		LinearizeVarLenCommon (f->count, len, e, f);
		while(j>0) f->data[j--] = first->data[i++];
		f->nextCell = first->link;
		f->cont = VarLenContinuation;
	}
	MidiFreeCell (e);	
	MidiFreeCell ((MidiEvPtr)first);	
}
