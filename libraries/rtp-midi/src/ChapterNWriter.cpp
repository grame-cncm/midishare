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

#include "ChapterNWriter.h"

#include "ChannelWriter.h"
#include <MidiShare.h>

#include "printingUtils.h"

#include <iostream>

ChapterNWriter::ChapterNWriter ( ChannelWriter * channelWriter )
  : ChannelChapterWriter ( channelWriter )
{

  _name = 'N';
  _channelWriter->registerForCommandNotification ( this, typeKeyOff );
  _channelWriter->registerForCommandNotification ( this, typeKeyOn );

}

void ChapterNWriter::notifyCommand ( MidiEvPtr command )
{

  if ( EvType ( command ) == typeKeyOn || EvType ( command ) == typeKeyOff ) {

    for ( list<TNoteInfo>::iterator i = _history.begin ( ) ; i != _history.end ( ) ; i++ ) {
      if ( ( * i ).pitch == Pitch ( command ) ) {
	_history.erase ( i );
	break;
      }
    }

    TNoteInfo newNote;
    newNote.pitch = Pitch ( command );
    if ( EvType ( command ) == typeKeyOff ) {
      newNote.vel = 0;
    }
    else {
      newNote.vel = Vel ( command );
    }
    newNote.payload = currentPayloadNumber ( );
    _history.push_back ( newNote );

  }

}

void ChapterNWriter::notifyResetCommand ( MidiEvPtr command, short resetType )
{

  /* Reset N-Active */
  if ( resetType == RESET_STATE || resetType == RESET_N_ACTIVE ) {
    _history.clear ( );
  }

}

void ChapterNWriter::calculateChapter ( )
{

  _chapter = ( uint8 * ) realloc ( _chapter, 2 );
  if ( _chapter == NULL ) {
    cerr << "realloc failed !" << endl;
    exit ( EXIT_FAILURE );
  }
  memset ( _chapter, 0, 2 );
  _chapterLength = 2;
  setFlag ( & _chapter[0], 0, 1 );

  uint8 * position = _chapter + 2;
  unsigned short low = 15;
  unsigned short high = 0;
  unsigned short len = 0;
  for ( list<TNoteInfo>::iterator i = _history.begin ( ) ; i != _history.end ( ) ; i++ ) {
    if ( ( * i ).payload >= checkpoint ( ) ) {

      if ( ( * i ).vel > 0 ) {
	len++;
	_chapterLength += 2;
	_chapter = ( uint8 * ) realloc ( _chapter, _chapterLength );
	if ( _chapter == NULL ) {
	  cerr << "realloc failed !" << endl;
	  exit ( EXIT_FAILURE );
	};
	position = _chapter + _chapterLength - 2;
	position[0] = 0x80;
	// S
	if ( ( * i ).payload == currentPayloadNumber ( ) - 1 ) {
	  setFlag ( & position[0], 0, 0 );
	  setFlag ( & _chapter[0], 0, 0 );
	  unsetParentSBit ( );
	}
	// NOTENUM
	position[0] |= ( * i ).pitch;
	// Y
	position[1] = 0x80;
	// VELOCITY
	position[1] |= ( * i ).vel;
      }

      else {
	// B
	if ( ( * i ).payload == currentPayloadNumber ( ) - 1 ) {
	  setFlag ( & _chapter[0], 0, 0 );
	  unsetParentSBit ( );
	}
	if ( low > ( * i ).pitch / 8 ){
	  low = ( * i ).pitch / 8;
	}
	if ( high < ( * i ).pitch / 8 ){
	  high = ( * i ).pitch / 8;
	}
      }

    }
  }

  // LEN
  _chapter[0] |= len;
  // LOW
  _chapter[1] = low << 4;
  // HIGH
  _chapter[1] |= high;

  if ( ! ( low == 15 && high == 0 ) ) {
    _chapter = ( uint8 * ) realloc ( _chapter, _chapterLength + high - low + 1 );
    if ( _chapter == NULL ) {
      cerr << "realloc failed !" << endl;
      exit ( EXIT_FAILURE );
    }
    position = _chapter + _chapterLength;
    memset ( position, 0, high - low + 1 );
    _chapterLength += high - low + 1;

    for ( list<TNoteInfo>::iterator i = _history.begin ( ) ; i != _history.end ( ) ; i++ ) {
      if ( ( * i ).vel == 0 && ( * i ).payload >= checkpoint ( ) ) {
	position[( * i ).pitch / 8 - low] |= ( 0x80 >> ( * i ).pitch % 8 );
      }
    }
  }

  if ( _chapterLength == 2 ) {
    _chapterLength = 0;
  }

}

uint8 * ChapterNWriter::chapter ( )
{

  return _chapter;

}

unsigned int ChapterNWriter::chapterLength ( )
{

  return _chapterLength;

}

void ChapterNWriter::printHistory ( )
{

  cout << "# CHAPTER N ( " << _channelWriter->channelNumber ( ) << " ) WRITER HISTORY" << endl;
  for ( list<TNoteInfo>::iterator i = _history.begin ( ) ; i != _history.end ( ) ; i++ ) {
    cout << "   pitch\t" << hex << ( * i ).pitch << dec << "\tpayload\t" << ( * i ).payload << "\tvelocity\t" << hex << ( * i ).vel << dec << endl;
  }

}
