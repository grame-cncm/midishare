/*
  MidiShare Project
  Copyright (C) Grame 1999

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifndef __msSync__
#define __msSync__

//#include "msTypes.h"
#include "MidiShare.h"


#ifdef __Macintosh__
# if defined(__POWERPC__) || defined(__i386__) || __x86_64__

# else

void    INT_OFF(void) = {0x40E7, 0x007C, 0x0700};
void    INT_ON(void)  = {0x46DF};

# endif
#endif /* __Macintosh__ */

/*------------------------------------------------------------------------------*/
/*                             synchronization functions                        */
/*------------------------------------------------------------------------------*/
Boolean MSCompareAndSwap (void** adr, void* compareTo, void* swapWith);

#endif
