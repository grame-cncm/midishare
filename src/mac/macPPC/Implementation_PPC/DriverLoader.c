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
  grame@grame.fr

*/

#include <Errors.h>
#include <Files.h>
#include <Folders.h>
#include <Memory.h>
#include <Processes.h>
#include <Resources.h>
#include <string.h>

#include "DriverLoader.h"
#include "msDrvFun.h"

/* ----------------------------------*/
/* constants definitions             */
#define MidiShareDrvRef		127
#define MidiShareDrvFolder	"\pMidiShare Folder"
#define maxMsgSize			26
#define	MSDriverType		'APPL'
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
static DriverListPtr Load1Driver (CInfoPBRec * pb);

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
static DriverListPtr Load1Driver (CInfoPBRec * pb)
{
	FSSpec spec; OSErr err; 
	LaunchParamBlockRec l;
	DriverListPtr drv = AllocateDriver ();
	if (!drv) return 0;

	err = FSMakeFSSpec(pb->hFileInfo.ioVRefNum, pb->hFileInfo.ioFlParID, 
						pb->hFileInfo.ioNamePtr, &spec);
	if (err != noErr) return 0;

	l.launchBlockID = extendedBlock;
	l.launchEPBLength = extendedBlockLen;
	l.launchFileFlags = 0;
	l.launchControlFlags = launchNoFileFlags 
						 | launchContinue 
						 | launchUseMinimum;
	l.launchAppSpec = &spec;
	l.launchAppParameters = 0;
	
	err = LaunchApplication (&l);
	if (err == memFullErr) {
	}
	return 0;
}

/* -----------------------------------------------------------------------------*/
/* utilities                                                                    */
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
