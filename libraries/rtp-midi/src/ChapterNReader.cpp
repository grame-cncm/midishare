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

#include "ChapterNReader.h"

#include "ChannelReader.h"
#include <MidiShare.h>

#include "printingUtils.h"
#include "eventUtils.h"

#include <iostream>

ChapterNReader::ChapterNReader ( ChannelReader * channelReader )
  : ChannelChapterReader ( channelReader )
{

  _name = 'N';
  _channelReader->registerForCommandNotification ( this, typeKeyOff );
  _channelReader->registerForCommandNotification ( this, typeKeyOn );

}

void ChapterNReader::notifyCommand ( MidiEvPtr command )
{

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

void ChapterNReader::notifyResetCommand ( MidiEvPtr command, short resetType )
{

  /* Reset N-Active */
  if ( resetType == RESET_STATE || resetType == RESET_N_ACTIVE ) {
    //    _history.clear ( );
  }

}

int ChapterNReader::readChapter ( uint8 * buffer )
{

  unsigned short len = buffer[0] & 0x7F;
  unsigned short low = ( buffer[1] & 0xF0 ) >> 4;
  unsigned short high = buffer[1] & 0x0F;

  uint8 * position = buffer + 2;
  for ( unsigned int i = 0 ; i < len ; i++ ) {
    unsigned short pitch = position[0] & 0x7F;
    unsigned short velocity = position[1] & 0x7F;
    list<TNoteInfo>::iterator iter;
    for ( iter = _history.begin ( ) ; iter != _history.end ( ) ; iter++ ) {
      if ( ( * iter ).pitch == pitch ) {
	if ( ( * iter ).vel != velocity ) {
	  addRecoveryCommand ( newKeyOn ( 0, _channelReader->channelNumber ( ), pitch, velocity ) );
	}
	break;
      }
    }
    if ( iter == _history.end ( ) ) {
      addRecoveryCommand ( newKeyOn ( 0, _channelReader->channelNumber ( ), pitch, velocity ) );
    }
    position += 2;
  }

  if ( ! ( low == 15 && high == 0 ) && ! ( low == 15 && high == 1 ) ) {

    for ( short i = 0 ; i <= high - low ; i++ ) {
      for ( unsigned short j = 0 ; j < 8 ; j++ ) {
	if ( position[i] & ( 0x80 >> j ) ) {
	  unsigned short pitch = ( i + low ) * 8 + j;
	  list<TNoteInfo>::iterator iter;
	  for ( iter = _history.begin ( ) ; iter != _history.end ( ) ; iter++ ) {
	    if ( ( * iter ).pitch == pitch ) {
	      if ( ( * iter ).vel != 0 ) {
		addRecoveryCommand ( newKeyOff ( 0, _channelReader->channelNumber ( ), pitch, 0 ) );
	      }
	      break;
	    }
	  }
	  if ( iter == _history.end ( ) ) {
	    addRecoveryCommand ( newKeyOff ( 0, _channelReader->channelNumber ( ), pitch, 0 ) );
	  }
	}
      }
    }

    position += high - low + 1;

  }

  return position - buffer;

}

void ChapterNReader::printHistory ( )
{

  cout << "# CHAPTER N ( " << _channelReader->channelNumber ( ) << " ) READER HISTORY" << endl;
  for ( list<TNoteInfo>::iterator i = _history.begin ( ) ; i != _history.end ( ) ; i++ ) {
    cout << "   pitch\t" << hex << ( * i ).pitch << dec << "\tpayload\t" << ( * i ).payload << "\tvelocity\t" << hex << ( * i ).vel << dec << endl;
  }

}
