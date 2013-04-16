/*
  MidiShare Project
  Copyright (C) Grame 2001
  Copyright (C) Mil Productions 2001

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/


#ifndef __Handle__
#define __Handle__


#if macintosh

#include <Memory.h>

typedef Handle		INetHandle;

#define HandlePtr(h)	*(h)
#define 	INetNewHandle 		NewHandle
#define		INetDisposeHandle 	DisposeHandle

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
