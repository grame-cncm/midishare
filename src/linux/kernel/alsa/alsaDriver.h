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

*/

#ifndef __alsaDriver__
#define __alsaDriver__

#include "include/driver.h"
#include "include/seq_kernel.h"
#include "include/rawmidi.h"
#include "include/seq_device.h"
#include "include/seq_midi_event.h"


#include "msDefs.h"
#include "msDriver.h"
#include "msFrcv.h"
#include "msFxmt.h"
#include "lffifo.h"

#define SYSEX_LEN 512  	/* length  of the SysEx buffer for sending */
#define MIDI_PORT  0    /* Midi port coded in received events */

/* data for this midi synth driver */
typedef struct {
	int card;
	int device;
	snd_rawmidi_t *input_rmidi;
	snd_rawmidi_t *output_rmidi;
	int seq_client;
	int seq_port;
	RFifo p2e;
	XFifo e2p;
	fifo infifo;
	unsigned long jiffies;
	unsigned char running_state;	/* for keeping the last status byte */	
} TAlsaDriver;

#endif
