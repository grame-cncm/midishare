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
  grame@rd.grame.fr

*/

#include "msEvents.h"
#include "msSync.h"


#define Debug(s)
#define defaultTime(g)		0

/*__________________________________________________________________________*/
#define kCountFieldsError	0
#define kGetFieldError		0


/*--------------------------------------------------------------------------*/
/*                   Structure to optimize event fields access              */
typedef union TMidiFastEv
{
    MidiEvPtr          std;        /* standard way to access an event */
    struct {                       /* fast way to access an event     */
        MidiEvPtr     link;        /* link to next event           */
        unsigned long date;        /* event date (in ms)           */
        union {                    /* common datas                 */
            Byte      byteC[4];
            short     shortC[2];
            long      longC;
        }             common;
        union {                    /* info depending of event type */
            Byte      byteF[4];
            short     shortF[2];
            long      longF;
        }             specific;
    } FAR *           fast;
    long FAR *        tab;         /* an event seen as a long table */
} TMidiFastEv;

#define Common(e)        (e).fast->common
#define Specific(e)      (e).fast->specific

/*===========================================================================
  Internal functions prototypes
  =========================================================================== */
static void 	InitSmpteEvents		( TSmpteDatasPtr smpte);
static void 	InitNewEvMth		( NewEvMethodPtr FAR *table);
static void 	InitFreeEvMth		( FreeEvMethodPtr FAR *table);
static void 	InitCopyEvMth		( CopyEvMethodPtr FAR *table);
static void 	InitCountFieldsMth	( CountFieldsMethodPtr FAR *table);
static void 	InitSetFieldMth		( SetFieldMethodPtr FAR *table);
static void 	InitGetFieldMth		( GetFieldMethodPtr FAR *table);
static void 	InitAddFieldMth		( AddFieldsMethodPtr FAR *table);

static MidiEvPtr NEAR NewUndefEv	( MSMemoryPtr g, short typeNum);
static MidiEvPtr NEAR NewSmallEv	( MSMemoryPtr g, short typeNum);
static MidiEvPtr NEAR NewSexEv		( MSMemoryPtr g, short typeNum);
static MidiEvPtr NEAR NewPrivateEv	( MSMemoryPtr g, short typeNum);

static void 	NEAR FreeUndefEv	( MSMemoryPtr g, MidiEvPtr e);
static void 	NEAR FreeSmallEv	( MSMemoryPtr g, MidiEvPtr e);
static void 	NEAR FreeSexEv		( MSMemoryPtr g, MidiEvPtr e);
static void 	NEAR FreePrivateEv	( MSMemoryPtr g, MidiEvPtr e);

static MidiEvPtr NEAR CopyUndefEv	( MSMemoryPtr g, MidiEvPtr ev);
static MidiEvPtr NEAR CopySmallEv	( MSMemoryPtr g, MidiEvPtr ev);
static MidiEvPtr NEAR CopyPrivateEv	( MSMemoryPtr g, MidiEvPtr ev);
static MidiEvPtr NEAR CopySexEv		( MSMemoryPtr g, MidiEvPtr ev);

static void	NEAR SetFUndefEv	( MSMemoryPtr g, MidiEvPtr e, unsigned long f, long v);
static void	NEAR SetFSmallEv	( MSMemoryPtr g, MidiEvPtr e, unsigned long f, long v);
static void	NEAR SetF2_16Ev		( MSMemoryPtr g, MidiEvPtr e, unsigned long f, long v);
static void	NEAR SetFTempo		( MSMemoryPtr g, MidiEvPtr e, unsigned long f, long v);
static void	NEAR SetFSMPTEOffset( MSMemoryPtr g, MidiEvPtr e, unsigned long f, long v);
static void	NEAR SetFTimeSign	( MSMemoryPtr g, MidiEvPtr e, unsigned long f, long v);
static void	NEAR SetFKeySign	( MSMemoryPtr g, MidiEvPtr e, unsigned long f, long v);
static void	NEAR SetFSexEv		( MSMemoryPtr g, MidiEvPtr e, unsigned long f, long v);
static void	NEAR SetFPrivateEv	( MSMemoryPtr g, MidiEvPtr e, unsigned long f, long v);

static long	NEAR GetFUndefEv	( MSMemoryPtr g, MidiEvPtr e, long f);
static long	NEAR GetFSmallEv	( MSMemoryPtr g, MidiEvPtr e, long f);
static long	NEAR GetFSexEv		( MSMemoryPtr g, MidiEvPtr e, long f);
static long	NEAR GetFPrivateEv	( MSMemoryPtr g, MidiEvPtr e, long f);
static long	NEAR GetF2_16Ev		( MSMemoryPtr g, MidiEvPtr e, long f);
static long	NEAR GetFTempo		( MSMemoryPtr g, MidiEvPtr e, long f);
static long	NEAR GetFSMPTEOffset( MSMemoryPtr g, MidiEvPtr e, long f);
static long	NEAR GetFTimeSign	( MSMemoryPtr g, MidiEvPtr e, long f);
static long	NEAR GetFKeySign	( MSMemoryPtr g, MidiEvPtr e, long f);

static long	NEAR CountFUndefEv	( MidiEvPtr e);
static long	NEAR Count0Field	( MidiEvPtr e);
static long	NEAR Count1Field	( MidiEvPtr e);
static long	NEAR Count2Fields	( MidiEvPtr e);
static long	NEAR Count3Fields	( MidiEvPtr e);
static long	NEAR Count4Fields	( MidiEvPtr e);
static long	NEAR Count6Fields	( MidiEvPtr e);
static long	NEAR CountFSexEv	( MidiEvPtr e);

static long	NEAR AddFUndefEv	( MSMemoryPtr g, MidiEvPtr e, long v);
static long	NEAR AddNoField		( MSMemoryPtr g, MidiEvPtr e, long v);
static long	NEAR AddFSexEv		( MSMemoryPtr g, MidiEvPtr e, long v);


/*===========================================================================
  External MidiShare functions implementation
  =========================================================================== */		
MSFunctionType(MidiEvPtr) MSNewCell (MSMemoryPtr g)
{
	return PopMidiEv (FreeList(g));
}

/*__________________________________________________________________________________*/
MSFunctionType(void) MSFreeCell (MidiEvPtr e, MSMemoryPtr g)
{
	if (e) {
		PushMidiEv (FreeList(g), e);
	}
}

/*__________________________________________________________________________________*/
MSFunctionType(MidiEvPtr) MSNewEv (short typeNum, MSMemoryPtr g)
                                  { return NewEvMeth(g)[typeNum]( g, typeNum); }
MSFunctionType(void)      MSFreeEv (MidiEvPtr e, MSMemoryPtr g)
                                  { if( e) FreeEvMeth(g)[EvType(e)]( g, e); }
MSFunctionType(MidiEvPtr) MSCopyEv (MidiEvPtr e, MSMemoryPtr g)
                                  { return e ? CopyEvMeth(g)[EvType(e)]( g, e) : e; }
MSFunctionType(void)      MSSetField (MidiEvPtr e, unsigned long f, long v, MSMemoryPtr g)
                                  { if( e) SetFieldMeth(g)[EvType(e)]( g, e, f, v); }
MSFunctionType(long)      MSGetField (MidiEvPtr e, long f, MSMemoryPtr g)
                                  { return e ? GetFieldMeth(g)[EvType(e)]( g, e, f) : kGetFieldError; }
MSFunctionType(long)      MSCountFields (MidiEvPtr e, MSMemoryPtr g)
                                  { return e ? CountFieldsMeth(g)[EvType(e)](e) : kCountFieldsError; }
MSFunctionType(long)      MSAddField (MidiEvPtr e, long v, MSMemoryPtr g)
                                  { return e ? AddFieldMeth(g)[EvType(e)](g, e, v) : MIDIerrEv; }


/*===========================================================================
  External initialization functions
  =========================================================================== */
void InitEvents (MSMemoryPtr g)
{
	InitSmpteEvents		( &Smpte(g));
	InitNewEvMth		( NewEvMeth(g));
	InitFreeEvMth		( FreeEvMeth(g));
	InitCopyEvMth		( CopyEvMeth(g));
	InitSetFieldMth		( SetFieldMeth(g));
	InitGetFieldMth		( GetFieldMeth(g));
	InitCountFieldsMth	( CountFieldsMeth(g));
	InitAddFieldMth		( AddFieldMeth(g));
}



/*===========================================================================
  Internal functions implementation
  =========================================================================== 
  Methods tables initialization
  __________________________________________________________________________*/
static void InitSmpteEvents( TSmpteDatasPtr smpte) {

  smpte->bfMask[0]= 0x06000000;
  smpte->bfMask[1]= 0x1F000000;
  smpte->bfMask[2]= 0x00FC0000;
  smpte->bfMask[3]= 0x0003F000;
  smpte->bfMask[4]= 0x00000F80;
  smpte->bfMask[5]= 0x0000007F;

  smpte->bfShft[0]= 29;
  smpte->bfShft[1]= 24;
  smpte->bfShft[2]= 18;
  smpte->bfShft[3]= 12;
  smpte->bfShft[4]= 7;
  smpte->bfShft[5]= 0;

}

/*__________________________________________________________________________________*/
static void InitNewEvMth( NewEvMethodPtr FAR *table)
{
  int i;

  for (i=typeNote;i<=typeReset;i++)            table[i]= NewSmallEv;
  for (i=typeSysEx;i<=typeStream;i++)          table[i]= NewSexEv;
  for (i=typePrivate;i<=typeDProcess;i++)      table[i]= NewPrivateEv;
  for (i=typeCtrl14b;i<=typeSeqNum;i++)        table[i]= NewSmallEv;
  for (i=typeTextual;i<=typeCuePoint;i++)      table[i]= NewSexEv;
  for (i=typeChanPrefix;i<=typeKeySign;i++)    table[i]= NewSmallEv;
  for (i=typeReserved;i<=typeLastReserved;i++) table[i]= NewUndefEv;
  table[typeQuarterFrame] = NewSmallEv;
  table[typeSpecific]     = NewSexEv;
  table[typeDead]         = NewPrivateEv;
}

/*__________________________________________________________________________________*/
static void InitFreeEvMth( FreeEvMethodPtr FAR *table)
{
  int i;

  for (i=typeNote;i<=typeReset;i++)            table[i]= FreeSmallEv;
  for (i=typeSysEx;i<=typeStream;i++)          table[i]= FreeSexEv;
  for (i=typePrivate;i<=typeDProcess; i++)     table[i]= FreePrivateEv;
  for (i=typeCtrl14b;i<=typeSeqNum;i++)        table[i]= FreeSmallEv;
  for (i=typeTextual;i<=typeCuePoint;i++)      table[i]= FreeSexEv;
  for (i=typeChanPrefix;i<=typeKeySign;i++)    table[i]= FreeSmallEv;
  for (i=typeReserved;i<=typeLastReserved;i++) table[i]= FreeUndefEv;
  table[typeQuarterFrame] = FreeSmallEv;
  table[typeSpecific]     = FreeSexEv;
  table[typeDead]         = FreePrivateEv;

}

/*__________________________________________________________________________________*/
static void InitCopyEvMth( CopyEvMethodPtr FAR *table)
{
  int i;

  for (i=typeNote;i<=typeReset;i++)            table[i]= CopySmallEv;
  for (i=typeSysEx;i<=typeStream;i++)          table[i]= CopySexEv;
  for (i=typePrivate;i<=typeDProcess;i++)      table[i]= CopyPrivateEv;
  for (i=typeCtrl14b;i<=typeSeqNum;i++)        table[i]= CopySmallEv;
  for (i=typeTextual;i<=typeCuePoint;i++)      table[i]= CopySexEv;
  for (i=typeChanPrefix;i<=typeKeySign;i++)    table[i]= CopySmallEv;
  for (i=typeReserved;i<=typeLastReserved;i++) table[i]= CopyUndefEv;
  table[typeQuarterFrame] = CopySmallEv;
  table[typeSpecific]     = CopySexEv;
  table[typeDead]         = CopyPrivateEv;

}

/*__________________________________________________________________________________*/
static void InitCountFieldsMth( CountFieldsMethodPtr FAR *table)
{
  int i;

  for(i=typeKeyOn;i<= typeCtrlChange; i++)    table[i]= Count2Fields;
  for(i=typeProgChange;i<=typeChanPress;i++)  table[i]= Count1Field;
  for(i=typePitchWheel;i<=typeSongPos;i++)    table[i]= Count2Fields;
  for(i=typeClock;i<=typeReset;i++)           table[i]= Count0Field;
  for(i=typeSysEx;i<=typeStream;i++)          table[i]= CountFSexEv;
  for(i=typePrivate;i<=typeDProcess;i++)      table[i]= Count4Fields;
  for(i=typeQuarterFrame;i<=typeRegParam;i++) table[i]= Count2Fields;
  for(i=typeTextual;i<=typeCuePoint;i++)      table[i]= CountFSexEv;
  for(i=typeReserved;i<=typeLastReserved;i++) table[i]= CountFUndefEv;
  table[typeNote]        = Count3Fields;
  table[typeSongSel]     = Count1Field;
  table[typeSeqNum]      = Count1Field;
  table[typeChanPrefix]  = Count1Field;
  table[typeEndTrack]    = Count0Field;
  table[typeTempo]       = Count1Field;
  table[typeSMPTEOffset] = Count6Fields;
  table[typeTimeSign]    = Count4Fields;
  table[typeKeySign]     = Count2Fields;
  table[typeSpecific]    = CountFSexEv;
  table[typeDead]        = Count4Fields;

}

/*__________________________________________________________________________________*/
static void InitSetFieldMth( SetFieldMethodPtr FAR *table)
{
  int i;

  for(i=typeNote; i<= typeReset;i++)          table[i]= SetFSmallEv;
  for(i=typeSysEx;i<=typeStream;i++)          table[i]= SetFSexEv;
  for(i=typePrivate;i<=typeDProcess;i++)      table[i]= SetFPrivateEv;
  for(i=typeCtrl14b;i<=typeSeqNum;i++)        table[i]= SetF2_16Ev;
  for(i=typeTextual;i<=typeCuePoint;i++)      table[i]= SetFSexEv;
  for(i=typeChanPrefix;i<=typeEndTrack;i++)	  table[i]= SetFSmallEv;
  for(i=typeReserved;i<=typeLastReserved;i++) table[i]= SetFUndefEv;
  table[typeQuarterFrame] = SetFSmallEv;
  table[typeTempo]        = SetFTempo;
  table[typeSMPTEOffset]  = SetFSMPTEOffset;
  table[typeTimeSign]     = SetFTimeSign;
  table[typeKeySign]      = SetFKeySign;
  table[typeSpecific]     = SetFSexEv;
  table[typeDead]         = SetFPrivateEv;

}

/*__________________________________________________________________________________*/
static void InitGetFieldMth( GetFieldMethodPtr FAR *table)
{
  int i;

  for(i=typeNote;i<=typeReset;i++)            table[i]= GetFSmallEv;
  for(i=typeSysEx;i<=typeStream;i++)          table[i]= GetFSexEv;
  for(i=typePrivate;i<=typeDProcess;i++)      table[i]= GetFPrivateEv;
  for(i=typeCtrl14b;i<=typeSeqNum;i++)        table[i]= GetF2_16Ev;
  for(i=typeTextual;i<=typeCuePoint;i++)      table[i]= GetFSexEv;
  for(i=typeChanPrefix;i<=typeEndTrack;i++)   table[i]= GetFSmallEv;
  for(i=typeReserved;i<=typeLastReserved;i++) table[i]= GetFUndefEv;
  table[typeQuarterFrame] = GetFSmallEv;
  table[typeTempo]        = GetFTempo;
  table[typeSMPTEOffset]  = GetFSMPTEOffset;
  table[typeTimeSign]     = GetFTimeSign;
  table[typeKeySign]      = GetFKeySign;
  table[typeSpecific]     = GetFSexEv;
  table[typeDead]         = GetFPrivateEv;

}

/*__________________________________________________________________________________*/
static void InitAddFieldMth( AddFieldsMethodPtr FAR *table)
{
  int i;
	
  for(i=typeNote;i<=typeReset;i++)            table[i]= AddNoField;
  for(i=typeSysEx;i<=typeStream;i++)          table[i]= AddFSexEv;
  for(i=typePrivate;i<=typeSeqNum;i++)        table[i]= AddNoField;
  for(i=typeTextual;i<=typeCuePoint;i++)      table[i]= AddFSexEv;
  for(i=typeChanPrefix;i<=typeKeySign;i++)    table[i]= AddNoField;
  for(i=typeReserved;i<=typeLastReserved;i++) table[i]= AddFUndefEv;
  table[typeSpecific] = AddFSexEv;
  table[typeDead]     = AddNoField;
}


/*__________________________________________________________________________________
  NEW EVENT METHODS			
  __________________________________________________________________________________*/
static MidiEvPtr NEAR NewUndefEv( MSMemoryPtr unused1, short unused2)
{
	Debug( "NewEv : WRONG EVENT TYPE !!");
	return 0;
}
	
/*__________________________________________________________________________________*/
static MidiEvPtr NEAR NewSmallEv( MSMemoryPtr g, short typeNum)
{
	TMidiFastEv ev;

	ev.std= MSNewCell( g);
	if( ev.std) {
		Link(ev.std)= 0;
		Date(ev.std)= defaultTime(g);
		EvType(ev.std)= (uchar)typeNum;
		RefNum(ev.std)= 0xff;
		Common(ev).shortC[1]= 0;
		Specific(ev).longF= 0;
	}
	return ev.std;
}

/*__________________________________________________________________________________*/
static MidiEvPtr NEAR NewSexEv( MSMemoryPtr g, short typeNum)
{
	TMidiFastEv ev; TMidiFastEv ext;

	ev.std= MSNewCell( g);
	if( ev.std) {
		ext.std= MSNewCell( g);
		if( ext.std) {
			Link(ext.std)= ext.std;				/* creates a clear extension block */
			Date(ext.std)		= 0;
			Common(ext).longC	= 0;
			Specific(ext).longF	= 0;

			Link(ev.std)= 0;					/* initialize the header           */
			Date(ev.std)= defaultTime(g);
			EvType(ev.std)= (uchar)typeNum;
			RefNum(ev.std)= 0xff;
			Common(ev).shortC[1]= 0;
			LinkSE(ev.std)= (MidiSEXPtr)ext.std;/* link the extension block        */
		}
		else {
			MSFreeCell( ev.std, g);
			return 0;
		}
	}
	return ev.std;
}

/*__________________________________________________________________________________*/
static MidiEvPtr NEAR NewPrivateEv( MSMemoryPtr g, short typeNum)
{
	TMidiFastEv ev; MidiSTPtr ext;
	
	ev.std= MSNewCell( g);
	if( ev.std) {
		ext= (MidiSTPtr)MSNewCell( g);
		if( ext) {
											    /* creates a clear extension block */
			ext->ptr1= ext->ptr2= ext->ptr3= ext->ptr4= 0;
			Link(ev.std)= 0;					/* initialize the header           */
			Date(ev.std)= defaultTime(g);
			EvType(ev.std)= (uchar)typeNum;
			RefNum(ev.std)= 0xff;
			Common(ev).shortC[1]= 0;
			LinkST(ev.std)= ext;				/* link the extension block        */
		}
		else {
			MSFreeCell( ev.std, g);
			return 0;
		}
	}
	return ev.std;
}

/*__________________________________________________________________________________
  FREE EVENT METHODS
  __________________________________________________________________________________*/
static void NEAR FreeUndefEv( MSMemoryPtr unused1, MidiEvPtr unused2)
{
	Debug( "FreeEv : WRONG EVENT TYPE !!");
}

/*__________________________________________________________________________________*/
static void NEAR FreeSmallEv( MSMemoryPtr g, MidiEvPtr e)
{
	MSFreeCell( e, g);
}

/*__________________________________________________________________________________*/
static void NEAR FreeSexEv( MSMemoryPtr g, MidiEvPtr e)
{
	if (e) {
		MidiEvPtr extension = (MidiEvPtr)LinkSE(e);
		e->link= extension->link;
		PushMidiList (FreeList(g), e, extension);
	}
}

/*__________________________________________________________________________________*/
static void NEAR FreePrivateEv( MSMemoryPtr g, MidiEvPtr e)
{
	if (e) {
		MidiEvPtr extension = (MidiEvPtr)LinkST(e);
		e->link= extension;
		PushMidiList (FreeList(g), e, extension);
	}
}

/*__________________________________________________________________________________
  COPY EVENT METHODS					
  __________________________________________________________________________________*/
static MidiEvPtr NEAR CopyUndefEv( MSMemoryPtr unused1, MidiEvPtr unused2)
{
	Debug( "CopyEv : WRONG EVENT TYPE !!");
	return 0;
}

/*__________________________________________________________________________________*/
static MidiEvPtr NEAR CopySmallEv( MSMemoryPtr g, MidiEvPtr ev)
{
	MidiEvPtr copy = MSNewCell( g);
	if (copy) {
		*copy = *ev;
	}
	return copy;
}

/*__________________________________________________________________________________*/
static MidiEvPtr NEAR CopyPrivateEv( MSMemoryPtr g, MidiEvPtr ev)
{
	MidiEvPtr copy; MidiSTPtr ext;

	copy= CopySmallEv( g, ev);
	if( copy) {
		ext= (MidiSTPtr)CopySmallEv( g, (MidiEvPtr)LinkST(ev));
		if( ext)
			LinkST( copy)= ext;
		else {
			MSFreeCell( copy, g);
			return 0;
		}
	}
	return copy;
}

/*__________________________________________________________________________________*/
static MidiEvPtr NEAR CopySexEv( MSMemoryPtr g, MidiEvPtr ev)
{
	MidiEvPtr copy; MidiSEXPtr ext, extCopy, previous;
	
	copy= CopyPrivateEv( g, ev);			/* copy the event like a private event */
	if( copy) {
		extCopy= previous= LinkSE(copy);	/* get the last extension cell      */
		ext= Link(LinkSE(ev));				/* get the first cell to be copied	*/
		while( ext!= LinkSE(ev)) {			/* is it the last ?                 */
			extCopy= (MidiSEXPtr)CopySmallEv( g, (MidiEvPtr)ext);
			if( extCopy) {					/* cell copy succes                 */
				Link(previous)= extCopy;	/* link to the previous             */
				previous= extCopy;			/* previous cell is the copy        */
			}
			else {							/* copy failed                      */
				Link(previous)=LinkSE(copy);/* restore the event consistency    */
				FreeSexEv( g, copy);		/* and free the event               */
				return 0;					/* returns nil                      */
			}
			ext= Link(ext);					/* next extension cell to be copied */
		}
		Link(extCopy)= LinkSE(copy);		/* link last copy to the last extension */
	}
	return copy;
}



/*__________________________________________________________________________________
  SET FIELD METHODS				
  __________________________________________________________________________________*/
static void NEAR SetFUndefEv( MSMemoryPtr unused1, MidiEvPtr unused2, unsigned long unused3, long unused4)
{
	Debug( "SetFEv : WRONG EVENT TYPE !!");
}

/*__________________________________________________________________________________*/
static void NEAR SetFSmallEv( MSMemoryPtr unused1, MidiEvPtr e, unsigned long f, long v)
{
	if( f< 2)
		Data(e)[f]= (Byte)v;
	else if( f== 2)
		Dur(e)=(unsigned short)v;
}

/*__________________________________________________________________________________*/
static void NEAR SetF2_16Ev( MSMemoryPtr unused1, MidiEvPtr e, unsigned long f, long v)
{
	if( f < 2) {
		TMidiFastEv fast;

		fast.std= e;
		Specific(fast).shortF[f]=(short)v;
	}
}

/*__________________________________________________________________________________*/
static void NEAR SetFTempo( MSMemoryPtr unused1, MidiEvPtr e, unsigned long f, long v)
{
	if( !f) Tempo(e)= v;
}

/*__________________________________________________________________________________*/

static void NEAR SetFSMPTEOffset( MSMemoryPtr g, MidiEvPtr e, unsigned long f, long v)
{
	if( f < 6) {
		TMidiFastEv fast;

		fast.std= e;
		v <<= Smpte(g).bfShft[f];
		v &= Smpte(g).bfMask[f];
		Specific(fast).longF |= v;
	}
}


/*__________________________________________________________________________________*/
static void NEAR SetFTimeSign( MSMemoryPtr unused1, MidiEvPtr e, unsigned long f, long v)
{
	if( f < 4) Data(e)[f]=(Byte)v;
}

/*__________________________________________________________________________________*/
static void NEAR SetFKeySign( MSMemoryPtr unused1, MidiEvPtr e, unsigned long f, long v)
{
	if( f < 2) Data(e)[f]=(Byte)v;
}

/*__________________________________________________________________________________*/
static void NEAR SetFSexEv( MSMemoryPtr unused1, MidiEvPtr e, unsigned long f, long v)
{
	TSexEv ext;

	ext.std= Link(LinkSE(e));           /* first event extension cell           */
	while( true) {
		if( ext.std== LinkSE(e)) {      /* if it's the last extension cell      */
			if( f < ext.last->num)		/* if the index is free                 */
				ext.last->data[f]=(Byte)v;	/* store the value                  */
			break;
		}
		if( f < kLenDatas) {			/* if the field is located in this cell */
			ext.std->data[f]=(Byte)v;	/* store the value                      */
			break;
		}
		f-= kLenDatas;					/* substract lenData to the field index */
		ext.std= ext.last->link;		/* get the next extension cell and loop */
	}
}

/*__________________________________________________________________________________*/
static void NEAR SetFPrivateEv( MSMemoryPtr unused1, MidiEvPtr e, unsigned long f, long v)
{
	if( f < 4) {
		TMidiFastEv fast;

		fast.std= (MidiEvPtr)LinkST(e);
		fast.tab[f]= v;
	}
}


/*__________________________________________________________________________________
  GET FIELD METHODS					
  __________________________________________________________________________________*/
static long NEAR GetFUndefEv( MSMemoryPtr unused1, MidiEvPtr unused2, long unused3)
{
	Debug( "GetField : WRONG EVENT TYPE !!");
	return kGetFieldError;
}

/*__________________________________________________________________________________*/
static long NEAR GetFSmallEv( MSMemoryPtr unused1, MidiEvPtr e, long f)
{
	if( f < 2)
		return Data(e)[f];
	return Dur(e);
}

/*__________________________________________________________________________________*/
static long NEAR GetFSexEv( MSMemoryPtr unused1, MidiEvPtr e, long f)
{
	TSexEv ext;

	ext.std= Link(LinkSE(e));           /* first event extension cell           */
	while( ext.std!= LinkSE(e)) {       /* while it'snt the last cell           */
		if( f < kLenDatas)              /* if the field is located in this cell */
			return ext.std->data[f];    /* returns the value                    */
		f-= kLenDatas;                  /* substract lenData to the field index */
		ext.std= ext.last->link;        /* get the next extension cell          */
	}
                                        /* last extension cell                  */
	if( f < ext.last->num)              /* if the field is located in this cell */
		return ext.last->data[f];       /* returns the value                    */
                                        /* otherwise returns the last value     */
	return ext.last->data[ext.last->num-1];
}

/*__________________________________________________________________________________*/
static long NEAR GetFPrivateEv( MSMemoryPtr unused1, MidiEvPtr e, long f)
{
	TMidiFastEv fast;

	fast.std= (MidiEvPtr)LinkST(e);
	return fast.tab[ (f > 3) ? 3 :f];
}

/*__________________________________________________________________________________*/
static long NEAR GetF2_16Ev( MSMemoryPtr unused1, MidiEvPtr e, long f)
{
	TMidiFastEv fast;

	fast.std= e;
	return Specific(fast).shortF[ (f>1) ? 1 : f];
}

/*__________________________________________________________________________________*/
static long NEAR GetFTempo( MSMemoryPtr unused1, MidiEvPtr e, long unused2)
{
	return Tempo(e);
}

/*__________________________________________________________________________________*/
static long NEAR GetFSMPTEOffset( MSMemoryPtr g, MidiEvPtr e, long f)
{
	long val; TMidiFastEv fast;

	fast.std= e;
	if( f > 5) f= 5;
	val= Specific(fast).longF & Smpte(g).bfMask[f];
	return val >> Smpte(g).bfShft[f];
}

/*__________________________________________________________________________________*/
static long NEAR GetFTimeSign( MSMemoryPtr unused1, MidiEvPtr e, long f)
{
	return Data(e)[ (f > 3) ? 3 : f];
}

/*__________________________________________________________________________________*/
static long NEAR GetFKeySign( MSMemoryPtr unused1, MidiEvPtr e, long f)
{
	return Data(e)[ (f > 1) ? 1 : f];
}



/*__________________________________________________________________________________
  COUNT FIELDS METHODS
  __________________________________________________________________________________*/
static long NEAR CountFUndefEv ( MidiEvPtr unused1)
{
	Debug( "CountField : WRONG EVENT TYPE !!");
	return kCountFieldsError;
}

/*__________________________________________________________________________________*/
static long NEAR Count0Field	( MidiEvPtr unused1)	{ return 0;}
static long NEAR Count1Field	( MidiEvPtr unused1)	{ return 1;}
static long NEAR Count2Fields	( MidiEvPtr unused1)	{ return 2;}
static long NEAR Count3Fields	( MidiEvPtr unused1)	{ return 3;}
static long NEAR Count4Fields	( MidiEvPtr unused1)	{ return 4;}
static long NEAR Count6Fields	( MidiEvPtr unused1)	{ return 6;}

/*__________________________________________________________________________________*/
static long NEAR CountFSexEv( MidiEvPtr e)
{
	TSexEv ext; long count= 0;

	ext.std= Link(LinkSE(e));			/* first event extension              */
	while( ext.std!= LinkSE(e)) {		/* while it'snt the last cell         */
		count+= kLenDatas;				/* add the data len to the count      */
		ext.std= ext.last->link;		/* next extension cell                */
	}
	return count + ext.last->num;		/* return count + the data len of the */
										/* the last cell                      */
}


/*__________________________________________________________________________________
  ADD FIELD METHODS
  __________________________________________________________________________________*/
static long NEAR AddFUndefEv( MSMemoryPtr unused1, MidiEvPtr unused2, long unused3)
{
	Debug( "AddField : WRONG EVENT TYPE !!");
	return MIDIerrUndef;
}

/*__________________________________________________________________________________*/
static long NEAR AddNoField( MSMemoryPtr unused1, MidiEvPtr unused2, long unused3)
{
	return MIDIerrIndex;
}

/*__________________________________________________________________________________*/
static long NEAR AddFSexEv( MSMemoryPtr g, MidiEvPtr e, long v)
{
	TSexEv ext, nouv;

	ext.std= LinkSE(e);							/* event last extension         */
	if( ext.last->num < kLenDatas-1) {			/* If there remains place       */
		ext.last->data[ext.last->num]=(Byte)v;	/* store the value              */
		ext.last->num++;						/* update the busy space count  */
	}
	else { 
		nouv.std= (MidiSEXPtr)MSNewCell( g);	/* add a new cell               */
		if( nouv.std) {
			ext.last->data[kLenDatas-1]=(Byte)v;/* store the value              */
			nouv.last->num= 0;					/* busy space count             */
			nouv.std->link= ext.std->link;		/* link the new cell            */
			ext.std->link= nouv.std;
			LinkSE(e)= nouv.std;				/* link header to the last cell */
		}
		else return MIDIerrSpace;				/* allocation failed            */
	}
	return true;
}
