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

#include "ChapterAWriter.h"

#include "ChannelWriter.h"
#include <MidiShare.h>

#include <iostream>

ChapterAWriter::ChapterAWriter ( ChannelWriter * channelWriter )
  : ChannelChapterWriter ( channelWriter )
{

  _name = 'A';
  _channelWriter->registerForCommandNotification ( this, typeKeyPress );

}

void ChapterAWriter::notifyCommand ( MidiEvPtr command )
{

  /* KeyPress */
  if ( EvType ( command ) == typeKeyPress ) {

    for ( list<TNotePressInfo>::iterator i = _history.begin ( ) ; i != _history.end ( ) ; i++ ) {
      if ( ( * i ).pitch == Pitch ( command ) ) {
	_history.erase ( i );
	break;
      }
    }

    TNotePressInfo newNotePress;
    newNotePress.pitch = Pitch ( command );
    newNotePress.press = Vel ( command );
    newNotePress.x = 0;
    newNotePress.payload = currentPayloadNumber ( );
    _history.push_back ( newNotePress );

  }

}

void ChapterAWriter::notifyResetCommand ( MidiEvPtr command, short resetType )
{

  /* Reset N-Active */
  if ( resetType == RESET_STATE || resetType == RESET_N_ACTIVE ) {
    _history.clear ( );
  }

  /* For X bit */
  if ( EvType ( command ) == typeCtrlChange &&
       ( ( short ) Data ( command ) [0] == 120 ||
	 ( short ) Data ( command ) [0] == 123 ||
	 ( short ) Data ( command ) [0] == 124 ||
	 ( short ) Data ( command ) [0] == 125 ||
	 ( short ) Data ( command ) [0] == 126 ||
	 ( short ) Data ( command ) [0] == 127 ) ) {
    for ( list<TNotePressInfo>::iterator i = _history.begin ( ) ; i != _history.end ( ) ; i++ ) {
      ( * i ).x = 1;
    }
  }

}

void ChapterAWriter::calculateChapter ( )
{

  _chapter = ( uint8 * ) realloc ( _chapter, 2 );
  if ( _chapter == NULL ) {
    cerr << "realloc failed !" << endl;
    exit ( EXIT_FAILURE );
  }
  memset ( _chapter, 0, 2 );
  uint8 * position = _chapter + 1;
  unsigned short len = 0;
  for ( list<TNotePressInfo>::iterator i = _history.begin ( ) ; i != _history.end ( ) ; i++ ) {
    if ( ( * i ).payload >= checkpoint ( ) ) {

    len++;
    _chapterLength = position - _chapter;
    _chapter = ( uint8 * ) realloc ( _chapter, _chapterLength + 2 );
    if ( _chapter == NULL ) {
      cerr << "realloc failed !" << endl;
      exit ( EXIT_FAILURE );
    }
    position = _chapter + _chapterLength;
    // S
    if ( ( * i ).payload == currentPayloadNumber ( ) - 1 ) {
      position[0] = 0x00;
      _chapter[0] = 0x00;
      unsetParentSBit ( );
    }
    else {
      position[0] = 0x80;
      _chapter[0] = 0x80;
    }
    // NOTENUM
    position[0] |= ( * i ).pitch;
    // X
    if ( ( * i ).x ) {
      position[1] = 0x80;
    }
    else {
      position[1] = 0x00;
    }
    // PRESSURE
    position[1] |= ( * i ).press;
    position += 2;

    }
  }

  if ( len != 0 ) {
    // LEN
    _chapter[0] |= len - 1;
    _chapterLength = 1 + len * 2;
  }
  else {
    _chapterLength = 0;
  }

}

uint8 * ChapterAWriter::chapter ( )
{

  return _chapter;

}

unsigned int ChapterAWriter::chapterLength ( )
{

  return _chapterLength;

}

void ChapterAWriter::printHistory ( )
{

  cout << "# CHAPTER A ( " << _channelWriter->channelNumber ( ) << " ) WRITER HISTORY" << endl;
  for ( list<TNotePressInfo>::iterator i = _history.begin ( ) ; i != _history.end ( ) ; i++ ) {
    cout << "   pitch\t" << hex << ( * i ).pitch << "\tpressure\t" << ( * i ).press << dec << "\tpayload\t" << ( * i ).payload << endl;
  }

}
