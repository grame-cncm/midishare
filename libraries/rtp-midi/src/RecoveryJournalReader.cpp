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

#include "RecoveryJournalReader.h"
#include "StreamReader.h"
#include "ChannelReader.h"
#include "SystemChapterReader.h"

#include "binaryUtils.h"

#include <iostream>

RecoveryJournalReader::RecoveryJournalReader ( StreamReader * streamReader)
  : _streamReader ( streamReader )
{

  /* add here the system chapters in their order of appearance in the journal */

  for ( unsigned int i = 0 ; i < CHANNELS ; i++ ) {
    _channels[i] = new ChannelReader ( this, i );
  }

}

int RecoveryJournalReader::readJournal ( uint8 * buffer )
{

  uint8 * position = buffer + 3;

  /* SYSTEM JOURNAL */

  if ( getFlag ( buffer[0], 1 ) ) {
    uint8 * systemJournal = position;
    for ( unsigned int i = 0 ; i < SYSTEM_CHAPTERS ; i++ ) {
      if ( getFlag ( systemJournal[0], i ) ) {
	for ( unsigned int j = 0 ; j < _systemChapters.size ( ) ; j++ ) {
	  if ( _systemChapters[j]->name ( ) == systemChapterName[i] ) {
	    position += _systemChapters[j]->readChapter ( position );
	  }
	}
      }
    }
  }

  /* CHANNEL JOURNALS */

  if ( getFlag ( buffer[0], 2 ) ) {
    unsigned short totchan = ( buffer[0] & 0x0f ) + 1;
    for ( unsigned short i = 0 ; i < totchan ; i++ ) {
      unsigned short chan = ( position[0] & 0x78 ) >> 3;
      position += _channels[chan]->readChannel ( position );
    }
  }

  return position - buffer;

}

MidiEvPtr RecoveryJournalReader::getRecoveryCommand ( )
{

  MidiEvPtr command;

  for ( unsigned int i = 0 ; i < _systemChapters.size ( ) ; i++ ) {
    if ( ( command = _systemChapters[i]->getRecoveryCommand ( ) ) ) {
      return command;
    }
  }
  
  for ( unsigned int i = 0 ; i < CHANNELS ; i++ ) {
    if ( ( command = _channels[i]->getRecoveryCommand ( ) ) ) {
      return command;
    }
  }
  
  return NULL;

}

StreamReader * RecoveryJournalReader::streamReader ( )
{

  return _streamReader;

}

vector < Chapter * > * RecoveryJournalReader::systemChapters ( )
{

  return ( vector < Chapter * > * ) & _systemChapters;

}

Channel * RecoveryJournalReader::channel ( unsigned short n )
{

  return _channels[n];

}
