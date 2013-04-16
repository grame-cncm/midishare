/*
  MidiShare Project
  Copyright (C) Grame 1999-2000

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/


#ifndef __msDriver__
#define __msDriver__

#include "msDefs.h"

/*_______________________________________________________________*/
#define MaxSlots	64
#define MaxPorts	256
#define PortMapSize	MaxPorts/8
#define SlotMapSize	MaxSlots/8

#define Driver(appl)	(appl->driver)
#define Wakeup(appl)	Driver(appl)->op.wakeup
#define Sleep(appl)		Driver(appl)->op.sleep

typedef char PortMap[PortMapSize], * PortMapPtr;	
typedef char SlotMap[SlotMapSize];	

typedef struct SInfos {
	SlotName 		name;
	SlotDirection 	direction;
} SInfos, * SInfosPtr;

/*_______________________________________________________________*/
typedef struct {
	TDriverOperation op;
	short	version;
	short	slotsCount;
	PortMapPtr	map[MaxSlots];
	SInfosPtr 	slotInfos[MaxSlots];
	char		port[MaxPorts][SlotMapSize];
} TDriver, * TDriverPtr;

#endif
