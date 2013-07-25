/*
  MidiShare Project
  Copyright (C) Grame 2000

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/


#ifndef __msDrvUtils__
#define __msDrvUtils__

#ifdef __cplusplus
extern "C" {
#endif

Boolean 	GetDriversState (void *buff, long buffsize);
long 		GetDriversStateSize ();
void 		SetDriversState (void * ptr, long size);

Boolean		Get1DriverState (short refNum, short slotsCount, void *buff, long buffsize);
long 		Get1DriverStateSize (short slotsCount);
void 		Set1DriverState (short refNum, short slotsCount, void * ptr, long size);

#ifdef __cplusplus
}
#endif

#endif
