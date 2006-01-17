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

#include "StreamWriter.h"

#include "CommandFilter.h"
#include "PayloadWriter.h"
#include "RecoveryJournalWriter.h"

#include "eventUtils.h"
#include "binaryUtils.h"
#include <iostream>

#define J 1

StreamWriter::StreamWriter ( )
{

  _commandFilter = new CommandFilter ( );
  _payloadWriter = new PayloadWriter ( this );
  _recoveryJournalWriter = new RecoveryJournalWriter ( this );
  _recoveryJournalWriter->changeCheckpoint ( 0 );

  _journallingMethod = RECJ;

}

void StreamWriter::newPayload ( const uint32 payloadNumber, uint8 * buffer, const unsigned short size )
{

  _currentPayloadNumber = payloadNumber;
  _payloadBuffer = buffer;
  _payloadBufferSize = size;

  unsigned short commandSize;
  if ( _journallingMethod != NONE ) {
    commandSize = size - recoveryJournalWriter ( )->calculateJournal ( );
  }
  else {
    commandSize = size;
  }

  _payloadWriter->newPayload ( buffer, commandSize );

}

short StreamWriter::putCommand ( const MidiEvPtr command )
{

  short accept = _commandFilter->acceptCommand ( command );
  if ( accept > 0 ) {
    short size = _payloadWriter->putCommand ( command );
    if ( size ) {
      _recoveryJournalWriter->notifyCommand ( command );
    }
    return size;
  }
  return accept;

}

int StreamWriter::finalizePayload ( )
{

  int length;
  length = _payloadWriter->finalizePayload ( );
  if ( _journallingMethod != NONE ) {
    int journalLength = _recoveryJournalWriter->writeJournal ( _payloadBuffer + length );
    length += journalLength;
    if ( journalLength != 0 ) {
      setFlag ( & _payloadBuffer[0], J );
    }
  }

  if ( length > _payloadBufferSize ) {
    cerr << "internal error : payload size exceeded !" << endl;
    exit ( EXIT_FAILURE );
  }

  return length;

}

void StreamWriter::changeCheckpoint ( unsigned int checkpoint )
{

  _recoveryJournalWriter->changeCheckpoint ( checkpoint );

}

RecoveryJournalWriter * StreamWriter::recoveryJournalWriter ( ) const
{

  return _recoveryJournalWriter;

}

void StreamWriter::setJournallingMethod ( journallingMethod j )
{

  _journallingMethod = j;

}

void StreamWriter::setChapterInclusion ( char chapterName, chapterInclusion c, short channel )
{

  _recoveryJournalWriter->setChapterInclusion ( chapterName, c, channel );
  if ( c == UNUSED ) {
    _commandFilter->setChapterUnused ( chapterName, channel );
  }
  else {
    _commandFilter->setChapterUsed ( chapterName, channel );
  }

}

void StreamWriter::setControllerLogTool ( unsigned short controller, unsigned short tool, short channel )
{

  _recoveryJournalWriter->setControllerLogTool ( controller, tool, channel );

}

void StreamWriter::printChapterHistory ( char chapter, short channel )
{

  _recoveryJournalWriter->printChapterHistory ( chapter, channel );

}
