/*

  Copyright © Grame 2000

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

#include <Files.h>
#include <Folders.h>
#include <Memory.h>
#include <Resources.h>
#include <string.h>

#include "DriverLoader.h"
#include "msDrvFun.h"

/* ----------------------------------*/
/* constants definitions             */
#define MidiShareDrvRef		127
#define MidiShareDrvFolder	"\pMidiShare Folder"
#define maxMsgSize			26
#define	MSDriverType		'Mshd'
#define	InitRsrcType		'INIT'
#define	InitRsrcID			0


/* ----------------------------------*/
/* some macros                       */
#define Directory(attr)		((attr) & 0x10)
#define	DriverFlag(pb)		(pb.hFileInfo.ioFlFndrInfo.fdType)

/* ----------------------------------*/
/* functions declarations            */
typedef void (* DriverInitPtr) ();

static DriverListPtr AllocateDriver ();
static DriverInitPtr LoadInitRsrc (FSSpec *spec, long * memSize);
static DriverListPtr Load1Driver (CInfoPBRec * pb);
static void 		 Dispose1Driver (TMSGlobalPtr g, DriverListPtr drv);

/* -----------------------------------------------------------------------------*/
/* Loader external functions                                                    */
/* -----------------------------------------------------------------------------*/
DriverListPtr LoadDrivers ()
{
	CInfoPBRec pb; DriverListPtr loaded, drv = 0;
	FSSpec spec; short i, n; OSErr err;
	
	if (FindDriversFolder (&spec, &n)) {
		for (i=1; i<=n; i++) {
			err = GetCatInfos (&pb, &spec, i);
			if ((err == noErr) && 
				!Directory(pb.hFileInfo.ioFlAttrib) &&
				(DriverFlag(pb) == MSDriverType)) {
				loaded = Load1Driver (&pb);
				if (loaded) {
					loaded->next = drv;
					drv = loaded;
				}
			}
		}
	}
	return drv;
}

/* -----------------------------------------------------------------------------*/
void UnloadDrivers (TMSGlobalPtr g, DriverListPtr drv)
{
	DriverListPtr next;
	while (drv) {
		next = drv->next;
		Dispose1Driver (g, drv);
		drv = next;
	}
}

/* -----------------------------------------------------------------------------*/
Boolean FindDriversFolder (FSSpec *spec, short *numFiles) 
{
	CInfoPBRec paramBlock; 
	unsigned char * name = MidiShareDrvFolder;
	OSErr err; long sysID; short vrefnum;
	err = FindFolder(kOnSystemDisk, kSystemFolderType, false, &vrefnum, &sysID);
	if (err != noErr) return false;
	
	err= FSMakeFSSpec (vrefnum, sysID, name, spec);
	if (err != noErr) return false;
	
	err= GetCatInfos (&paramBlock, spec, 0);
	if (err != noErr) return false;
	if (Directory(paramBlock.hFileInfo.ioFlAttrib)) {
		*numFiles = paramBlock.dirInfo.ioDrNmFls;
		spec->parID = paramBlock.dirInfo.ioDrDirID;
		return true;
	}
	return false;
}

/* -----------------------------------------------------------------------------*/
static void Dispose1Driver (TMSGlobalPtr g, DriverListPtr drv)
{
	Ptr mem = drv->memory; TClientsPtr clients = Clients(g);
	long low = (long)mem, high = low + drv->memSize;
	
	if (mem) {
		short i, ref, n = MSCountDrivers(clients);
		for (i=1; i<=n; i++) {
			ref = MSGetIndDriver (i, clients);
			if (ref != MIDIerrIndex) {
				long wadr = (long)Wakeup(clients->appls[ref]);
				if ((wadr > low) && (wadr < high))
					MSUnregisterDriver (ref, g);
			}
		}
		DisposePtr (mem);
	}
	DisposePtr ((Ptr)drv);
}

/* -----------------------------------------------------------------------------*/
static DriverListPtr Load1Driver (CInfoPBRec * pb)
{
	FSSpec spec; OSErr err; DriverInitPtr fInit;
	DriverListPtr drv = AllocateDriver ();
	if (!drv) return 0;

	err = FSMakeFSSpec(pb->hFileInfo.ioVRefNum, pb->hFileInfo.ioFlParID, 
						pb->hFileInfo.ioNamePtr, &spec);
	if (err != noErr) goto err;
	fInit = LoadInitRsrc (&spec, &drv->memSize);
	if (!fInit) goto err;
	
	drv->memory = (Ptr)fInit;
	fInit();
	return drv;

err:
	DisposePtr ((Ptr)drv);
	return 0;
}

/* -----------------------------------------------------------------------------*/
/* utilities                                                                    */
/* -----------------------------------------------------------------------------*/
static DriverInitPtr LoadInitRsrc (FSSpec *spec, long * memSize)
{
	Handle rh; short ref, current; 
	Ptr mem = 0;
	*memSize = 0;
	
	current = CurResFile();
	ref = FSpOpenResFile(spec, fsRdPerm);
	if (ref == -1) return 0;

	UseResFile (ref);
	rh = GetResource(InitRsrcType, InitRsrcID);
	if (rh) {
		Size size = InlineGetHandleSize (rh);
		mem = NewPtrSys (size);
		if (mem) {
			memmove (mem, *rh, size);
			*memSize = size;
		}
		ReleaseResource (rh);
	}
	UseResFile (current);
	return (DriverInitPtr)mem;
}

/* -----------------------------------------------------------------------------*/
OSErr GetCatInfos(CInfoPBRec *paramBlock, FSSpec *spec, short index)
{
	paramBlock->hFileInfo.ioCompletion	= 0;
	paramBlock->hFileInfo.ioNamePtr		= spec->name;
	paramBlock->hFileInfo.ioVRefNum		= spec->vRefNum;
	paramBlock->hFileInfo.ioFDirIndex	= index;
	paramBlock->hFileInfo.ioDirID		= spec->parID;
	return PBGetCatInfoSync (paramBlock);
}

/* -----------------------------------------------------------------------------*/
static DriverListPtr AllocateDriver () 
{
	DriverListPtr drv = (DriverListPtr)NewPtrSys (sizeof(DriverList));
	if (drv) {
		drv->next = 0;
		drv->memory = 0;
	}
	return drv;
}
