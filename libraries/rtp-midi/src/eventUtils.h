/*

  Copyright (c) Grame 2005

  This library is free software; you can redistribute it and modify it under 
  the terms of the GNU Lesser General Public License as published by the 
  Free Software Foundation version 2 of the License, or any later version.

  This library is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public 
  License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

  Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
  research@grame.fr
 
*/

#ifndef EVENT_UTILS
#define EVENT_UTILS

#include <MidiShare.h>
#include "types.h"

#define UNSUPPORTED_COMMAND   0
#define SYSTEM_COMMAND        1
#define CHANNEL_COMMAND       2

unsigned short commandCategory ( unsigned short eventType );

MidiEvPtr newKeyOff ( unsigned int deltaTime, short channel, short pitch, short velocity );
MidiEvPtr newKeyOn ( unsigned int deltaTime, short channel, short pitch, short velocity );
MidiEvPtr newKeyPress ( unsigned int deltaTime, short channel, short pitch, short velocity );
MidiEvPtr newCtrlChange ( unsigned int deltaTime, short channel, short controller, short value );
MidiEvPtr newProgChange ( unsigned int deltaTime, short channel, short program );
MidiEvPtr newChanPress ( unsigned int deltaTime, short channel, short pressure );
MidiEvPtr newPitchWheel ( unsigned int deltaTime, short channel, short lsb, short msb );
MidiEvPtr newCtrl14b ( unsigned int deltaTime, unsigned short channel, unsigned short control, unsigned short value );
MidiEvPtr newNonRegParam ( unsigned int deltaTime, unsigned short channel, unsigned short parameter, unsigned short value );
MidiEvPtr newRegParam ( unsigned int deltaTime, unsigned short channel, unsigned short parameter, unsigned short value );

MidiEvPtr newSongPos ( unsigned int deltaTime, short lsb, short msb );
MidiEvPtr newSongSel ( unsigned int deltaTime, short song );
MidiEvPtr newClock ( unsigned int deltaTime );
MidiEvPtr newStart ( unsigned int deltaTime );
MidiEvPtr newContinue ( unsigned int deltaTime );
MidiEvPtr newStop ( unsigned int deltaTime );
MidiEvPtr newTune ( unsigned int deltaTime );
MidiEvPtr newActiveSens ( unsigned int deltaTime );
MidiEvPtr newReset ( unsigned int deltaTime );

unsigned short lengthOf ( const MidiEvPtr command );

unsigned short writeCommand ( MidiEvPtr command, uint8 * buffer );
unsigned short readCommand ( uint8 * buffer, MidiEvPtr * commandAddress );

bool equal ( MidiEvPtr command_1, MidiEvPtr command_2 );

#endif // EVENT_UTILS
