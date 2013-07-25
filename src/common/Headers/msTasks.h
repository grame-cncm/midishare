/*
  MidiShare Project
  Copyright (C) Grame 1999

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

  modifications history:
   [08-09-99] DF - adaptation to the new memory management

*/

#ifndef __msTasks__
#define __msTasks__

#include "lflifo.h"
#include "lffifo.h"
#include "msDefs.h"
#include "msTypes.h"
#include "msAppls.h"

/*__________________________________________________________________________________*/
/* Task extension block                                                             */
typedef struct TTaskExt *TTaskExtPtr;
typedef struct TTaskExt
{
    TaskPtr fun;
    void*   arg1;
    void*   arg2;
    void*   arg3;
} TTaskExt;


/*__________________________________________________________________________________*/
MSFunctionType(void)      MSCall  (TaskPtr task, unsigned long date, short r, void* a1, void* a2, void* a3, 
                                   lifo* freelist, fifo* schedlist);
MSFunctionType(MidiEvPtr) MSTask  (TaskPtr task, unsigned long date, short r, void* a1, void* a2, void* a3,  
                                   lifo* freelist, fifo* schedlist);
MSFunctionType(MidiEvPtr) MSDTask (TaskPtr task, unsigned long date, short r, void* a1, void* a2, void* a3,  
                                   lifo* freelist, fifo* schedlist);

MSFunctionType(void)      MSForgetTask	(MidiEvPtr *e);
MSFunctionType(atomic_long) MSCountDTasks	(short refnum, TClientsPtr g);
MSFunctionType(void)      MSFlushDTasks	(short refnum, TClientsPtr g);
MSFunctionType(void)      MSExec1DTask	(short refnum, TClientsPtr g);

/*__________________________________________________________________________________*/
Boolean ForgetTaskSync (MidiEvPtr * taskPtr, MidiEvPtr content);


#endif
