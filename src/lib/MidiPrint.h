/*
  MidiShare Project
  Copyright (C) Grame 2000

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifndef __MidiPrint__
#define __MidiPrint__

//______________________________________________
#ifdef __cplusplus
extern "C" {
#endif

void MidiPrintf (char *s);
void MidiPrintp (unsigned char *s);
void MidiPrintl (long val);
void MidiPrintH (long val);

#ifdef __cplusplus
}
#endif

#endif
