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

static void IOCTL(int index);
#ifndef MODULE
#include "MidiShare.h"
#else
#include "MidiShareKernel.h"
#endif

#include "TDClient.h"

char *testDev = "/dev/TestDrivers";
int   devID = -1;

/*____________________________________________________________________*/
static void OpenDev()
{	
  devID = open(testDev, 0);
  if (devID < 0) {
	printf("Can't open %s\n", testDev);
	exit(-1);
  }
}

/*____________________________________________________________________*/
static void IOCTL(int index) {
	int err;
	if (devID < 0) OpenDev();
	err = ioctl(devID, index);
	if (err < 0) printf("ioctl #%d failed: %d\n", index, err); 
}

/*____________________________________________________________________*/
main()
{
	printf ("\nMidiShare drivers test.\n");
	printf (  "================================\n");
	
	if (MidiGetVersion() < 180) {
		printf ("Drivers management not implemented\n");
		exit(-1);
	}
	
	OpenDev();

	FEnvironment();
	FOpenClose();
	FWakeupSleep();
	FInfos();
	FConnections();
	FSlots();
	FSendingAndReceiving();

	close(devID);
	printf ("\nEnd of kernel events test.\n");
	return 0;
}
