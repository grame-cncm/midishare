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

#include "StreamReader.h"

#include "PayloadReader.h"
#include "RecoveryJournalReader.h"

#include <iostream>

StreamReader::StreamReader ( )
{

  _payloadReader = new PayloadReader ( this );
  _recoveryJournalReader = new RecoveryJournalReader ( this );

}

void StreamReader::newPayload ( const uint32 payloadNumber, uint8 * buffer, const unsigned short size )
{

  _currentPayloadNumber = payloadNumber;
  _payloadReader->newPayload ( buffer, size );
  uint8 * journal = _payloadReader->recoveryJournalLocation ( );
  if ( journal ) {
    _recoveryJournalReader->readJournal ( journal );
  }

}

MidiEvPtr StreamReader::getCommand ( )
{

  MidiEvPtr command;
  if ( ( command = _payloadReader->getCommand ( ) ) ) {
    _recoveryJournalReader->notifyCommand ( command );
    return command;
  }
  else {
    return NULL;
  }

}

MidiEvPtr StreamReader::getRecoveryCommand ( )
{

  return _recoveryJournalReader->getRecoveryCommand ( );

}

void StreamReader::changeCheckpoint ( unsigned int checkpoint )
{
  _recoveryJournalReader->changeCheckpoint ( checkpoint );
}

RecoveryJournalReader * StreamReader::recoveryJournalReader ( ) const
{

  return _recoveryJournalReader;

}

void StreamReader::printChapterHistory ( char chapter, short channel )
{

  _recoveryJournalReader->printChapterHistory ( chapter, channel );

}
