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

  modifications history:
   [08-09-99] DF - adaptation to the new memory management

*/


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "../kernel/msIOCtl.h"

#include "msAppls.h"
#include "msEvents.h"
#include "msMemory.h"
#include "msSeq.h"
#include "msTypes.h"

/*__________________________________________________________________________________*/
/* Task extension block                                                             */

typedef struct TTaskExt FAR *TTaskExtPtr;
typedef struct TTaskExt
{
    TaskPtr fun;
    long    arg1;
    long    arg2;
    long    arg3;
} TTaskExt;

/*__________________________________________________________________________________*/
/* Verification and Initialisation of the library          							*/

#define CHECK if (msfd < 0) MidiInstall()

#define NewAppl(size)		(TApplPtr)malloc( size)
#define FreeAppl(appl)		free(appl)

#define CALL(index,args) \
	 {int err; if (msfd < 0) MidiInstall();\
	 err = ioctl(msfd, (index), args); \
	 if (err < 0) { printf("ioctl failed index %d file %s line %d\n", index, __FILE__, __LINE__);  exit(-1); }}


/*__________________________________________________________________________________*/
/*  Globals variable declaration                                                    */

extern TClients   	gLClients ;
extern TClients*	gClients ;
extern int msfd;


/*__________________________________________________________________________________*/
/*  Function prototypes                                                   			*/

void makeAppl(TClientsPtr g, TApplPtr appl, short ref, MidiName n);
void MidiInstall();
void FreeList1( MidiEvPtr l);
MidiEvPtr MidiGetEvAux(short ref, int command);

