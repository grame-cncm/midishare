/*

  Copyright © Grame 1999

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
  grame@rd.grame.fr

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

