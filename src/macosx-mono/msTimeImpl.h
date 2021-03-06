/*
  MidiShare Project
  Copyright (C) Grame 2004

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifndef __msTimeImpl__
#define __msTimeImpl__

#include "msKernel.h"

void OpenAudioTime(TMSGlobalPtr g);
void CloseAudioTime(TMSGlobalPtr g);

void OpenTimer(TMSGlobalPtr g);
void CloseTimer(TMSGlobalPtr g);

#endif
