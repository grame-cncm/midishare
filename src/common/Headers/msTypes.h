/*
  MidiShare Project
  Copyright (C) Grame 1999

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
  
  modifications history:
   [08-09-99] DF - struct cell added
   [13-09-99] DF - 'PascalNames' definition moved to platform dependent header
   [24-09-10] DF - 64 bits support: definitions revised 
*/

#ifndef __msTypes__
#define __msTypes__

#if defined(__Macintosh__) && !defined(__POWERPC__) && !defined(__i386__) && !__x86_64__
# define __Pascal_fun__
#endif

#ifdef __Macintosh__
# ifndef __MacOSX__
# include <mactypes.h>
# define __Pascal_alarm__
# endif

#elif defined(WIN32)
# include <windows.h>

#elif defined(__linux__)
# include <linux/types.h>
#endif

#ifdef __Pascal_fun__
# define MSFunctionType(type)   pascal type
#else
# define MSFunctionType(type)   type
#endif

enum { false, true };

//typedef unsigned char Boolean;
//typedef unsigned char Byte;
//typedef char * Ptr;
//typedef unsigned long ulong;
//typedef unsigned char uchar;

#endif
