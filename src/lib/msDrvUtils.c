/*
  MidiShare Project
  Copyright (C) Grame 2000

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/


#ifdef MODULE
#include "MidiShareKernel.h"
#else
#include "MidiShare.h"
#include "FilterUtils.h"
#endif

#include "msDrvUtils.h"



/*===========================================================================
  Internal functions prototypes
  =========================================================================== */
static short  CountSlots ();
static void * memcopy (void *src, void *dst, long size);
static char * getDriverLocation (TDriverInfos *dInfos, char *ptr, long size);
static void	  RestoreSlot (SlotRefNum sref, TSlotInfos * sInfos, char *ptr, long size);


/*===========================================================================
  External functions
  =========================================================================== */
Boolean Get1DriverState (short refNum, short slotsCount, void *buff, long buffsize)
{
	TSlotInfos sInfos; SlotRefNum sref; short i;
	
	for (i=1; i<=slotsCount; i++) {
		sref = MidiGetIndSlot (refNum, i);
		if ((Slot(sref) >= 0) && MidiGetSlotInfos (sref, &sInfos)) {
			if (buffsize >= sizeof(sInfos)) {
				buff = memcopy (&sInfos, buff, sizeof(sInfos));
				buffsize -= sizeof(sInfos);
			}
			else return false;
		}
	}
	return true;
}

/*____________________________________________________________________________*/
Boolean GetDriversState (void * buff, long buffsize)
{
	TDriverInfos dInfos; long size;
	short i, n = MidiCountDrivers ();

	for (i=1; i<=n; i++) {
		short ref = MidiGetIndDriver (i);
		if ((ref > 0) && MidiGetDriverInfos (ref, &dInfos)) {
			if (buffsize >= sizeof(dInfos)) {
				buff = memcopy (&dInfos, buff, sizeof(dInfos));
				buffsize -= sizeof(dInfos);
			}
			else return false;
			if (Get1DriverState (ref, dInfos.slots, buff, buffsize)) {
				size = Get1DriverStateSize (dInfos.slots);
				buffsize -= size;
				buff = (Ptr)((long)buff + size);
			}
			else return false;
		}
	}
	return true;
}

/*____________________________________________________________________________*/
long Get1DriverStateSize (short slotsCount)
{
	return slotsCount * sizeof(TSlotInfos);
}

/*____________________________________________________________________________*/
long GetDriversStateSize ()
{
	short nslots = CountSlots (), n = MidiCountDrivers ();
	return (n * sizeof(TDriverInfos)) + (nslots * sizeof(TSlotInfos));
}

/*____________________________________________________________________________*/
void Set1DriverState (short refNum, short slotsCount, void * ptr, long size)
{
	TSlotInfos sInfos; SlotRefNum sref; short i;

	for (i=1; i<=slotsCount; i++) {
		sref = MidiGetIndSlot (refNum, i);
		if ((Slot(sref) >= 0) && MidiGetSlotInfos (sref, &sInfos)) {
			RestoreSlot (sref, &sInfos, (char *)ptr, size);
		}
	}
}

/*____________________________________________________________________________*/
void SetDriversState (void * ptr, long size)
{
	TDriverInfos dInfos; long offset;
	short i, n = MidiCountDrivers ();

	for (i=1; i<=n; i++) {
		short ref = MidiGetIndDriver (i);
		if ((ref > 0) && MidiGetDriverInfos (ref, &dInfos)) {
			char * buff = getDriverLocation (&dInfos, (char *)ptr, size);			
			if (buff) {
				buff += sizeof(TDriverInfos);
				offset = (long)buff - (long)ptr;
				Set1DriverState (ref, dInfos.slots, buff, size - offset);
			}
		}
	}
}

/*===========================================================================
  Internal functions
  =========================================================================== */
static Boolean equalNames (DriverName d1, DriverName d2)
{
#if macintosh
	short count = *d1 + 1;
	while (count--) {
		if (*d1++ != *d2++) return false;
	}
	return true;
#else
	while (*d1 && (*d1 == *d2) ) { d1++; d2++; }
	return (*d1 == *d2);
#endif
}

static TSlotInfos * getSlotLocation (TSlotInfos * sInfos, char *ptr, long size)
{
	while (size > 0) {
		TSlotInfos * sPtr = (TSlotInfos *)ptr;
		if (equalNames (sInfos->name, sPtr->name) &&
			(sInfos->direction == sPtr->direction))
			return (TSlotInfos *)ptr;
		ptr += sizeof(TSlotInfos);
		size -= sizeof(TSlotInfos);
	}
	return 0;
}

static void RestoreSlot (SlotRefNum sref, TSlotInfos * sInfos, char *ptr, long size)
{
	TSlotInfos * inf = getSlotLocation (sInfos, ptr, size);
	if (inf) {
		short port;
		for (port=0; port<256; port++) {
			Boolean set = IsAcceptedBit (inf->cnx, port);
			MidiConnectSlot (port, sref, set);
		}
	}
}

static char * getDriverLocation (TDriverInfos *dInfos, char *ptr, long size)
{
	TDriverInfos * dPtr; long chunkSize;
	while (size > 0) {
		dPtr = (TDriverInfos *)ptr;
		if (equalNames (dInfos->name, dPtr->name))
			return ptr;
		chunkSize = sizeof(TDriverInfos) + (sizeof(TSlotInfos) * dPtr->slots);
		ptr += chunkSize;
		size -= chunkSize;
	}
	return 0;
}
			
static short CountSlots ()
{
	TDriverInfos infos;
	short i, ref, count = 0, n = MidiCountDrivers ();
	
	for (i=1; i<=n; i++) {
		ref = MidiGetIndDriver (i);
		if ((ref > 0) && MidiGetDriverInfos (ref, &infos)) {
			count += infos.slots;
		}
	}
	return count;
}

static void * memcopy (void *src, void *dst, long size)
{
	char *s = (char *)src, *d = (char *)dst;
	while (size--) {
		*d++ = *s++;
	}
	return d;
}
