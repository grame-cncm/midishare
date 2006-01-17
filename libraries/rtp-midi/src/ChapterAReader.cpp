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

#include "ChapterAReader.h"

#include "ChannelReader.h"
#include <MidiShare.h>

#include "eventUtils.h"

#include <iostream>

ChapterAReader::ChapterAReader ( ChannelReader * channelReader )
  : ChannelChapterReader ( channelReader )
{

  _name = 'A';
  _channelReader->registerForCommandNotification ( this, typeKeyPress );

}

void ChapterAReader::notifyCommand ( MidiEvPtr command )
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
    newNotePress.payload = currentPayloadNumber ( );
    _history.push_back ( newNotePress );

  }

}

void ChapterAReader::notifyResetCommand ( MidiEvPtr command, short resetType )
{

  /* Reset N-Active */
  if ( resetType == RESET_STATE || resetType == RESET_N_ACTIVE ) {
    //    _history.clear ( );
  }

}

int ChapterAReader::readChapter ( uint8 * buffer )
{

  unsigned int len = ( buffer[0] & 0x7F );

  uint8 * position = buffer + 1;
  for ( unsigned int i = 0 ; i < len + 1 ; i++ ) {
    unsigned short pitch = position[0] & 0x7F;
    //    unsigned short x = position[1]
    unsigned short pressure = position[1] & 0x7F;
    list<TNotePressInfo>::iterator iter;
    for ( iter = _history.begin ( ) ; iter != _history.end ( ) ; iter++ ) {
      if ( ( * iter ).pitch == pitch ) {
	if ( ( * iter ).press != pressure ) {
	  addRecoveryCommand ( newKeyPress ( 0, _channelReader->channelNumber ( ), pitch, pressure ) );
	}
	break;
      }
    }
    if ( iter == _history.end ( ) ) {
      addRecoveryCommand ( newKeyPress ( 0, _channelReader->channelNumber ( ), pitch, pressure ) );
    }
    position += 2;
  }

  return position - buffer;

}

void ChapterAReader::printHistory ( )
{

  cout << "# CHAPTER A ( " << _channelReader->channelNumber ( ) << " ) READER HISTORY" << endl;
  for ( list<TNotePressInfo>::iterator i = _history.begin ( ) ; i != _history.end ( ) ; i++ ) {
    cout << "   pitch\t" << hex << ( * i ).pitch << "\tpressure\t" << ( * i ).press << dec << "\tpayload\t" << ( * i ).payload << endl;
  }

}
