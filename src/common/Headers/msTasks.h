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

#ifndef __msTasks__
#define __msTasks__

#include "msDefs.h"
#include "msTypes.h"
#include "msAppls.h"
#include "msMemory.h"
#include "msSorter.h"


/*__________________________________________________________________________________*/
/* Task extension block                                                             */
typedef struct TTaskExt FAR *TTaskExtPtr;
typedef struct TTaskExt
{
    TaskPtr fun;
    long    arg1;
    long    arg2;
    long    arg3;
} TTaskExt;


/*__________________________________________________________________________________*/
MSFunctionType(void)      MSCall  (TaskPtr task, unsigned long date, short r, long a1,long a2,long a3, 
                                   MSMemoryPtr g, TsorterBlockPtr sb);
MSFunctionType(MidiEvPtr) MSTask  (TaskPtr task, unsigned long date, short r, long a1,long a2,long a3,  
                                   MSMemoryPtr g, TsorterBlockPtr sb);
MSFunctionType(MidiEvPtr) MSDTask (TaskPtr task, unsigned long date, short r, long a1,long a2,long a3,  
                                   MSMemoryPtr g, TsorterBlockPtr sb);

MSFunctionType(void)      MSForgetTask	(MidiEvPtr *e);
MSFunctionType(long)      MSCountDTasks	(short refnum, TClientsPtr g);
MSFunctionType(void)      MSFlushDTasks	(short refnum, TClientsPtr g);
MSFunctionType(void)      MSExec1DTask	(short refnum, TClientsPtr g, long currtime);

#endif
