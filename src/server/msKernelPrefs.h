/*

  Copyright © Grame 2002

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


#ifndef __msKernelPrefs__
#define __msKernelPrefs__

#include "msAppls.h"

enum	{
 	kTimeModeDefault,   //
	kTimeModeRTC,       // only supported on linux
	kTimeModeAudio,     // 
	kTimeModeMMSystem   // only supported on windows
};

#define MaxLogName	512
typedef struct {
	unsigned long memory;
	int           timeMode;
	short         timeRes;    // time resolution in ms
	short         drvCount;
	char *        drivers[MaxDrivers];
	char          logfile[MaxLogName];
} msKernelPrefs;

//________________________________________________________________________
msKernelPrefs * ReadPrefs   ();
void            AdjustPrefs (msKernelPrefs * prefs, int argc, char *argv[]);
char *          DrvName     (msKernelPrefs * prefs, short index);

#endif

