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
  
  modification history:
   [08-09-99] fifo functions moved to lffifoh
              mem allocation functions moved to mem.c

*/

#include <Devices.h>
#include <Errors.h>
#include <MacMemory.h>
#include <Script.h>

#include "DriverLoader.h" 
#include "msDrvUtils.h"
#include "msExtern.h"

#define stateFile  "\pMidiShare state"
enum { kStateCreator = 'Mshr', kStateType = 'msst' };


/*__________________________________________________________________________*/
/* saving and restoring state                                               */
/*__________________________________________________________________________*/
/* internal functions                                                       */
static long GetFileSize (FSSpec * spec)
{
	CInfoPBRec 	paramBlock;
	OSErr 		err;
	
	err= GetCatInfos (&paramBlock, spec, 0);
	if (err== noErr)
		return paramBlock.hFileInfo.ioFlLgLen;
	return 0;
}

static OSErr OpenFile (FSSpec * spec, char perm, short * ref)
{
	HParamBlockRec p; OSErr err;
	p.ioParam.ioCompletion = 0;
	p.ioParam.ioNamePtr = spec->name;	
	p.ioParam.ioVRefNum = spec->vRefNum;
	p.ioParam.ioPermssn = perm;
	p.fileParam.ioDirID   = spec->parID;
	err = PBHOpenDFSync(&p);
	if (err == noErr) *ref = p.ioParam.ioRefNum;
	return err;
}

static OSErr ReadFile (short ref, long *size, Ptr buff)
{
	ParamBlockRec p; OSErr err;
	p.ioParam.ioCompletion = 0;
	p.ioParam.ioRefNum = ref;	
	p.ioParam.ioBuffer = buff;
	p.ioParam.ioReqCount = *size;
	p.ioParam.ioPosMode  = fsFromStart;
	p.ioParam.ioPosOffset = 0;
	err = PBReadSync(&p);
	*size = p.ioParam.ioActCount;
	return err;
}

static OSErr WriteFile (short ref, long *size, Ptr buff)
{
	ParamBlockRec p; OSErr err;
	p.ioParam.ioCompletion = 0;
	p.ioParam.ioRefNum = ref;	
	p.ioParam.ioBuffer = buff;
	p.ioParam.ioReqCount = *size;
	p.ioParam.ioPosMode  = fsFromStart;
	p.ioParam.ioPosOffset = 0;
	err = PBWriteSync(&p);
	*size = p.ioParam.ioActCount;
	return err;
}

static void CloseFile (short ref)
{
	ParamBlockRec p;
	p.ioParam.ioCompletion = 0;
	p.ioParam.ioRefNum = ref;	
	PBCloseSync(&p);
}

/*__________________________________________________________________________*/
/* external functions                                                       */
/*__________________________________________________________________________*/
void SaveState (TMSGlobalPtr g)
{
	short n, ref; FSSpec spec, file; OSErr err;
	long size = getDriversStateSize ();
	Ptr ptr = NewPtrSys(size);
	if (ptr) {
		getDriversState (ptr, size);
		if (FindDriversFolder (&spec, &n)) {
			err = FSMakeFSSpec(spec.vRefNum, spec.parID, stateFile, &file);
			if (err == fnfErr)
				err = FSpCreate (&file, kStateCreator, kStateType, smSystemScript);
			if (err != noErr) goto err;
			err = OpenFile(&file, fsWrPerm, &ref);
			if (err != noErr) goto err;
			err = WriteFile(ref, &size, ptr);
			CloseFile (ref);
		}
	}
	else return;
err:
	DisposePtr (ptr);
}

void RestoreState  (TMSGlobalPtr g)
{
	short n, ref; FSSpec spec, file; OSErr err;
	long size; Ptr ptr = 0;
	if (FindDriversFolder (&spec, &n)) {
		err = FSMakeFSSpec(spec.vRefNum, spec.parID, stateFile, &file);
		err = OpenFile(&file, fsRdPerm, &ref);
		if (err != noErr) return;
		size = GetFileSize (&file);
		if (size) {
			ptr = NewPtrSys(size);
			if (ptr) {
				err = ReadFile (ref, &size, ptr);
				if (err == noErr)
					setDriversState (ptr, size);
				DisposePtr (ptr);
			}
		}
		CloseFile (ref);
	}
}

