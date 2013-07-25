/*
  MidiShare Project
  Copyright (C) Grame 1999

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "msAlarms.h"
#include "msExtern.h"


/*_____________________________________________________________________________________*/
void CallAlarm (short ref, int alarmCode, TClientsPtr g)
{
	TApplPtr *applPtr, appl;
	long code; short i;

	code   = ref;
	code <<= 16;
	code  |= alarmCode & 0xffff;                      /* alarm arguments                 */
	applPtr = g->appls;                               /* points the applications table   */
	for( i=0; i<MaxAppls; i++) {                      /* for each application            */
		appl = *applPtr++;
		if( appl && appl->applAlarm) {                /* if the alarm exists             */
			CallApplAlarm (appl->context, appl->applAlarm, appl->refNum, code);
		}
	}
}
