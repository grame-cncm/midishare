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

#ifndef __mem__
#define __mem__

#include "msTypes.h"

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

FarPtr(void) AllocateMemory (MemoryType type, unsigned long size);
void         DisposeMemory  (FarPtr(void) memPtr);

#endif
