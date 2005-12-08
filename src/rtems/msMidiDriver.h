/*******************************************************************************
 * C H A M E L E O N    S. D. K.                                               *
 *******************************************************************************
 *  $Archive:: /Chameleon.sdk/system/midishare/rtems/msMidiDriver.h            $
 *     $Date: 2005/12/08 13:39:26 $
 * $Revision: 1.1.2.1 $
 *-----------------------------------------------------------------------------*
 * This file is part of the Chameleon Software Development Kit                 *
 *                                                                             *
 * Copyright (C) 2001 soundart                                                 *
 * www.soundart-hot.com                                                        *
 * codemaster@soundart-hot.com                                                 *
 ******************************************************************************/

/*

  This library is free software; you can redistribute it and modify it under 
  the terms of the GNU Library General Public License as published by the 
  Free Software Foundation version 2 of the License, or any later version.

  This library is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public 
  License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#ifndef __MIDISHARE_MIDI_DRIVER__
#define __MIDISHARE_MIDI_DRIVER__

#include "EventToMidiStream.h"
#include "MidiStreamToEvent.h"

#define MidiDriverName		"Midi Driver"
#define MidiSlotName		"Midi Slot"

typedef struct
{
	SlotRefNum	slot;
	StreamFifo	rcv;
	Ev2StreamRec	xmt;
	fifo		fifoXmt;
} MidiSlot, *MidiSlotPtr;

typedef struct
{
	short 		drv;
	LinearizeMthTbl	lTbl;
	ParseMethodTbl	rTbl;
	Status2TypeTbl	s2t;
	TFilter		filter;
	MidiSlot	slotData;
} MidiDrv, *MidiDrvPtr;

BOOL msOpenMidiDriver(void);
void msCloseMidiDriver(void);

#endif

