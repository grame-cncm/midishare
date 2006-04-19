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
   [08-09-99] DF - struct cell added
   [13-09-99] DF - 'PascalNames' definition moved to platform dependent header

*/

#ifndef __msTypes__
#define __msTypes__

#ifdef __Macintosh__
#ifdef __MacOSX__
	typedef unsigned char Boolean;
	typedef unsigned char Byte;
	typedef char * Ptr;
	enum { false, true };
#else
# include <mactypes.h> 
#endif 
                                   
# if defined(__POWERPC__) || defined(__i686__)
# define MSFunctionType(type)   type
# define FAR
# else 
# define MSFunctionType(type)   pascal type
# define FAR
# endif

typedef unsigned long ulong;

#else
# define MSFunctionType(type)   type
#endif

#ifdef WIN32
# include <windows.h>
	typedef unsigned char Byte;
	typedef char * Ptr;
	typedef unsigned char Boolean;
	typedef unsigned long ulong;
	enum { false = 0, true };
#endif

#ifdef __linux__
#	include <linux/types.h>
#	define FAR

	typedef unsigned char Byte;
	typedef char * Ptr;
	typedef unsigned char Boolean;
	enum { false = 0, true };
#endif

typedef unsigned char uchar;
#define FarPtr(type)  type FAR *


#endif
