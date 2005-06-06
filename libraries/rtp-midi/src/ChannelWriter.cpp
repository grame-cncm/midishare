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

#include "ChannelWriter.h"

#include "ChapterPWriter.h"
#include "ChapterCWriter.h"
#include "ChapterWWriter.h"
#include "ChapterNWriter.h"
#include "ChapterTWriter.h"
#include "ChapterAWriter.h"
#include "eventUtils.h"
#include "binaryUtils.h"

#include <iostream>

ChannelWriter::ChannelWriter ( RecoveryJournalWriter * recoveryJournalWriter, unsigned short channelNumber )
  : Channel ( channelNumber ), _recoveryJournalWriter ( recoveryJournalWriter )
{

  /* add the chapters in their order of appearance in the journal ! */
  _channelChapters.push_back ( new ChapterPWriter ( this ) );
  _channelChapters.push_back ( new ChapterCWriter ( this ) );
  _channelChapters.push_back ( new ChapterWWriter ( this ) );
  _channelChapters.push_back ( new ChapterNWriter ( this ) );
  _channelChapters.push_back ( new ChapterTWriter ( this ) );
  _channelChapters.push_back ( new ChapterAWriter ( this ) );

}

unsigned int ChannelWriter::calculateChannel ( )
{

  _sBit = 1;

  unsigned int channelLength = 0;
  for ( unsigned int i = 0 ; i < _channelChapters.size ( ) ; i++ ) {
    _channelChapters[i]->calculateChapter ( );
    channelLength += _channelChapters[i]->chapterLength ( );
  }
  if ( channelLength > 0 ) {
    channelLength += 3;
  }

  return channelLength;

}

int ChannelWriter::writeChannel ( uint8 * buffer )
{

  uint8 * position = buffer;

  unsigned int channelChaptersLength = 0;
  for ( unsigned int i = 0 ; i < _channelChapters.size ( ) ; i++ ) {
    channelChaptersLength += _channelChapters[i]->chapterLength ( );
  }

  if ( channelChaptersLength > 0 ) {
    if ( channelChaptersLength + 2 > 1023 ) {
      cerr << "channel journal is too large : " << channelChaptersLength << " !" << endl;
    }
    position[0] = ( _channelNumber << 3 ) ;
    position[1] = ( channelChaptersLength + 3 ) / 256;
    position[1] = ( channelChaptersLength + 3 ) % 256;
    position[2] = 0;
    position += 3;

    for ( unsigned int i = 0 ; i < CHANNEL_CHAPTERS ; i++ ) {
      for ( unsigned int j = 0 ; j < _channelChapters.size ( ) ; j++ ) {
	if ( _channelChapters[j]->name ( ) == channelChapterName[i] ) {
	  unsigned short chapterLength;
	  if ( ( chapterLength = _channelChapters[j]->chapterLength ( ) ) > 0 ) {
	    if ( _channelChapters[j]->writeChapter ( position ) ) {
	      position += chapterLength;
	      setFlag ( & buffer[2], i );
	    }
	  }
	}
      }
    }
    // Channel S Bit
    setFlag ( & buffer[0], 0, _sBit );
    return channelChaptersLength + 3;

  }
  else {
    return 0;
  }

}

void ChannelWriter::unsetSBit ( )
{

  _sBit = 0;
  _recoveryJournalWriter->unsetSBit ( );

}

RecoveryJournalWriter * ChannelWriter::recoveryJournalWriter ( )
{

  return _recoveryJournalWriter;

}

vector < Chapter * > * ChannelWriter::channelChapters ( )
{

  return ( vector < Chapter * > * ) & _channelChapters;

}

bool ChannelWriter::sBit ( )
{

  return _sBit;

}

void ChannelWriter::notifyResetCommand ( const MidiEvPtr command, short resetType )
{

  for ( unsigned int i = 0 ; i < channelChapters ( )->size ( ) ; i++ ) {
    ( ( ChapterWriter * ) channelChapters ( )->at ( i ) )->ChapterWriter::notifyResetCommand ( command, resetType );
  }

}

void ChannelWriter::setChapterInclusion ( char chapterName, chapterInclusion c )
{

  for ( unsigned int i = 0 ; i < _channelChapters.size ( ) ; i++ ) {
    if ( _channelChapters[i]->name ( ) == chapterName ) {
      ( ( ChapterWriter * ) _channelChapters[i] )->setChapterInclusion ( c );
    }
  }

}

void ChannelWriter::setControllerLogTool ( unsigned short controller, unsigned short tool )
{

  for ( unsigned int i = 0 ; i < _channelChapters.size ( ) ; i++ ) {
    if ( _channelChapters[i]->name ( ) == 'C' ) {
      ( ( ChapterCWriter * ) _channelChapters[i] )->setControllerLogTool ( controller, tool );
    }
  }

}
