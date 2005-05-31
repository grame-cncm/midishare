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

#include "ChannelReader.h"
#include "ChapterPReader.h"
#include "ChapterCReader.h"
#include "ChapterWReader.h"
#include "ChapterNReader.h"
#include "ChapterTReader.h"
#include "ChapterAReader.h"

#include "binaryUtils.h"

#include <iostream>

ChannelReader::ChannelReader ( RecoveryJournalReader * recoveryJournalReader, unsigned short channelNumber )
  : Channel ( channelNumber ), _recoveryJournalReader ( recoveryJournalReader )
{

  /* add the chapters in their order of appearance in the journal ! */
  _channelChapters.push_back ( new ChapterPReader ( this ) );
  _channelChapters.push_back ( new ChapterCReader ( this ) );
  _channelChapters.push_back ( new ChapterWReader ( this ) );
  _channelChapters.push_back ( new ChapterNReader ( this ) );
  _channelChapters.push_back ( new ChapterTReader ( this ) );
  _channelChapters.push_back ( new ChapterAReader ( this ) );

}

int ChannelReader::readChannel ( uint8 * buffer )
{

  uint8 * position = buffer + 3;

  for ( unsigned short i = 0 ; i < 8 ; i++ ) {
    if ( getFlag ( buffer[2], i ) ) {
      for ( unsigned int j = 0 ; j < _channelChapters.size ( ) ; j++ ) {
	if ( _channelChapters[j]->name ( ) == channelChapterName[i] ) {
	  unsigned short chapterLength;
	  chapterLength = _channelChapters[j]->readChapter ( position );
	  position += chapterLength;
	}
      }
    }
  }

  return position - buffer;

}

MidiEvPtr ChannelReader::getRecoveryCommand ( )
{

  MidiEvPtr command;

  for ( unsigned int i = 0 ; i < _channelChapters.size ( ) ; i++ ) {
    if ( ( command = _channelChapters[i]->getRecoveryCommand ( ) ) ) {
      return command;
    }
  }
  
  return NULL;

}

RecoveryJournalReader * ChannelReader::recoveryJournalReader ( )
{

  return _recoveryJournalReader;

}

vector < Chapter * > * ChannelReader::channelChapters ( )
{

  return ( vector < Chapter * > * ) & _channelChapters;

}
