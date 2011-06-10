/*

  Copyright (c) Grame 1999-2010

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
