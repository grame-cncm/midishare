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

#include "ChapterCReader.h"

#include "ChannelReader.h"
#include <MidiShare.h>

#include "eventUtils.h"

#include <iostream>

#define ON  127
#define OFF 0
#define IsOff( e )  ( ( e ) < 64 )
#define IsOn( e )   ( ( e ) > 63 )

ChapterCReader::ChapterCReader ( ChannelReader * channelReader )
  : ChannelChapterReader ( channelReader )
{

  _name = 'C';
  _channelReader->registerForCommandNotification ( this, typeCtrlChange );

}

void ChapterCReader::notifyCommand ( MidiEvPtr command )
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

void ChapterCReader::notifyResetCommand ( MidiEvPtr command, short resetType )
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

int ChapterCReader::readChapter ( uint8 * buffer )
{

  unsigned short len = buffer[0] & 0x7F;

  uint8 * position = buffer + 1;
  for ( unsigned int i = 0 ; i <= len ; i++ ) {

    unsigned short number = position[0] & 0x7F;
    unsigned short a = position[1] & 0x80;
    unsigned short t = position[1] & 0x40;
    unsigned short value;
    if ( ! a ) {
      value = position[1] & 0x7F;
    }
    else {
      value = position[1] & 0x3F;
    }

    list<TCtrlInfo>::iterator iter;
    for ( iter = _history.begin ( ) ; iter != _history.end ( ) ; iter++ ) {
      if ( ( * iter ).number == number ) {

	// toggle tool
	if ( a && t && ( * iter ).toggle != value ) {
	  if ( ( * iter ).toggle > value ) {
	    value += 64;
	  }
	  while ( ( * iter ).toggle < value ) {
	    if ( IsOn ( ( * iter ).value ) ) {
	      addRecoveryCommand ( newCtrlChange ( 0, _channelReader->channelNumber ( ), number, OFF ) );
	    }
	    if ( IsOff ( ( * iter ).value ) ) {
	      addRecoveryCommand ( newCtrlChange ( 0, _channelReader->channelNumber ( ), number, ON ) );
	    }
	  }
	}

	// value tool
	if ( ! a && ( * iter ).value != value ) {
	  addRecoveryCommand ( newCtrlChange ( 0, _channelReader->channelNumber ( ), number, value ) );
	}
	
	// count tool
	if ( a && ! t && ( * iter ).count != value ) {
	  if ( ( * iter ).count > value ) {
	    value += 64;
	  }
	  while ( ( * iter ).count < value ) {
	    addRecoveryCommand ( newCtrlChange ( 0, _channelReader->channelNumber ( ), number, ( * iter ).value ) );
	  }
	}

	break;
      }
    }

    if ( iter == _history.end ( ) ) {

      // toggle tool
      if ( a && t ) {
	unsigned short ctrl = OFF;
	for ( unsigned short i = 0 ; i < value ; i++ ) {
	  if ( ctrl == OFF ) {
	    addRecoveryCommand ( newCtrlChange ( 0, _channelReader->channelNumber ( ), number, ON ) );
	    ctrl = ON;
	  }
	  else if ( ctrl == ON ) {
	    addRecoveryCommand ( newCtrlChange ( 0, _channelReader->channelNumber ( ), number, OFF ) );
	    ctrl = OFF;
	  }
	}
      }

      // value tool
      if ( ! a ) {
	addRecoveryCommand ( newCtrlChange ( 0, _channelReader->channelNumber ( ), number, value ) );
      }

      // count tool
      if ( a && ! t ) {
	for ( unsigned short i = 0 ; i < value ; i++ ) {
	  addRecoveryCommand ( newCtrlChange ( 0, _channelReader->channelNumber ( ), number, defaultValue ( number ) ) );
	}
      }

    }
    position += 2;
  }
  
  return position - buffer;

}

void ChapterCReader::printHistory ( )
{

  cout << "# CHAPTER C ( " << _channelReader->channelNumber ( ) << " ) READER HISTORY" << endl;
  for ( list<TCtrlInfo>::iterator i = _history.begin ( ) ; i != _history.end ( ) ; i++ ) {
    cout << "   number\t" << hex << ( * i ).number << dec << "\tpayload\t" << ( * i ).payload << "\tvalue\t" << hex << ( * i ).value << dec << "\ttoggle\t" << hex << ( * i ).toggle << dec << "\tcount\t" << hex << ( * i ).count << dec << endl;
  }

}

short ChapterCReader::defaultValue ( short controller )
{

  switch ( controller ) {

  case 7 : // Volume
    return 100;

  case 10 : // Pan
    return 64;

  case 11 : // Expression
    return 127;

  default :
    return 0;

  }

}
