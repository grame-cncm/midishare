/*
  MidiShare Project
  Copyright (C) Grame 2000

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
  
*/


#ifndef __msPrefs__
#define __msPrefs__

#include "MidiShare.h"

enum { kTimerTime=1, kAudioTime };

//_________________________________________________________
unsigned long LoadSpace ();
unsigned long LoadBufferSize();
unsigned long LoadTimeMode();
unsigned long LoadTimeRes();

void LoadDriverName(char* name, unsigned long size);
unsigned short CountDrivers();
Boolean GetDriver(short index, char *dst, short bufsize);

void Report (const char* what, const char* obj, const char* reason);
void ReportN (const char* what, const char* obj, long num);

#endif

