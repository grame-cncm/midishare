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
  
  modifications history:
   [08-09-99] DF - using lifo for memory management
                   including methods tables in the module

*/

#include "msEvents.h"

#define Debug(s)
#define defaultTime			0

/*__________________________________________________________________________*/
#define kCountFieldsError	0
#define kGetFieldError		0

#define typeLastReserved    typeDead-1    /* last reserved event type      */
#define kLenEvent           16            /* len of a MidiShare event      */
#define kLenDatas           kLenEvent-4   /* len of a sysex extension data */

// -- Method prototypes
typedef MidiEvPtr (*NewEvMethodPtr)      (lifo* freelist, short typeNum);
typedef MidiEvPtr (*CopyEvMethodPtr)     (lifo* freelist, MidiEvPtr ev);
typedef void      (*FreeEvMethodPtr)     (lifo* freelist, MidiEvPtr ev);
typedef void      (*SetFieldMethodPtr)   (MidiEvPtr ev, unsigned long f, long v);
typedef long      (*GetFieldMethodPtr)   (MidiEvPtr ev, unsigned long f);
typedef long      (*CountFieldsMethodPtr)(MidiEvPtr ev);
typedef void      (*AddFieldMethodPtr)   (lifo* freelist, MidiEvPtr ev, long v);


/*===========================================================================
  Internal functions prototypes
  =========================================================================== */
static void 	InitNewEvMth		( NewEvMethodPtr * table);
static void 	InitFreeEvMth		( FreeEvMethodPtr * table);
static void 	InitCopyEvMth		( CopyEvMethodPtr * table);
static void 	InitCountFieldsMth	( CountFieldsMethodPtr * table);
static void 	InitSetFieldMth		( SetFieldMethodPtr * table);
static void 	InitGetFieldMth		( GetFieldMethodPtr * table);
static void 	InitAddFieldMth		( AddFieldMethodPtr * table);

static MidiEvPtr NewUndefEv		( lifo* freelist, short typeNum);
static MidiEvPtr NewSmallEv		( lifo* freelist, short typeNum);
static MidiEvPtr NewSexEv		( lifo* freelist, short typeNum);
static MidiEvPtr NewPrivateEv	( lifo* freelist, short typeNum);

static void 	FreeUndefEv	    ( lifo* freelist, MidiEvPtr e);
static void 	FreeSmallEv		( lifo* freelist, MidiEvPtr e);
static void 	FreeSexEv		( lifo* freelist, MidiEvPtr e);
static void 	FreePrivateEv	( lifo* freelist, MidiEvPtr e);

static MidiEvPtr CopyUndefEv	( lifo* freelist, MidiEvPtr ev);
static MidiEvPtr CopySmallEv	( lifo* freelist, MidiEvPtr ev);
static MidiEvPtr CopyPrivateEv	( lifo* freelist, MidiEvPtr ev);
static MidiEvPtr CopySexEv		( lifo* freelist, MidiEvPtr ev);

static void	SetFUndefEv		( MidiEvPtr e, unsigned long f, long v);
static void	SetFSmallEv		( MidiEvPtr e, unsigned long f, long v);
static void	SetF2_16Ev		( MidiEvPtr e, unsigned long f, long v);
static void	SetFTempo		( MidiEvPtr e, unsigned long f, long v);
static void	SetFSMPTEOffset	( MidiEvPtr e, unsigned long f, long v);
static void	SetFTimeSign	( MidiEvPtr e, unsigned long f, long v);
static void	SetFKeySign		( MidiEvPtr e, unsigned long f, long v);
static void	SetFSexEv		( MidiEvPtr e, unsigned long f, long v);
static void	SetFPrivateEv	( MidiEvPtr e, unsigned long f, long v);

static long	GetFUndefEv		( MidiEvPtr e, unsigned long f);
static long	GetFSmallEv		( MidiEvPtr e, unsigned long f);
static long	GetFSexEv		( MidiEvPtr e, unsigned long f);
static long	GetFPrivateEv	( MidiEvPtr e, unsigned long f);
static long	GetF2_16Ev		( MidiEvPtr e, unsigned long f);
static long	GetFTempo		( MidiEvPtr e, unsigned long f);
static long	GetFSMPTEOffset	( MidiEvPtr e, unsigned long f);
static long	GetFTimeSign	( MidiEvPtr e, unsigned long f);
static long	GetFKeySign		( MidiEvPtr e, unsigned long f);

static long	CountFUndefEv	( MidiEvPtr e);
static long	Count0Field		( MidiEvPtr e);
static long	Count1Field		( MidiEvPtr e);
static long	Count2Fields	( MidiEvPtr e);
static long	Count3Fields	( MidiEvPtr e);
static long	Count4Fields	( MidiEvPtr e);
static long	Count6Fields	( MidiEvPtr e);
static long	CountFSexEv		( MidiEvPtr e);

static void	AddFUndefEv		( lifo* freelist, MidiEvPtr e, long v);
static void	AddNoField		( lifo* freelist, MidiEvPtr e, long v);
static void	AddFSexEv		( lifo* freelist, MidiEvPtr e, long v);


/* data storage */

#ifdef __Macintosh__
# ifdef __POWERPC__
# else
static asm void NewEvMeth()       { ds.l 256 }
static asm void CopyEvMeth()      { ds.l 256 }
static asm void FreeEvMeth()      { ds.l 256 }
static asm void SetFieldMeth()    { ds.l 256 }
static asm void GetFieldMeth()    { ds.l 256 }
static asm void CountFieldsMeth() { ds.l 256 }
static asm void AddFieldMeth()    { ds.l 256 }


#define NewEvMethodTbl	     ((NewEvMethodPtr *)NewEvMeth)
#define CopyEvMethodTbl      ((CopyEvMethodPtr *)CopyEvMeth)
#define FreeEvMethodTbl      ((FreeEvMethodPtr *)FreeEvMeth)
#define SetFieldMethodTbl    ((SetFieldMethodPtr *)SetFieldMeth)
#define GetFieldMethodTbl    ((GetFieldMethodPtr *)GetFieldMeth)
#define CountFieldsMethodTbl ((CountFieldsMethodPtr *)CountFieldsMeth)
#define AddFieldMethodTbl    ((AddFieldMethodPtr *)AddFieldMeth)

static asm void smpteMask() { 
	dc.l 0x06000000,0x1F000000,0x00FC0000
	dc.l 0x0003F000,0x00000F80,0x0000007F
}
static asm void smpteShft() { 
	dc.b 29,24,18,12,7,0 
}

#define smpteMaskTbl	  (const unsigned long *)smpteMask
#define smpteShftTbl      (const char *)smpteShft

# endif

#else

static NewEvMethodPtr       NewEvMeth[256];        /* Allocation methods table */
static CopyEvMethodPtr      CopyEvMeth[256];       /* Copy methods table       */
static FreeEvMethodPtr      FreeEvMeth[256];       /* Free methods table       */
static SetFieldMethodPtr    SetFieldMeth[256];     /* SetField methods table   */
static GetFieldMethodPtr    GetFieldMeth[256];     /* GetField methods table   */
static CountFieldsMethodPtr CountFieldsMeth[256];  /* CountField methods table */
static AddFieldMethodPtr    AddFieldMeth[256];     /* AddField methods table   */

#define NewEvMethodTbl	     NewEvMeth
#define CopyEvMethodTbl      CopyEvMeth
#define FreeEvMethodTbl      FreeEvMeth
#define SetFieldMethodTbl    SetFieldMeth
#define GetFieldMethodTbl    GetFieldMeth
#define CountFieldsMethodTbl CountFieldsMeth
#define AddFieldMethodTbl    AddFieldMeth

const unsigned long smpteMask[]= { 0x06000000,0x1F000000,0x00FC0000,
							       0x0003F000,0x00000F80,0x0000007F };
const char 		    smpteShft[]= { 29,24,18,12,7,0 };

#define smpteMaskTbl	  smpteMask
#define smpteShftTbl      smpteShft

#endif


/*===========================================================================
  External MidiShare functions implementation
  =========================================================================== */		
MSFunctionType(MidiEvPtr) MSNewCellFunction (lifo* freelist)
                                  { return (MidiEvPtr)lfpop(freelist); }

MSFunctionType(void)      MSFreeCellFunction (MidiEvPtr e, lifo* freelist)
                                  { if( e) lfpush(freelist, (cell *)e); }

MSFunctionType(MidiEvPtr) MSNewEv (short typeNum, lifo* freelist)
                                  { return NewEvMethodTbl[typeNum](freelist, typeNum); }

MSFunctionType(void)      MSFreeEv (MidiEvPtr e, lifo* freelist)
                                  { if( e) FreeEvMethodTbl[EvType(e)]( freelist, e); }

MSFunctionType(MidiEvPtr) MSCopyEv (MidiEvPtr e, lifo* freelist)
                                  { return e ? CopyEvMethodTbl[EvType(e)]( freelist, e) : e; }

MSFunctionType(void)      MSSetField (MidiEvPtr e, unsigned long f, long v)
                                  { if( e) SetFieldMethodTbl[EvType(e)]( e, f, v); }

MSFunctionType(long)      MSGetField (MidiEvPtr e, long f)
                                  { return e ? GetFieldMethodTbl[EvType(e)]( e, f) : kGetFieldError; }

MSFunctionType(long)      MSCountFields (MidiEvPtr e)
                                  { return e ? CountFieldsMethodTbl[EvType(e)](e) : kCountFieldsError; }

MSFunctionType(void)      MSAddField (MidiEvPtr e, long v, lifo* freelist)
                                  { if ( e) AddFieldMethodTbl[EvType(e)](freelist, e, v); }


/*===========================================================================
  External initialization functions
  =========================================================================== */
void InitEvents ()
{
	InitNewEvMth		( NewEvMethodTbl );
	InitFreeEvMth		( FreeEvMethodTbl );
	InitCopyEvMth		( CopyEvMethodTbl );
	InitSetFieldMth		( SetFieldMethodTbl );
	InitGetFieldMth		( GetFieldMethodTbl );
	InitCountFieldsMth	( CountFieldsMethodTbl );
	InitAddFieldMth		( AddFieldMethodTbl );
	
	/* will be implemented as method table later */
	#ifdef __Linux__
	InitStructTbl();
	#endif
}

#ifdef __Macintosh__
#	define NewProcessEv		NewPrivateEv
#	define FreeProcessEv	FreePrivateEv
#	define CopyProcessEv	CopyPrivateEv
#	define CountFProcessEv	Count4Fields
#	define SetFProcessEv	SetFPrivateEv
#	define GetFProcessEv	GetFPrivateEv
#endif

#ifdef __Linux__
#	define NewProcessEv		NewSmallEv
#	define FreeProcessEv	FreeSmallEv
#	define CopyProcessEv	CopySmallEv
#	define CountFProcessEv	CountFUndefEv
#	define SetFProcessEv	SetFUndefEv
#	define GetFProcessEv	GetFUndefEv
#endif

/*===========================================================================
  Internal functions implementation
  =========================================================================== 
  Methods tables initialization
  __________________________________________________________________________*/
static void InitNewEvMth( NewEvMethodPtr *table)
{
  int i;

  for (i=typeNote;i<=typeReset;i++)            table[i]= NewSmallEv;
  for (i=typeSysEx;i<=typeStream;i++)          table[i]= NewSexEv;
  for (i=typePrivate;i<typeProcess;i++)        table[i]= NewPrivateEv;
  for (i=typeCtrl14b;i<=typeSeqNum;i++)        table[i]= NewSmallEv;
  for (i=typeTextual;i<=typeCuePoint;i++)      table[i]= NewSexEv;
  for (i=typeChanPrefix;i<=typeKeySign;i++)    table[i]= NewSmallEv;
  for (i=typeReserved;i<=typeLastReserved;i++) table[i]= NewUndefEv;
  table[typeQuarterFrame] = NewSmallEv;
  table[typeSpecific]     = NewSexEv;
  table[typeRcvAlarm]     = NewSmallEv;
  table[typeApplAlarm]    = NewSmallEv;
  
  table[typeProcess] 	= NewProcessEv;
  table[typeDProcess] 	= NewProcessEv;
  table[typeDead]    	= NewProcessEv; 
}

/*__________________________________________________________________________________*/
static void InitFreeEvMth( FreeEvMethodPtr *table)
{
  int i;

  for (i=typeNote;i<=typeReset;i++)            table[i]= FreeSmallEv;
  for (i=typeSysEx;i<=typeStream;i++)          table[i]= FreeSexEv;
  for (i=typePrivate;i<typeProcess; i++)       table[i]= FreePrivateEv;
  for (i=typeCtrl14b;i<=typeSeqNum;i++)        table[i]= FreeSmallEv;
  for (i=typeTextual;i<=typeCuePoint;i++)      table[i]= FreeSexEv;
  for (i=typeChanPrefix;i<=typeKeySign;i++)    table[i]= FreeSmallEv;
  for (i=typeReserved;i<=typeLastReserved;i++) table[i]= FreeUndefEv;
  table[typeQuarterFrame] = FreeSmallEv;
  table[typeSpecific]     = FreeSexEv;
  table[typeRcvAlarm]     = FreeSmallEv;
  table[typeApplAlarm]    = FreeSmallEv;
  
  table[typeProcess] 	= FreeProcessEv;
  table[typeDProcess] 	= FreeProcessEv;
  table[typeDead]       = FreeProcessEv;
}

/*__________________________________________________________________________________*/
static void InitCopyEvMth( CopyEvMethodPtr *table)
{
  int i;

  for (i=typeNote;i<=typeReset;i++)            table[i]= CopySmallEv;
  for (i=typeSysEx;i<=typeStream;i++)          table[i]= CopySexEv;
  for (i=typePrivate;i<typeProcess;i++)        table[i]= CopyPrivateEv;
  for (i=typeCtrl14b;i<=typeSeqNum;i++)        table[i]= CopySmallEv;
  for (i=typeTextual;i<=typeCuePoint;i++)      table[i]= CopySexEv;
  for (i=typeChanPrefix;i<=typeKeySign;i++)    table[i]= CopySmallEv;
  for (i=typeReserved;i<=typeLastReserved;i++) table[i]= CopyUndefEv;
  table[typeQuarterFrame] = CopySmallEv;
  table[typeSpecific]     = CopySexEv;
  table[typeRcvAlarm]     = CopySmallEv;
  table[typeApplAlarm]    = CopySmallEv;
  
  table[typeProcess] 	= CopyProcessEv;
  table[typeDProcess] 	= CopyProcessEv;
  table[typeDead]       = CopyProcessEv;  
}

/*__________________________________________________________________________________*/
static void InitCountFieldsMth( CountFieldsMethodPtr *table)
{
  int i;

  for(i=typeKeyOn;i<= typeCtrlChange; i++)    table[i]= Count2Fields;
  for(i=typeProgChange;i<=typeChanPress;i++)  table[i]= Count1Field;
  for(i=typePitchWheel;i<=typeSongPos;i++)    table[i]= Count2Fields;
  for(i=typeClock;i<=typeReset;i++)           table[i]= Count0Field;
  for(i=typeSysEx;i<=typeStream;i++)          table[i]= CountFSexEv;
  for(i=typePrivate;i<typeProcess;i++)        table[i]= Count4Fields;
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
  table[typeRcvAlarm]    = CountFUndefEv;
  table[typeApplAlarm]   = Count1Field;
  
  table[typeProcess] 	= CountFProcessEv;
  table[typeDProcess] 	= CountFProcessEv;
  table[typeDead]       = CountFProcessEv;
}

/*__________________________________________________________________________________*/
static void InitSetFieldMth (SetFieldMethodPtr *table)
{
  int i;

  for(i=typeNote; i<= typeReset;i++)          table[i]= SetFSmallEv;
  for(i=typeSysEx;i<=typeStream;i++)          table[i]= SetFSexEv;
  for(i=typePrivate;i<typeProcess;i++)        table[i]= SetFPrivateEv;
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
  table[typeRcvAlarm]     = SetFUndefEv;
  table[typeApplAlarm]    = SetFTempo;
  
  table[typeProcess] 	= SetFProcessEv;
  table[typeDProcess] 	= SetFProcessEv;
  table[typeDead]       = SetFProcessEv;
}

/*__________________________________________________________________________________*/
static void InitGetFieldMth (GetFieldMethodPtr *table)
{
  int i;

  for(i=typeNote;i<=typeReset;i++)            table[i]= GetFSmallEv;
  for(i=typeSysEx;i<=typeStream;i++)          table[i]= GetFSexEv;
  for(i=typePrivate;i<typeProcess;i++)        table[i]= GetFPrivateEv;
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
  table[typeRcvAlarm]     = GetFUndefEv;
  table[typeApplAlarm]    = GetFTempo;
  
  table[typeProcess] 	= GetFProcessEv;
  table[typeDProcess] 	= GetFProcessEv;
  table[typeDead]       = GetFProcessEv;
}

/*__________________________________________________________________________________*/
static void InitAddFieldMth (AddFieldMethodPtr *table)
{
  int i;
	
  for(i=typeNote;i<=typeReset;i++)            table[i]= AddNoField;
  for(i=typeSysEx;i<=typeStream;i++)          table[i]= AddFSexEv;
  for(i=typePrivate;i<=typeSeqNum;i++)        table[i]= AddNoField;
  for(i=typeTextual;i<=typeCuePoint;i++)      table[i]= AddFSexEv;
  for(i=typeChanPrefix;i<=typeKeySign;i++)    table[i]= AddNoField;
  for(i=typeReserved;i<=typeLastReserved;i++) table[i]= AddFUndefEv;
  table[typeSpecific] = AddFSexEv;
  table[typeRcvAlarm] = AddNoField;
  table[typeApplAlarm]= AddNoField;
  table[typeDead]     = AddNoField;
  
  table[typeProcess] = AddNoField;
  table[typeDProcess] = AddNoField;
}

/*__________________________________________________________________________________
  NEW EVENT METHODS			
  __________________________________________________________________________________*/
static MidiEvPtr NewUndefEv( lifo* unused1, short unused2)
{
	Debug( "NewEv : WRONG EVENT TYPE !!");
	return 0;
}
	
/*__________________________________________________________________________________*/
static MidiEvPtr NewSmallEv( lifo* fl, short typeNum)
{
	MidiEvPtr ev = MSNewCell( fl);
	if( ev) {
		Link(ev)= 0;
		Date(ev)= defaultTime;
		EvType(ev)= (uchar)typeNum;
		RefNum(ev)= 0xff;
		Chan(ev) = Port(ev) = 0;
		ev->info.longField = 0;
	}
	return ev;
}

/*__________________________________________________________________________________*/
static MidiEvPtr NewSexEv( lifo* fl, short typeNum)
{
	MidiEvPtr ev = MSNewCell( fl);
	if( ev) {
		MidiSTPtr ext= (MidiSTPtr)MSNewCell( fl);
		if (!ext) {
			MSFreeCell (ev, fl);
			return 0;
		}
		ext->val[0]= (long)ext;			/* creates a clear extension block */
		ext->val[1]= ext->val[2]= ext->val[3]= 0;
		
		Link(ev)= 0;					/* initialize the header           */
		Date(ev)= defaultTime;
		EvType(ev)= (uchar)typeNum;
		RefNum(ev)= 0xff;
		Chan(ev) = Port(ev) = 0;
		LinkSE(ev)= (MidiSEXPtr)ext;    /* link the extension block        */
	}
	return ev;
}

/*__________________________________________________________________________________*/
static MidiEvPtr NewPrivateEv( lifo* fl, short typeNum)
{
	MidiEvPtr ev = MSNewCell( fl);
	if( ev) {
		MidiSTPtr ext= (MidiSTPtr)MSNewCell(fl);
		if ( !ext) {
			MSFreeCell (ev, fl);
			return 0;
		}								   
		/* creates a clear extension block */
		ext->val[0]= ext->val[1]= ext->val[2]= ext->val[3]= 0;
		Link(ev)= 0;					/* initialize the header           */
		Date(ev)= defaultTime;
		EvType(ev)= (uchar)typeNum;
		RefNum(ev)= 0xff;
		Chan(ev) = Port(ev) = 0;
		LinkST(ev)= ext;				/* link the extension block        */
	}
	return ev;
}

/*__________________________________________________________________________________
  FREE EVENT METHODS
  __________________________________________________________________________________*/
static void FreeUndefEv( lifo* unused1, MidiEvPtr unused2)
{
	Debug( "FreeEv : WRONG EVENT TYPE !!");
}

/*__________________________________________________________________________________*/
static void FreeSmallEv( lifo* fl, MidiEvPtr e)
{
	MSFreeCell (e, fl);
}

/*__________________________________________________________________________________*/
static void FreeSexEv( lifo* fl, MidiEvPtr e)
{
	register MidiEvPtr f, c, n;

	f = c = (MidiEvPtr)LinkSE(e);
	MSFreeCell (e, fl);
	do {
		n = c->link;
		MSFreeCell (c, fl);
		c = n;
	} while (c != f);
}

/*__________________________________________________________________________________*/
static void FreePrivateEv( lifo* fl, MidiEvPtr e)
{
	MSFreeCell (LinkST(e), fl);
	MSFreeCell (e, fl);
}

/*__________________________________________________________________________________
  COPY EVENT METHODS					
  __________________________________________________________________________________*/
static MidiEvPtr CopyUndefEv( lifo* unused1, MidiEvPtr unused2)
{
	Debug( "CopyEv : WRONG EVENT TYPE !!");
	return 0;
}

/*__________________________________________________________________________________*/
static MidiEvPtr CopySmallEv( lifo* fl, MidiEvPtr ev)
{
	MidiEvPtr copy = MSNewCell( fl);
	if (copy) *copy = *ev;
	return copy;
}

/*__________________________________________________________________________________*/
static MidiEvPtr CopyPrivateEv( lifo* fl, MidiEvPtr ev)
{
	MidiEvPtr copy = MSNewCell( fl);
	if (copy) {
		MidiSTPtr ext = (MidiSTPtr)MSNewCell( fl);
		if (!ext) {
			MSFreeCell (copy, fl);
			return 0;
		}
		*copy = *ev;
		*ext = *LinkST(ev);
		LinkST(copy) = ext;
	}
	return copy;
}

/*__________________________________________________________________________________*/
static MidiEvPtr CopySexEv( lifo* fl, MidiEvPtr ev)
{
	MidiEvPtr copy; MidiSEXPtr ext, extCopy, previous;
	
	copy= CopyPrivateEv( fl, ev);			/* copy the event like a private event */
	if( copy) {
		extCopy= previous= LinkSE(copy);	/* get the last extension cell      */
		ext= Link(LinkSE(ev));				/* get the first cell to be copied	*/
		while( ext!= LinkSE(ev)) {			/* is it the last ?                 */
			extCopy= (MidiSEXPtr)CopySmallEv( fl, (MidiEvPtr)ext);
			if( extCopy) {					/* cell copy succes                 */
				Link(previous)= extCopy;	/* link to the previous             */
				previous= extCopy;			/* previous cell is the copy        */
			}
			else {							/* copy failed                      */
				Link(previous)=LinkSE(copy);/* restore the event consistency    */
				FreeSexEv( fl, copy);		/* and free the event               */
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
static void SetFUndefEv( MidiEvPtr unused2, unsigned long unused3, long unused4)
{
	Debug( "SetFEv : WRONG EVENT TYPE !!");
}

/*__________________________________________________________________________________*/
static void SetFSmallEv( MidiEvPtr e, unsigned long f, long v)
{
	if( f< 2)
		Data(e)[f]= (Byte)v;
	else if( f== 2)
		Dur(e)=(unsigned short)v;
}

/*__________________________________________________________________________________*/
static void SetF2_16Ev( MidiEvPtr e, unsigned long f, long v)
{
	if( f < 2) {
		e->info.shortFields[f]=(short)v;
	}
}

/*__________________________________________________________________________________*/
static void SetFTempo( MidiEvPtr e, unsigned long f, long v)
{
	if( !f) Tempo(e)= v;
}

/*__________________________________________________________________________________*/
static void SetFSMPTEOffset( MidiEvPtr e, unsigned long f, long v)
{
	if( f < 6) {
		const unsigned long * mask  = smpteMaskTbl;
		const char * shift = smpteShftTbl;
		v <<= shift[f];
		v &= mask[f];
		e->info.longField |= v;
	}
}

/*__________________________________________________________________________________*/
static void SetFTimeSign( MidiEvPtr e, unsigned long f, long v)
{
	if( f < 4) Data(e)[f]=(Byte)v;
}

/*__________________________________________________________________________________*/
static void SetFKeySign( MidiEvPtr e, unsigned long f, long v)
{
	if( f < 2) Data(e)[f]=(Byte)v;
}

/*__________________________________________________________________________________*/
static void SetFSexEv( MidiEvPtr e, unsigned long f, long v)
{
	MidiSEXPtr ext;

	ext = Link(LinkSE(e));              /* first event extension cell           */
	while( true) {
		if( ext == LinkSE(e)) {         /* if it's the last extension cell      */
			if( f < ext->data[11])		/* if the index is free                 */
				ext->data[f]=(Byte)v;	/* store the value                  */
			break;
		}
		if( f < kLenDatas) {			/* if the field is located in this cell */
			ext->data[f] = (Byte)v;     /* store the value                      */
			break;
		}
		f-= kLenDatas;					/* substract lenData to the field index */
		ext = ext->link;		        /* get the next extension cell and loop */
	}
}

/*__________________________________________________________________________________*/
static void SetFPrivateEv( MidiEvPtr e, unsigned long f, long v)
{
	if( f < 4) LinkST(e)->val[f] = v;
}


/*__________________________________________________________________________________
  GET FIELD METHODS					
  __________________________________________________________________________________*/
static long GetFUndefEv( MidiEvPtr unused2, unsigned long unused3)
{
	Debug( "GetField : WRONG EVENT TYPE !!");
	return kGetFieldError;
}

/*__________________________________________________________________________________*/
static long GetFSmallEv( MidiEvPtr e, unsigned long f)
{
	return (f < 2) ? Data(e)[f] : Dur(e);
}

/*__________________________________________________________________________________*/
static long GetFSexEv( MidiEvPtr e, unsigned long f)
{
	MidiSEXPtr ext;

	ext = Link(LinkSE(e));              /* first event extension cell           */
	while( ext != LinkSE(e)) {          /* while not the last cell              */
		if( f < kLenDatas)              /* if the field is located in this cell */
			return ext->data[f];        /* returns the value                    */
		f-= kLenDatas;                  /* substract lenData to the field index */
		ext = ext->link;                /* get the next extension cell          */
	}
                                        /* last extension cell                  */
	if( f < ext->data[11])              /* if the field is located in this cell */
		return ext->data[f];            /* returns the value                    */
                                        /* otherwise returns the last value     */
	return kGetFieldError;
}

/*__________________________________________________________________________________*/
static long GetFPrivateEv( MidiEvPtr e, unsigned long f)
{
	return (f < 4) ?  LinkST(e)->val[f] : kGetFieldError;
}

/*__________________________________________________________________________________*/
static long GetF2_16Ev( MidiEvPtr e, unsigned long f)
{
	return (f < 2) ?  e->info.shortFields[f] : kGetFieldError;
}

/*__________________________________________________________________________________*/
static long GetFTempo( MidiEvPtr e, unsigned long unused2)
{
	return Tempo(e);
}

/*__________________________________________________________________________________*/
static long GetFSMPTEOffset( MidiEvPtr e, unsigned long f)
{
	if ( f < 6) {
		const unsigned long * mask  = smpteMaskTbl;
		const char * shift = smpteShftTbl;
		return (e->info.tempo & mask[f]) >> shift[f];
	}
	return kGetFieldError;
}

/*__________________________________________________________________________________*/
static long GetFTimeSign( MidiEvPtr e, unsigned long f)
{
	return (f < 4) ? Data(e)[f] : kGetFieldError;
}

/*__________________________________________________________________________________*/
static long GetFKeySign( MidiEvPtr e, unsigned long f)
{
	return (f < 2) ? Data(e)[f] : kGetFieldError;
}



/*__________________________________________________________________________________
  COUNT FIELDS METHODS
  __________________________________________________________________________________*/
static long CountFUndefEv ( MidiEvPtr unused1)
{
	Debug( "CountField : WRONG EVENT TYPE !!");
	return kCountFieldsError;
}

/*__________________________________________________________________________________*/
static long Count0Field	    ( MidiEvPtr unused1)	{ return 0;}
static long Count1Field	    ( MidiEvPtr unused1)	{ return 1;}
static long Count2Fields	( MidiEvPtr unused1)	{ return 2;}
static long Count3Fields	( MidiEvPtr unused1)	{ return 3;}
static long Count4Fields	( MidiEvPtr unused1)	{ return 4;}
static long Count6Fields	( MidiEvPtr unused1)	{ return 6;}

/*__________________________________________________________________________________*/
static long CountFSexEv( MidiEvPtr e)
{
	long count= 0;
	MidiSEXPtr ext = Link(LinkSE(e));	/* first event extension              */
	while( ext != LinkSE(e)) {		    /* while not the last cell            */
		count+= kLenDatas;				/* add the data len to the count      */
		ext= ext->link;		            /* next extension cell                */
	}
	return count + ext->data[11];		/* return count + the data len of the */
										/* the last cell                      */
}


/*__________________________________________________________________________________
  ADD FIELD METHODS
  __________________________________________________________________________________*/
static void AddFUndefEv( lifo* unused1, MidiEvPtr unused2, long unused3)
{
	Debug( "AddField : WRONG EVENT TYPE !!");
}

/*__________________________________________________________________________________*/
static void AddNoField( lifo* unused1, MidiEvPtr unused2, long unused3)
{
}

/*__________________________________________________________________________________*/
static void AddFSexEv( lifo* fl, MidiEvPtr e, long v)
{
	MidiSEXPtr nouv;
	MidiSEXPtr ext = LinkSE(e);
	int i = ext->data[11];

	if( i < 11) {			              /* If there remains place       */
		ext->data[i] = (Byte)v;	          /* store the value              */
		ext->data[11]++;				  /* update the busy space count  */
	} else { 
		nouv = (MidiSEXPtr)MSNewCell(fl); /* add a new cell               */
		if( nouv) {
			ext->data[11] = (Byte)v;      /* store the value              */
			nouv->data[11] = 0;			  /* busy space count             */
			nouv->link= ext->link;		  /* link the new cell            */
			ext->link= nouv;
			LinkSE(e)= nouv;			  /* link header to the last cell */
		}
	}
}
