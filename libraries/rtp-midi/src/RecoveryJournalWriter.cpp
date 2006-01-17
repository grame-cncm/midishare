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

#include "RecoveryJournalWriter.h"
#include "StreamWriter.h"
#include "ChannelWriter.h"
#include "SystemChapterWriter.h"

#include "eventUtils.h"
#include "binaryUtils.h"

#include <iostream>

RecoveryJournalWriter::RecoveryJournalWriter ( StreamWriter * streamWriter )
  : _streamWriter ( streamWriter)
{

  /* add the chapters in their order of appearance in the journal ! */

  // create channels
  for ( unsigned int i = 0 ; i < CHANNELS ; i++ ) {
    _channels[i] = new ChannelWriter ( this, i );
  }

}

unsigned int RecoveryJournalWriter::calculateJournal ( )
{

  unsigned int journalLength = 3;
  _sBit = 1;
  _systemSBit = 1;

  unsigned int systemChaptersLength = 0;
  for ( unsigned int i = 0 ; i < _systemChapters.size ( ) ; i++ ) {
    _systemChapters[i]->calculateChapter ( );
    systemChaptersLength += _systemChapters[i]->chapterLength ( );
  }
  if ( systemChaptersLength > 0 ) {
    journalLength += 2;
    journalLength += systemChaptersLength;
  }

  for ( unsigned int i = 0 ; i < CHANNELS ; i++ ) {
    journalLength += _channels[i]->calculateChannel ( );
  }

  if ( journalLength == 3 ) {
    journalLength = 0;
  }

  return journalLength;

}

int RecoveryJournalWriter::writeJournal ( uint8 * buffer )
{

  /* HEADER */

  buffer[0] = 0;
  // CHECKPOINT
  buffer[1] = _checkpoint / 256;
  buffer[2] = _checkpoint % 256;

  uint8 * position = buffer + 3;

  /* SYSTEM JOURNAL */

  unsigned int systemChaptersLength = 0;
  for ( unsigned int i = 0 ; i < _systemChapters.size ( ) ; i++ ) {
      systemChaptersLength += _systemChapters[i]->chapterLength ( );
  }

  if ( systemChaptersLength > 0 ) {
    if ( systemChaptersLength + 2 > 1023 ) {
      cerr << "system journal is too large : " << systemChaptersLength << " !" << endl;
    }
    position[0] = ( systemChaptersLength + 2 ) / 256;
    position[1] = ( systemChaptersLength + 2 ) % 256;
    position += 2;
    for ( unsigned int i = 0 ; i < SYSTEM_CHAPTERS ; i++ ) {
      for ( unsigned int j = 0 ; j < _systemChapters.size ( ) ; j++ ) {
	if ( _systemChapters[j]->name ( ) == systemChapterName[i] ) {
	  unsigned short chapterLength;
	  if ( ( chapterLength = _systemChapters[j]->chapterLength ( ) ) > 0 ) {
	    if ( _systemChapters[j]->writeChapter ( position ) ) {
	      position += chapterLength;
	      setFlag ( & buffer[3], i );
	    }
	  }
	}
      }
    }
    // System S Bit
    setFlag ( & buffer[3], 0, _systemSBit );
  }

  /* CHANNEL JOURNALS */

  unsigned short totchan = 0;
  for ( unsigned int i = 0 ; i < CHANNELS ; i++ ) {
    unsigned short channelJournalLength = 0;
    channelJournalLength = _channels[i]->writeChannel ( position );
    if ( channelJournalLength > 0 ) {
      position += channelJournalLength;
      totchan++;
    }
  }
  if ( totchan ) {
    setFlag ( & buffer[0], 2 );
    buffer[0] |= ( totchan - 1 ) % 16;
  }

  // S bit
  setFlag ( & buffer[0], 0, _sBit );

  if ( position - buffer > 3 ) {
    return position - buffer;
  }
  else {
    return 0;
  }

}

void RecoveryJournalWriter::unsetSBit ( ) {

  _sBit = 0;

}

void RecoveryJournalWriter::unsetSystemSBit ( ) {

  _systemSBit = 0;
  unsetSBit ( );

}

StreamWriter * RecoveryJournalWriter::streamWriter ( )
{

  return _streamWriter;

}

vector < Chapter * > * RecoveryJournalWriter::systemChapters ( )
{

  return ( vector < Chapter * > * ) & _systemChapters;

}

Channel * RecoveryJournalWriter::channel ( unsigned short n )
{

  return _channels[n];

}

void RecoveryJournalWriter::notifyResetCommand ( const MidiEvPtr command, short resetType )
{

  for ( unsigned int i = 0 ; i < systemChapters ( )->size ( ) ; i++ ) {
    ( ( ChapterWriter * ) systemChapters ( )->at ( i ) )->ChapterWriter::notifyResetCommand ( command, resetType );
  }

  for ( unsigned int i = 0 ; i < CHANNELS ; i++ ) {
    ( ( ChannelWriter * ) channel ( i ) )->ChannelWriter::notifyResetCommand ( command, resetType );
  }

}

void RecoveryJournalWriter::setChapterInclusion ( char chapterName, chapterInclusion c, short channel )
{

  for ( unsigned int i = 0 ; i < _systemChapters.size ( ) ; i++ ) {
    if ( _systemChapters[i]->name ( ) == chapterName ) {
      _systemChapters[i]->setChapterInclusion ( c );
    }
  }

  if ( channel >= 0 && channel < CHANNELS ) {
    _channels[channel]->setChapterInclusion ( chapterName, c );
  }
  else {
    for ( unsigned short i = 0 ; i < CHANNELS ; i++ ) {
      _channels[i]->setChapterInclusion ( chapterName, c );
    }
  }

}

void RecoveryJournalWriter::setControllerLogTool ( unsigned short controller, unsigned short tool, short channel )
{

  if ( channel >= 0 && channel < CHANNELS ) {
    _channels[channel]->setControllerLogTool ( controller, tool );
  }
  else {
    for ( unsigned short i = 0 ; i < CHANNELS ; i++ ) {
      _channels[i]->setControllerLogTool ( controller, tool );
    }
  }

}
