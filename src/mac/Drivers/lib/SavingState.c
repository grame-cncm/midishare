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

#include <Errors.h>
#include <Devices.h>
#include <Files.h>
#include <Folders.h>
#include <MacMemory.h>
#include <Script.h>

#include "MidiShare.h"
#include "msDrvUtils.h"
#include "SavingState.h"

#define Directory(attr)		((attr) & 0x10)

/*__________________________________________________________________________*/
/* saving and restoring state                                               */
/*__________________________________________________________________________*/
/* internal functions                                                       */
static OSErr GetCatInfos(CInfoPBRec *paramBlock, FSSpec *spec, short index)
{
	paramBlock->hFileInfo.ioCompletion	= 0;
	paramBlock->hFileInfo.ioNamePtr		= spec->name;
	paramBlock->hFileInfo.ioVRefNum		= spec->vRefNum;
	paramBlock->hFileInfo.ioFDirIndex	= index;
	paramBlock->hFileInfo.ioDirID		= spec->parID;
	return PBGetCatInfoSync (paramBlock);
}

static long GetFileSize (FSSpec * spec)
{
	CInfoPBRec 	paramBlock;
	OSErr 		err;
	
	err= GetCatInfos (&paramBlock, spec, 0);
	if (err== noErr)
		return paramBlock.hFileInfo.ioFlLgLen;
	return 0;
}

/*__________________________________________________________________________*/
/* external functions                                                       */
/*__________________________________________________________________________*/
OSErr FindMidiShareFolder (Boolean create, short * vRefNum, long * dirID)
{
	CInfoPBRec paramBlock; FSSpec spec; OSErr err;
	unsigned char * name = MidiShareDrvFolder;

	err = FindFolder(kOnSystemDisk, kSystemFolderType, false, vRefNum, dirID);
	if (err != noErr) return err;
	
	err= FSMakeFSSpec (*vRefNum, *dirID, name, &spec);
	if (err == noErr) {
		err= GetCatInfos (&paramBlock, &spec, 0);
		if (err != noErr) return err;
		if (Directory(paramBlock.hFileInfo.ioFlAttrib)) {
			*dirID = paramBlock.dirInfo.ioDrDirID;
		}
		else return fnfErr;
	}
	else if (err == fnfErr) {
		if (create)
			err = FSpDirCreate (&spec, smSystemScript, dirID);
	}
	return err;
}

/*__________________________________________________________________________*/
Boolean SaveDriverState (short refnum, StringPtr file, OSType creator, OSType type)
{
	FSSpec spec; OSErr err; TDriverInfos dInfos;
	long size, dirID; short vrefNum, ref;
	Ptr ptr;
	
	if (FindMidiShareFolder (true, &vrefNum, &dirID) != noErr) return false;
	if (!MidiGetDriverInfos (refnum, &dInfos)) return false;

	size = Get1DriverStateSize (dInfos.slots);
	if (!size) return true;
	
	ptr = NewPtrSys(size);
	if (!ptr) return false;
		
	Get1DriverState (refnum, dInfos.slots, ptr, size);
	err = FSMakeFSSpec(vrefNum, dirID, file, &spec);
	if (err == fnfErr)
		err = FSpCreate (&spec, creator, type, smSystemScript);
	if (err != noErr) goto err;
		err = FSpOpenDF (&spec, fsWrPerm, &ref);
	if (err != noErr) goto err;
		err = FSWrite (ref, &size, ptr);
	FSClose (ref);
	DisposePtr (ptr);
	return err == noErr;

err:
	DisposePtr (ptr);
	return false;
}

/*__________________________________________________________________________*/
void RestoreDriverState	(short refnum, StringPtr file)
{
	long size, dirID; short vrefNum, ref; Ptr ptr;
	OSErr err; FSSpec spec; TDriverInfos dInfos;

	if (FindMidiShareFolder (false, &vrefNum, &dirID) != noErr) return;
	if (!MidiGetDriverInfos (refnum, &dInfos)) return;

	FSMakeFSSpec(vrefNum, dirID, file, &spec);
	err = FSpOpenDF (&spec, fsRdPerm, &ref);
	if (err != noErr) return;

	size = GetFileSize (&spec);
	if (size) {
		ptr = NewPtrSys (size);
		if (ptr) {
			err = FSRead (ref, &size, ptr);
			if (err == noErr)
				Set1DriverState (refnum, dInfos.slots, ptr, size);
			DisposePtr (ptr);
		}
	}
	FSClose (ref);
}

