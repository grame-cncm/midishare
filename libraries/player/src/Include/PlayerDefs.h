/*

  Copyright © Grame 1996-2005

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

*/


#ifndef __PlayerDefs__
#define __PlayerDefs__

#define nil 0

#if defined(linux) || defined(__MACH__) // for linux and Mac OS X
	#define true 1
	#define false 0
	#define errno 
	#define FAR
	#define NEAR
	#define EXPORT
	#define MSALARMAPI
	#define MFAPI

#elif defined(mac_classic)			 // Mac OS up to 9.x
	#define MidiSharePPC_68k
	#define __SupportOldSTDef__

#elif defined(WIN32)				// windows 32 bits OS
	#include <windows.h>
	#define __SupportOldSTDef__ 
	#define  EXPORT  __declspec(dllexport)
	#define  MFAPI
	#define __DEBUG__ 0
	#define true 1
	#define false 0

#elif defined(WIN16)				// windows 16 bits OS
	#include <windows.h>
	#define true 1
	#define false 0
	#define EXPORT WINAPI _export
	#define MFAPI WINAPI _export

#else
#error "architecture undefined"
#endif

#endif
