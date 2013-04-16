/*
  MidiShare Project
  Copyright (C) Grame 1999

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "msSmpte.h"

/*__________________________________________________________________________________*/
/*		- MidiGetSyncInfo - Lit les information de synchronisation			*/
/*__________________________________________________________________________________*/
MSFunctionType(void) MSGetSyncInfo (SyncInfoPtr p, TMSGlobalPtr g)
{
	/* not yet implemented */
}

/*__________________________________________________________________________________*/
/*		- MidiSetSyncMode - Change le mode de synchro				*/
/*__________________________________________________________________________________*/
MSFunctionType(void) MSSetSyncMode (unsigned short mode, TMSGlobalPtr g)
{
	/* not yet implemented */
}

/*__________________________________________________________________________________*/
/*		- MidiGetExtTime - Donne le temps externe (en ms)			*/
/*__________________________________________________________________________________*/
MSFunctionType(long) MSGetExtTime (TMSGlobalPtr g)
{
	/* not yet implemented */
	return 0;
}

/*__________________________________________________________________________________*/
/*		- MidiInt2ExtTime - Convertit le temps interne en temps externe (en ms)		*/
/*__________________________________________________________________________________*/
MSFunctionType(long) MSInt2ExtTime (long time, TMSGlobalPtr g)
{
	/* not yet implemented */
	return 0;
}

/*__________________________________________________________________________________*/
/*		- MidiExt2IntTime - Convertit le temps externe en temps interne (en ms)		*/
/*__________________________________________________________________________________*/
MSFunctionType(long) MSExt2IntTime (long time, TMSGlobalPtr g)
{
	/* not yet implemented */
	return 0;
}

