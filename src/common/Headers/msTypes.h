/*

  Copyright © Grame 1999-2002

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
  grame@grame.fr
  
  modifications history:
   [08-09-99] DF - struct cell added
   [13-09-99] DF - 'PascalNames' definition moved to platform dependent header
   [20-08-02] DF - types definition reorganized

*/

#ifndef __msTypes__
#define __msTypes__

typedef unsigned char uchar;

#define FAR
#define FarPtr(type)  type FAR *

#if defined(macintosh) && !defined(__MacOSX__)
#	include <mactypes.h> 
#	define MSFunctionType(type)   pascal type
#else 
#	define MSFunctionType(type)   type
#endif

#if defined(__MacOSX__)
	typedef unsigned char Boolean;
	typedef unsigned char Byte;
	typedef char * Ptr;
	enum { false, true };

#elif defined(WIN32)
#	include <windows.h>
	typedef unsigned char Byte;
	typedef char * Ptr;
	typedef unsigned char Boolean;
	enum { false = 0, true };

#elif defined(linux)
#	include <linux/types.h>

	typedef unsigned char Byte;
	typedef char * Ptr;
	typedef unsigned char Boolean;
	enum { false = 0, true };

#else
#	error "msTypes.h : target os undefined"
#endif

#endif
