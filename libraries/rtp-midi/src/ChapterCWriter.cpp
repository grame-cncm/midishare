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

#include "ChapterCWriter.h"

#include "ChannelWriter.h"
#include <MidiShare.h>

#include <iostream>

#define VALUE_TOOL  0x01
#define TOGGLE_TOOL 0x02
#define COUNT_TOOL  0x04

#define ON  127
#define OFF 0
#define IsOff( e )  ( ( e ) < 64 )
#define IsOn( e )   ( ( e ) > 63 )

ChapterCWriter::ChapterCWriter ( ChannelWriter * channelWriter )
  : ChannelChapterWriter ( channelWriter )
{

  _name = 'C';
  _channelWriter->registerForCommandNotification ( this, typeCtrlChange );

  for ( unsigned short i = 0 ; i < 128 ; i++ ) {
    _tool[i] = defaultTool ( i );
  }

}

void ChapterCWriter::notifyCommand ( MidiEvPtr command )
{

  /* CtrlChange */
  if ( EvType ( command ) == typeCtrlChange ) {

    list<TCtrlInfo>::iterator i;
    for ( i = _history.begin ( ) ; i != _history.end ( ) ; i++ ) {
      if ( ( * i ).number == Data ( command ) [0] ) {
	TCtrlInfo ctrlInfo;
	ctrlInfo.number = ( * i ).number;
	if ( ( IsOn ( ( * i ).value ) && IsOff ( Data ( command ) [1] ) ) ||
	     ( IsOff ( ( * i ).value ) && IsOn ( Data ( command ) [1] ) ) ) {
	  ctrlInfo.toggle = ( ( * i ).toggle + 1 ) % 64;
	}
	else {
	  ctrlInfo.toggle = ( * i ).toggle;
	}
	ctrlInfo.value = Data ( command ) [1];
	ctrlInfo.count = ( ( * i ).count + 1 ) % 64;
	ctrlInfo.payload = currentPayloadNumber ( );
	_history.erase ( i );
	_history.push_back ( ctrlInfo );
	break;
      }
    }
    if ( i == _history.end ( ) ) {
	TCtrlInfo ctrlInfo;
	ctrlInfo.number = Data ( command ) [0];
	ctrlInfo.value = Data ( command ) [1];
	ctrlInfo.toggle = IsOn ( Data ( command ) [1] );
	ctrlInfo.count = 1;
	ctrlInfo.payload = currentPayloadNumber ( );
	_history.push_back ( ctrlInfo );
    }

  }

}

void ChapterCWriter::notifyResetCommand ( MidiEvPtr command, short resetType )
{

  /* Reset State */
  if ( resetType == RESET_STATE ) {
    _history.clear ( );
  }

  /* Reset C-Active */
  if ( resetType == RESET_C_ACTIVE ) {
    for ( list<TCtrlInfo>::iterator i = _history.begin ( ) ; i != _history.end ( ) ; i++ ) {
      if ( ( * i ).number < 120 ) {
	_history.erase ( i );
	i--;
      }
    }
  }

}

void ChapterCWriter::calculateChapter ( )
{

  _chapter = ( uint8 * ) realloc ( _chapter, 1 );
  if ( _chapter == NULL ) {
    cerr << "realloc failed !" << endl;
    exit ( EXIT_FAILURE );
  }
  memset ( _chapter, 0, 1 );
  setFlag ( & _chapter[0], 0, 1 );
  uint8 * position = _chapter + 1;
  unsigned short len = 0;
  for ( list<TCtrlInfo>::iterator i = _history.begin ( ) ; i != _history.end ( ) ; i++ ) {
    if ( ( * i ).payload >= checkpoint ( ) ) {

      if ( _tool[( * i ).number] & TOGGLE_TOOL ) {
	len++;
	_chapterLength = position - _chapter;
	_chapter = ( uint8 * ) realloc ( _chapter, _chapterLength + 2 );
	if ( _chapter == NULL ) {
	  cerr << "realloc failed !" << endl;
	  exit ( EXIT_FAILURE );
	}
	position = _chapter + _chapterLength;
	position[0] = 0x80;
	// S
	if ( ( * i ).payload == currentPayloadNumber ( ) - 1 ) {
	  setFlag ( & position[0], 0, 0 );
	  setFlag ( & _chapter[0], 0, 0 );
	  unsetParentSBit ( );
	}
	// NUMBER
	position[0] |= ( * i ).number;
	// A + T
	position[1] = 0xC0;
	// VALUE
	position[1] |= ( * i ).toggle;
	position += 2;
      }

      if ( _tool[( * i ).number] & VALUE_TOOL ) {
	len++;
	_chapterLength = position - _chapter;
	_chapter = ( uint8 * ) realloc ( _chapter, _chapterLength + 2 );
	if ( _chapter == NULL ) {
	  cerr << "realloc failed !" << endl;
	  exit ( EXIT_FAILURE );
	}
	position = _chapter + _chapterLength;
	// S
	setFlag ( & position[0], 0, 1 );
	if ( ( * i ).payload == currentPayloadNumber ( ) - 1 ) {
	  setFlag ( & position[0], 0, 0 );
	  setFlag ( & _chapter[0], 0, 0 );
	  unsetParentSBit ( );
	}
	// NUMBER
	position[0] |= ( * i ).number;
	// A
	position[1] = 0x00;
	// VALUE
	position[1] |= ( * i ).value;
	position += 2;
      }

      if ( _tool[( * i ).number] & COUNT_TOOL ) {
	len++;
	_chapterLength = position - _chapter;
	_chapter = ( uint8 * ) realloc ( _chapter, _chapterLength + 2 );
	if ( _chapter == NULL ) {
	  cerr << "realloc failed !" << endl;
	  exit ( EXIT_FAILURE );
	}
	position = _chapter + _chapterLength;
	// S
	setFlag ( & position[0], 0, 1 );
	if ( ( * i ).payload == currentPayloadNumber ( ) - 1 ) {
	  setFlag ( & position[0], 0, 0 );
	  setFlag ( & _chapter[0], 0, 0 );
	  unsetParentSBit ( );
	}
	// NUMBER
	position[0] |= ( * i ).number;
	// A
	position[1] = 0x80;
	// VALUE
	position[1] |= ( * i ).count;
	position += 2;
      }

    }
  }

  _chapter[0] |= len - 1;
  if ( len != 0 ) {
    _chapterLength = 1 + len * 2;
  }
  else {
    _chapterLength = 0;
  }

}

uint8 * ChapterCWriter::chapter ( )
{

  return _chapter;

}

unsigned int ChapterCWriter::chapterLength ( )
{

  return _chapterLength;

}

void ChapterCWriter::printHistory ( )
{

  cout << "# CHAPTER C ( " << _channelWriter->channelNumber ( ) << " ) WRITER HISTORY" << endl;
  for ( list<TCtrlInfo>::iterator i = _history.begin ( ) ; i != _history.end ( ) ; i++ ) {
    cout << "   number\t" << hex << ( * i ).number << dec << "\tpayload\t" << ( * i ).payload <<"\tvalue\t" << hex << ( * i ).value << dec << "\ttoggle\t" << hex << ( * i ).toggle << dec << "\tcount\t" << hex << ( * i ).count << dec << endl;
  }

}

void ChapterCWriter::setControllerLogTool ( unsigned short controller, unsigned short tool )
{

  _tool[controller] = tool;

}

short ChapterCWriter::defaultTool ( short controller )
{

  switch ( controller ) {

  case 64 :
  case 65 :
  case 66 :
  case 67 :
  case 68 :
  case 69 :
    return TOGGLE_TOOL;

  case 96 :
  case 97 :
  case 120 :
  case 121 :
  case 123 :
  case 124 :
  case 125 :
  case 127 :
    return COUNT_TOOL;

  default :
    return VALUE_TOOL;

  }

}
