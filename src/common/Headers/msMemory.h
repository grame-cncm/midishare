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

#ifndef __Memory__
#define __Memory__

#include "msTypes.h"
#include "msDefs.h"

/*------------------------------------------------------------------------------*/
/*    constants definition                                                      */
/*------------------------------------------------------------------------------*/
#define freeSizeBlock  100                /* size of a block used to count the  */
                                          /* free events                        */

#define kLenEvent       16                /* len of a MidiShare event           */
#define kLenDatas       kLenEvent-4       /* len of a sysex extension data      */

#define typeLastReserved typeDead-1       /* last reserved event type           */

/*------------------------------------------------------------------------------*/
/*     data types                                                               */
/*------------------------------------------------------------------------------*/
typedef struct TMSMemory  FAR * MSMemoryPtr;

typedef MidiEvPtr (*NewEvMethodPtr)      (MSMemoryPtr g, short typeNum);
typedef MidiEvPtr (*CopyEvMethodPtr)     (MSMemoryPtr g, MidiEvPtr ev);
typedef void      (*FreeEvMethodPtr)     (MSMemoryPtr g, MidiEvPtr ev);
typedef void      (*SetFieldMethodPtr)   (MSMemoryPtr g, MidiEvPtr ev, unsigned long f, long v);
typedef long      (*GetFieldMethodPtr)   (MSMemoryPtr g, MidiEvPtr ev, long f);
typedef long      (*CountFieldsMethodPtr)(MidiEvPtr ev);
typedef long      (*AddFieldsMethodPtr)  (MSMemoryPtr g, MidiEvPtr ev, long v);


/*------------------------------------------------------------------------------*/
/*     data structures                                                          */
/*------------------------------------------------------------------------------*/
/*     extension block of a system exclusive event                              */
typedef union TSexEv FAR *TSexEvPtr;
typedef union TSexEv
{
    MidiSEXPtr std;
    struct {
        MidiSEXPtr  link;
        Byte        data[kLenDatas-1];  /* content of the sysex               */
        Byte        num;                /* last byte is reserved to store the */
                                        /* next free data byte index          */
    } FAR *last;
} TSexEv;

/*___________________________________*/
typedef struct EventMethods FAR *EventMethodsPtr;
typedef struct EventMethods {
    NewEvMethodPtr       newEv[256];        /* Allocation methods table */
    CopyEvMethodPtr      copyEv[256];       /* Copy methods table       */
    FreeEvMethodPtr      freeEv[256];       /* Free methods table       */
    SetFieldMethodPtr    setField[256];     /* SetField methods table   */
    GetFieldMethodPtr    getField[256];     /* GetField methods table   */
    CountFieldsMethodPtr countFields[256];  /* CountField methods table */
    AddFieldsMethodPtr   addField[256];     /* AddField methods table   */
} EventMethods;

/*___________________________________*/
typedef struct TSmpteDatas FAR *TSmpteDatasPtr;
typedef struct TSmpteDatas {       /* for smpte events management   */
    long    bfMask[6];
    long    bfShft[6];
    char    format[4];
} TSmpteDatas;

/*___________________________________*/
typedef struct MemBlock FAR *MemBlockPtr;
typedef struct MemBlock                    /* memory block structure          */
{
    MemBlockPtr nextblock;                 /* header points to the next block */
    TMidiEv     events;                    /* followed by MidiShare events    */
} MemBlock;

/*___________________________________*/
typedef struct TMSMemory
{
    MidiEvPtr       freeList;               /* available events list          */
    MemBlockPtr     blockList;              /* allocated block list           */
    ulong           desiredSpace;           /* amount of desired events       */
    ulong           allocatedSpace;         /* count of allocated events      */
    EventMethods    method;
    TSmpteDatas     smpte;                  /* for smpte events management    */
    long            active;                 /* active > 0 if memory is active */
} TMSMemory;

/*------------------------------------------------------------------------------*/
/*                             fields access macros                             */
/*------------------------------------------------------------------------------*/

#define FreeList(g)         &g->freeList
#define BlockList(g)        &g->blockList
#define Event(b)            &b->events

#define Smpte(g)            g->smpte
#define NewEvMeth(g)        g->method.newEv
#define CopyEvMeth(g)       g->method.copyEv
#define FreeEvMeth(g)       g->method.freeEv
#define SetFieldMeth(g)     g->method.setField
#define GetFieldMeth(g)     g->method.getField
#define CountFieldsMeth(g)  g->method.countFields
#define AddFieldMeth(g)     g->method.addField

/* Memory management functions         */
MSFunctionType(ulong) MSGrowSpace    (unsigned long nbev, MSMemoryPtr g);
MSFunctionType(ulong) MSDesiredSpace (MSMemoryPtr g);
MSFunctionType(ulong) MSFreeSpace    (MSMemoryPtr g);
MSFunctionType(ulong) MSTotalSpace   (MSMemoryPtr g);

/* private memory management functions */
void     InitMemory  (MSMemoryPtr g, ulong defaultSpace);
Boolean  OpenMemory  (MSMemoryPtr g);
void     CloseMemory (MSMemoryPtr g);

#endif
