/*

  Copyright © Grame 1999-2000

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


#ifndef __msDriver__
#define __msDriver__

#include "msDefs.h"

/*_______________________________________________________________*/
#define MaxSlots	128
#define MaxPorts	256

#define Driver(appl)	(appl->driver)
#define Wakeup(appl)	Driver(appl)->op.wakeup
#define Sleep(appl)		Driver(appl)->op.sleep

typedef struct SInfos {
	SlotName 		name;
	SlotDirection 	direction;
} SInfos, * SInfosPtr;

/*_______________________________________________________________*/
typedef struct SlotInfosPublic {
	short    driverRefNum;
	short    slotRefNum;
	SlotName name;
	char     cnx[MaxPorts/8];
	char    direction;
	char    infos;
} SlotInfosPublic, * SlotInfosPublicPtr;

/*_______________________________________________________________*/
typedef struct SlotInfos SlotInfos, * SlotInfosPtr;
struct SlotInfos {
	SlotInfosPtr	   next;
	SlotInfosPublicPtr pub;
	RcvAlarmPtr        rcvAlarm;
    fifo               rcv;         /* slot received events fifo */
	long               unused;
};

/*_______________________________________________________________*/
typedef struct {
	short	refNum;
	short	version;
	short	slotsCount;
} TDriverPublic, * TDriverPublicPtr;

/*_______________________________________________________________*/
typedef struct {
	TDriverPublicPtr pub;
	TDriverOperation op;
	SlotInfosPtr     slots;
} TDriver, * TDriverPtr;

#endif
