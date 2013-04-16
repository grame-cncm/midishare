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

#ifndef __msInetLib__
#define __msInetLib__

#include "TInetControler.h"
#include "TWANControler.h"

#ifdef __cplusplus
extern "C" {
#endif

// Open Transport utilities
bool CheckOT ();
strPtr TCPOpen ();
void TCPClose ();

// Net controler utilities
void Disconnect (TWANControler *wan, TMidiRemote * remote);
void Connect (TWANControler *wan, Str255 remote);
bool SetDriverMode (TInetControler *ctrl, bool on);

// Misc.
char * HostName ();

#ifdef __cplusplus
}

#endif

#endif
