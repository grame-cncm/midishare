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
  
  modifications history: 11/11/99 First version SL

*/

/*
	MIDI Driver built using ALSA
*/

#ifdef MODVERSIONS
#include <linux/modversions.h>
#include "include/sndversions.h"
#endif

#include "alsaDriver.h"
#include "msEvents.h"
#include "msKernel.h"
#include "msMemory.h"
#include "../msLoader.h"
#include "msXmtRcv.h"

/*
 * The ALSA low-level midi routines appear to return 'whole' midi events and 
 * have already handled midi running state. I don't know if will remain in 
 * the future. If not, some more elaborate MIDI parser is needed.
 */ 
 
 
unsigned char sysEx[SYSEX_LEN];
unsigned long lastDate;  // date of last sent event;
 

/*__________________________________________________________________________________*/

static void snd_midi_command(snd_rawmidi_t * rmidi, void *cmd_private_data, unsigned char *command, int count)
{
	TAlsaDriver *msynth = (TAlsaDriver *) cmd_private_data;
	int channel;
	int port;
	int i;
	MidiEvPtr ev;
	
	if (msynth == NULL) {
		snd_printk("msynth == NULL\n");
		return;
	}
	port = msynth->seq_port;
	channel = command[0] & 0x0f;
	
	switch (command[0] & 0xf0) {
	
		case 0x80:	// note off

			if (count == 3 && (ev = MSNewEv(typeKeyOff, FreeList(Memory(gMem))))) {
				ev->port = port;
				ev->chan = channel;
				MSSetField(ev,0,command[1]);
				MSSetField(ev,1,command[2]);
				MSSendIm(0, ev,SorterList(gMem), CurrTime(gMem));
				return;
			}
			break;

		case 0x90:	// note on

			if (count == 3 && (ev = MSNewEv(typeKeyOn,FreeList(Memory(gMem))))) {
				ev->port = port;
				ev->chan = channel;
				MSSetField(ev,0,command[1]);
				MSSetField(ev,1,command[2]);
				MSSendIm(0, ev,SorterList(gMem), CurrTime(gMem));
				return;
			}
			break;
	
		case 0xa0:	// poly key pressure

			if (count == 3 && (ev = MSNewEv(typeKeyPress,FreeList(Memory(gMem))))) {
				ev->port = port;
				ev->chan = channel;
				MSSetField(ev,0,command[1]);
				MSSetField(ev,1,command[2]);
				MSSendIm(0, ev,SorterList(gMem), CurrTime(gMem));
				return;
			}
			break;

		case 0xb0:	// control change
	
			if (count == 3 && (ev = MSNewEv(typeCtrlChange,FreeList(Memory(gMem))))) {
				ev->port = port;
				ev->chan = channel;
				MSSetField(ev,0,command[1]);
				MSSetField(ev,1,command[2]);
				MSSendIm(0, ev,SorterList(gMem), CurrTime(gMem));
				return;
			}
			break;

		case 0xc0:	// program change
		
			if (count == 2 && (ev = MSNewEv(typeProgChange,FreeList(Memory(gMem))))) {
				ev->port = port;
				ev->chan = channel;
				MSSetField(ev,0,command[1]);
				MSSendIm(0, ev,SorterList(gMem), CurrTime(gMem));
				return;
			}
			break;

		case 0xd0:	// channel pressure

			if (count == 2 && (ev = MSNewEv(typeChanPress,FreeList(Memory(gMem))))) {
				ev->port = port;
				ev->chan = channel;
				MSSetField(ev,0,command[1]);
				MSSendIm(0, ev,SorterList(gMem), CurrTime(gMem));
				return;
			}
			break;

		case 0xe0:	// pitch bender

			if (count == 3  && (ev = MSNewEv(typePitchWheel,FreeList(Memory(gMem))))) {
				ev->port = port;
				ev->chan = channel;
				MSSetField(ev,0,command[1]);
				MSSetField(ev,0,command[2]);
				MSSendIm(0, ev,SorterList(gMem), CurrTime(gMem));
				
				return;
			}
			break;

		case 0xf0:
			switch (command[0]) {
				case 0xf0:	// sysex 
				
					if ((ev = MSNewEv(typeSysEx,FreeList(Memory(gMem))))) {
						ev->port = port;
						ev->chan = 0;
						for (i = 0; i< count; i++) {
							MSAddField(ev,command[i+1],FreeList(Memory(gMem)));
						}
						MSSendIm(0, ev,SorterList(gMem), CurrTime(gMem));
						return;
					}
					
					break;	
		
				case 0xf1:	// MTC quarter frame 
				
					if (count == 2 && (ev = MSNewEv(typeQuarterFrame,FreeList(Memory(gMem))))) {
						ev->port = port;
						ev->chan = channel;
						MSSetField(ev,0,command[1] & 0xf0);
						MSSetField(ev,1,command[1] & 0x0f);
						MSSendIm(0, ev,SorterList(gMem), CurrTime(gMem));
						return;
					}
					break;

				case 0xf2:	// song position 
				
					if (count == 3  && (ev = MSNewEv(typeSongPos,FreeList(Memory(gMem))))) {
						ev->port = port;
						ev->chan = channel;
						MSSetField(ev,0,command[1]);
						MSSetField(ev,1,command[2]);
						MSSendIm(0, ev,SorterList(gMem), CurrTime(gMem));
						return;
					}
					break;

				case 0xf3:	// song select 
				
					if (count == 2 && (ev = MSNewEv(typeSongSel,FreeList(Memory(gMem))))) {
						ev->port = port;
						ev->chan = channel;
						MSSetField(ev,0,command[1]);
						MSSendIm(0, ev,SorterList(gMem), CurrTime(gMem));
						return;
					}
					break;

				case 0xf4:	
					return;

				case 0xf5:	
					return;

				case 0xf6:	// tune request 
					snd_printd("Rx: tune request\n");
					return;

				case 0xf7:	// end of sysex 
					return;

				// system real-time messages

				case 0xf8:	// timing clock 
				
					if (count == 1 && (ev = MSNewEv(typeClock,FreeList(Memory(gMem))))) {
						ev->port = port;
						ev->chan = 0;
						MSSendIm(0, ev,SorterList(gMem), CurrTime(gMem));
						return;
					}
				case 0xf9: // undefined 
					return;
					
				case 0xfa:	// start 
				
					if (count == 1 && (ev = MSNewEv(typeStart,FreeList(Memory(gMem))))) {
						ev->port = port;
						ev->chan = 0;
						MSSendIm(0, ev,SorterList(gMem), CurrTime(gMem));
						return;
					}
				case 0xfb:	// continue

					if (count == 1 && (ev = MSNewEv(typeContinue,FreeList(Memory(gMem))))) {
						ev->port = port;
						ev->chan = 0;
						MSSendIm(0, ev,SorterList(gMem), CurrTime(gMem));
						return;
					}
				case 0xfc:	// stop

					if (count == 1 && (ev = MSNewEv(typeStop,FreeList(Memory(gMem))))) {
						ev->port = port;
						ev->chan = 0;
						MSSendIm(0, ev,SorterList(gMem), CurrTime(gMem));
						return;
					}
					
				case 0xfd:	// undefined 
					return;

				case 0xfe:	// active sensing

					if (count == 1 && (ev = MSNewEv(typeActiveSens,FreeList(Memory(gMem))))) {
						ev->port = port;
						ev->chan = channel;
						MSSendIm(0, ev,SorterList(gMem), CurrTime(gMem));
						return;
					}
					
				case 0xff:	// system reset

					if (count == 1 && (ev = MSNewEv(typeReset,FreeList(Memory(gMem))))) {
						ev->port = port;
						ev->chan = channel;
						MSSendIm(0, ev,SorterList(gMem), CurrTime(gMem));
						return;
					}
					return;
			}
			
	}
}


/*__________________________________________________________________________________*/
/* send data to specified midi device */

static void dump_midi(snd_rawmidi_t * rmidi, unsigned char *buf, int count)
{
	int done = snd_midi_transmit(rmidi, buf, count);

	if (done != count) {
		snd_printk("only wrote %d instead of %d bytes to midi device\n", done, count);
	}
}

/*__________________________________________________________________________________*/

int SendToDriver(TDriverPtr drv, MidiEvPtr ev)
{
	TAlsaDriver * msynth = (TAlsaDriver *)drv;
	int channel = ev->chan;
	unsigned char msg[10];	/* buffer for constructing midi messages */
	snd_rawmidi_t *rmidi;
	int i,n;
	MidiEvPtr keyOff;
	
	if (msynth == NULL) {
		snd_printd("msynth == NULL\n");
		return -EINVAL;
	}
	
	if (Date(ev) > (lastDate+10)) msynth->running_state = 0;
	lastDate = Date(ev);
	
	rmidi = msynth->output_rmidi;
	if (!rmidi)
		return -EINVAL;

	/* decode actual event data... */
	switch (EvType(ev)) {
	
		case typeNote:
		
			keyOff = MSCopyEv(ev, FreeList(Memory(gMem)));
			if (keyOff) {
			
				msg[0] = (channel & 0x0f) | 0x90;	/* note on */
				msg[1] = MSGetField(ev,0) & 0x7f;
				msg[2] = MSGetField(ev,1) & 0x7f;
				if (msynth->running_state == msg[0]) {
					dump_midi(rmidi, msg + 1, 2);
				} else {
					msynth->running_state = msg[0];
					dump_midi(rmidi, msg, 3);
				}
				
				EvType(keyOff) = typeKeyOn;
				Vel(keyOff)=0;
				Date(keyOff)+= Dur(ev);
				MSSend(0x80, keyOff,SorterList(gMem));
			}
			break;

		case typeKeyOff:
		
			msg[0] = (channel & 0x0f) | 0x80;	/* note off */
			msg[1] = MSGetField(ev,0) & 0x7f;
			msg[2] = MSGetField(ev,1) & 0x7f;
			if (msynth->running_state == msg[0]) {
				dump_midi(rmidi, msg + 1, 2);
			} else {
				msynth->running_state = msg[0];
				dump_midi(rmidi, msg, 3);
			}
			break;

		case typeKeyOn:
		
			msg[0] = (channel & 0x0f) | 0x90;	/* note on */
			msg[1] = MSGetField(ev,0) & 0x7f;
			msg[2] = MSGetField(ev,1) & 0x7f;
			if (msynth->running_state == msg[0]) {
				dump_midi(rmidi, msg + 1, 2);
			} else {
				msynth->running_state = msg[0];
				dump_midi(rmidi, msg, 3);
			}
			break;

		case typeKeyPress:
		
			msg[0] = (channel & 0x0f) | 0xa0;	/* polyphonic key pressure */
			msg[1] = MSGetField(ev,0) & 0x7f;
			msg[2] = MSGetField(ev,1) & 0x7f;
			if (msynth->running_state == msg[0]) {
				dump_midi(rmidi, msg + 1, 2);
			} else {
				msynth->running_state = msg[0];
				dump_midi(rmidi, msg, 3);
			}
			break;

		case typeCtrlChange:
		
			msg[0] = (channel & 0x0f) | 0xb0;	/* control change */
			msg[1] = MSGetField(ev,0) & 0x7f;
			msg[2] = MSGetField(ev,1) & 0x7f;
			if (msynth->running_state == msg[0]) {
				dump_midi(rmidi, msg + 1, 2);
			} else {
				msynth->running_state = msg[0];
				dump_midi(rmidi, msg, 3);
			}
			break;

		case typeProgChange:
		
			msg[0] = (channel & 0x0f) | 0xc0;	/* program change */
			msg[1] = MSGetField(ev,0) & 0x7f;
			if (msynth->running_state == msg[0]) {
				dump_midi(rmidi, msg + 1, 1);
			} else {
				msynth->running_state = msg[0];
				dump_midi(rmidi, msg, 2);
			}
			break;

		case typeChanPress:
		
			msg[0] = (channel & 0x0f) | 0xd0;	/* channel pressure */
			msg[1] = MSGetField(ev,0) & 0x7f;
			if (msynth->running_state == msg[0]) {
				dump_midi(rmidi, msg + 1, 1);
			} else {
				msynth->running_state = msg[0];
				dump_midi(rmidi, msg, 2);
			}
			break;

		case typePitchWheel:
		
			msg[0] = (channel & 0x0f) | 0xe0;	/* pitch bender */
			msg[1] = MSGetField(ev,0) & 0x7f;	/* lsb */
			msg[2] = MSGetField(ev,1) & 0x7f;		/* msb */
			if (msynth->running_state == msg[0]) {
				dump_midi(rmidi, msg + 1, 2);
			} else {
				msynth->running_state = msg[0];
				dump_midi(rmidi, msg, 3);
			}

		case typeSysEx: /* To ckeck :writing several buffer */
			
			n = MSCountFields(ev);
			if (n < SYSEX_LEN) {
				sysEx[0] = 0xf0;
				for (i = 0; i < n; i++) {sysEx[i+1] = MSGetField(ev,i);}
				sysEx[i+1] = 0xf7;
				dump_midi(rmidi, sysEx, n+2); /* n bytes + F0 and F7 */
			}else {
				snd_printd("SysEx too long\n");
			}
			msynth->running_state = 0;
			break;


		case typeQuarterFrame:  /* To ckeck */
		
			msg[0] = 0xf1;		/* MTC quarter frame */
			msg[1] = ((MSGetField(ev,0)<<4) + MSGetField(ev,1)) & 0x7f;
			dump_midi(rmidi, msg, 2);
			msynth->running_state = 0;
			break;

		case typeClock:
		
			msg[0] = 0xf8;
			dump_midi(rmidi, msg, 1);
			msynth->running_state = 0;
			break;

		case typeStart:
		
			msg[0] = 0xfa;
			dump_midi(rmidi, msg, 1);
			msynth->running_state = 0;
			break;

		case typeContinue:
		
			msg[0] = 0xfb;
			dump_midi(rmidi, msg, 1);
			msynth->running_state = 0;
			break;

		case typeStop:
		
			msg[0] = 0xfc;
			dump_midi(rmidi, msg, 1);
			msynth->running_state = 0;
			break;

		case typeActiveSens:
		
			msg[0] = 0xfe;	/* active sensing */
			dump_midi(rmidi, msg, 1);
			msynth->running_state = 0;
			break;

		default:
			snd_printd("Event  = Decoding for type %d is not implemented\n", EvType(ev));
	}
	return 0;
}

/*__________________________________________________________________________________*/

static int OpenDriverInt(TAlsaDriver * msynth, int card, int device)
{
	int err;
	
	memset(msynth, 0, sizeof(TAlsaDriver));
	msynth->seq_port = MIDI_PORT;
	msynth->card = card;
	msynth->device = device;
	
	/* open midi port for input */
	if ((err = snd_midi_open(msynth->card, msynth->device, 
	SND_RAWMIDI_LFLG_INPUT, &msynth->input_rmidi)) < 0) {
		snd_printk("midi input open failed!!!\n");
		//MOD_DEC_USE_COUNT;
		return err;
	}
	
	msynth->input_rmidi->input.u.p.command = snd_midi_command;
	msynth->input_rmidi->input.u.p.cmd_private_data = msynth;
	err = snd_midi_start_input(msynth->input_rmidi); 
	
	/* open midi port for output */
	if ((err = snd_midi_open(msynth->card, msynth->device, 
		SND_RAWMIDI_LFLG_OUTPUT, &msynth->output_rmidi)) < 0) {
		snd_printk("midi output open failed!!!\n");
		//MOD_DEC_USE_COUNT;
		return err;
	}
	//MOD_INC_USE_COUNT;

	return 0;
}

/*__________________________________________________________________________________*/

static int  CloseDriverInt(TAlsaDriver * msynth)
{
 	int err;
    unsigned char buf = 0xfe;/*active sensing midi event*/

	if (!msynth->input_rmidi) return -EINVAL;
	msynth->input_rmidi = NULL;
	
	if ((err = snd_midi_close(msynth->card, msynth->device, 
		SND_RAWMIDI_LFLG_INPUT))) {
		snd_printk("midi input close failed!!!\n");
		//MOD_DEC_USE_COUNT;
		return err;
	}
	//MOD_DEC_USE_COUNT;
	
	/*sending single active sensing message to shut the device up*/
	
	snd_midi_transmit (msynth->output_rmidi, &buf, 1);
	snd_midi_flush_output(msynth->output_rmidi);
	msynth->output_rmidi = NULL;
	
	if ((err = snd_midi_close(msynth->card, msynth->device, SND_RAWMIDI_LFLG_OUTPUT))) {
		snd_printd("midi output close failed!!!\n");
		//MOD_DEC_USE_COUNT;
		return err;
	}
	//MOD_DEC_USE_COUNT;
	return 0;

}

/*__________________________________________________________________________________*/

TDriverPtr	OpenDriverManager ()	{
	TAlsaDriver * drv = (TAlsaDriver *)kmalloc (sizeof(TAlsaDriver), GFP_KERNEL);
	if (drv) OpenDriverInt (drv,0,0); 
	return drv;
}

/*__________________________________________________________________________________*/

void CloseDriverManager (TDriverPtr driver)	{ 
	if (driver) {
		CloseDriverInt((TAlsaDriver *)driver);
		kfree (driver);
	}
}
