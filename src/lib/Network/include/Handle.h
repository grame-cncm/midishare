/*

  Copyright © Grame 2001
  Copyright © Mil Productions 2001

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

*/


#ifndef __Handle__
#define __Handle__


#if macintosh

#define HandlePtr(h)	*(h)

#else

typedef struct {
	void *	ptr;
	long	len;
} INetHandleRec, * INetHandle;

#define HandlePtr(h)		(h)->ptr
#define GetHandleSize(h)	(h)->len

#ifdef __cplusplus
extern "C" {
#endif

INetHandle 	INetNewHandle (long size);
void		INetDisposeHandle (INetHandle h);

#ifdef __cplusplus
}
#endif

#endif

#endif
