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

#include <stdio.h>

static void IOCTL(int index, short arg);
#include "TFClient.h"

char *testDev = "/dev/TestFunctions";
int   devID = -1;
enum { false, true };

/*____________________________________________________________________*/
static void OpenDev()
{	
	devID = open(testDev, 0);
	if (devID < 0) {
		printf("Can't open %s (id = %d)\n", testDev, devID);
		exit(-1);
  	}
}

/*____________________________________________________________________*/
static void IOCTL(int index, short arg) {
	int err;
	if (devID < 0) OpenDev();
	err = ioctl(devID, index, arg);
	if (err < 0) printf("ioctl #%d failed: %d\n", index, err); 
}

/*____________________________________________________________________*/
main()
{
	short r;
	printf ("\nMidiShare kernel functions test.\n");
	printf (  "================================\n");
	
	OpenDev();
	FEnvironment(); fflush( stdout);
//	FOpenClose(); fflush( stdout);
//	FApplConfiguration(); fflush( stdout);
//	FConnections(true); fflush( stdout);
//	FEvents(true); fflush( stdout);
//	FSequences(true); fflush( stdout);
//	FTime(); fflush( stdout);
//	FSending(); fflush( stdout); 
//							FMail(); fflush( stdout);
//	FTasks(true); fflush( stdout);
//	FSynchro(); fflush( stdout);
//	FTolerance(); fflush( stdout);
//	FNoMem(); fflush( stdout);

	FClose ();
	close(devID);
	printf ("\nEnd of kernel functions test.\n");
	return 0;
}
