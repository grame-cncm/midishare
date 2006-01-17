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

#include "PayloadReader.h"

#include "deltaTime.h"
#include "eventUtils.h"
#include "binaryUtils.h"
#include <stdlib.h>
#include <iostream>

#define B 0
#define J 1
#define Z 2
#define P 3

void PayloadReader::newPayload ( uint8 * buffer, const unsigned short size )
{

  _buffer = buffer;
  _endOfMidiList = _buffer + 2 /*header*/ + ( _buffer[0] & 0x0F ) * 0x0100 + _buffer[1] /*LEN*/ - 1;
  _startPayload = true;

  // we always consider a 12 bits LEN field ( this might be improved )
  _position = _buffer + 2;

}

MidiEvPtr PayloadReader::getCommand ( )
{

  if ( _position >= _endOfMidiList ) {
    return NULL;
  }

  uint32 deltaTime;
  MidiEvPtr command;

  if ( _startPayload && ! getFlag ( _buffer[0], Z ) ) {
    _position += readCommand ( _position, & command );
    Date ( command ) = 0;
  }

  if ( _startPayload ) {
    _startPayload = false;
  }

  _position += decodeDeltaTime ( _position, & deltaTime );
  _position += readCommand ( _position, & command );

  Date ( command ) = deltaTime;
  return command;

}

uint8 * PayloadReader::recoveryJournalLocation( )
{

  if ( getFlag ( _buffer[0], J ) ) {
    return _endOfMidiList + 1;
  }
  else {
    return NULL;
  }

}
