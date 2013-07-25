/*
  MidiShare Project
  Copyright (C) Grame 1999

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifndef __mem__
#define __mem__

/*------------------------------------------------------------------------------*/
/*                               memory management                              */
/*------------------------------------------------------------------------------*/
/* defines a memory block maximum size: when a client application requests 
   for a larger block, the allocated memory is splitted in smaller blocks */
typedef enum {	
        kStdMemory,        /* no constraints on the allocated memory  */
                           /* it should only be accessed and owned by the calling process */

        kSharedMemory,     /* the allocated memory should be shared by the kernel and     */
                           /* all the client applications.                                */

        kernelSharedMemory /* the allocated memory should be shared by the kernel and     */
                           /* the calling process                                         */
} MemoryType;

void*	AllocateMemory (MemoryType type, unsigned long size);
void	DisposeMemory  (void* memPtr);

#endif
