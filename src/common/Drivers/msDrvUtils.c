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


#include "MidiShare.h"
#include "msDrvUtils.h"

/*===========================================================================
  Internal functions prototypes
  =========================================================================== */
static short  CountSlots ();
static void * memcopy (void *src, void *dst, long size);
static char * getDriverLocation (TDriverInfos *dInfos, char *ptr, long size);
static void	  RestoreSlot (SlotRefNum sref, TSlotInfos * sInfos, char *ptr);
static TSlotInfos * getSlotLocation (TSlotInfos * sInfos, char *ptr);


/*===========================================================================
  External functions
  =========================================================================== */
void getDriversState (void * buff, long buffsize)
{
	TDriverInfos dInfos; TSlotInfos sInfos; SlotRefNum sref;
	short i, j, n = MidiCountDrivers ();

	for (i=1; i<=n; i++) {
		short ref = MidiGetIndDriver (i);
		if ((ref > 0) && MidiGetDriverInfos (ref, &dInfos)) {
			if (buffsize >= sizeof(dInfos)) {
				buff = memcopy (&dInfos, buff, sizeof(dInfos));
				buffsize -= sizeof(dInfos);
			}
			else return;
			for (j=1; j<=dInfos.slots; j++) {
				sref = MidiGetIndSlot (ref, j);
				if ((sref > 0) && MidiGetSlotInfos (sref, &sInfos)) {
					if (buffsize >= sizeof(sInfos)) {
						buff = memcopy (&sInfos, buff, sizeof(sInfos));
						buffsize -= sizeof(sInfos);
					}
					else return;
				}
			}
		}
	}
}

/*____________________________________________________________________________*/
long getDriversStateSize ()
{
	short nslots = CountSlots (), n = MidiCountDrivers ();
	return (n * sizeof(TDriverInfos)) + (nslots * sizeof(TSlotInfos));
}

/*____________________________________________________________________________*/
void setDriversState (void * ptr, long size)
{
	TDriverInfos dInfos; TSlotInfos sInfos; SlotRefNum sref;
	short i, j, n = MidiCountDrivers ();

	for (i=1; i<=n; i++) {
		short ref = MidiGetIndDriver (i);
		if ((ref > 0) && MidiGetDriverInfos (ref, &dInfos)) {
			char * buff = getDriverLocation (&dInfos, ptr, size);			
			if (buff) {
				for (j=1; j<=dInfos.slots; j++) {
					sref = MidiGetIndSlot (ref, j);
					if ((sref > 0) && MidiGetSlotInfos (sref, &sInfos)) {
						RestoreSlot (sref, &sInfos, buff);
					}
				}
			}
		}
	}
}

/*===========================================================================
  Internal functions
  =========================================================================== */
static Boolean equalNames (DriverName d1, DriverName d2)
{
#ifdef PascalNames
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

static void RestoreSlot (SlotRefNum sref, TSlotInfos * sInfos, char *ptr)
{
	TSlotInfos * inf = getSlotLocation (sInfos, ptr);
	if (inf) {
		short port;
		for (port=0; port<256; port++) {
			Boolean set = IsAcceptedBit (inf->cnx, port);
			MidiConnectSlot (port, sref, set);
		}
	}
}

static TSlotInfos * getSlotLocation (TSlotInfos * sInfos, char *ptr)
{
	TDriverInfos * dPtr = (TDriverInfos *)ptr;
	short n = dPtr->slots;
	ptr += sizeof(TDriverInfos);
	while (n--) {
		TSlotInfos * sPtr = (TSlotInfos *)ptr;
		if (equalNames (sInfos->name, sPtr->name) &&
			(sInfos->direction == sPtr->direction))
			return (TSlotInfos *)ptr;
		ptr += sizeof(TSlotInfos);
	}
	return 0;
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
