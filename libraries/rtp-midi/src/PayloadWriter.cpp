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

#include "PayloadWriter.h"
#include "RecoveryJournalWriter.h"
#include "StreamWriter.h"

#include "deltaTime.h"
#include "eventUtils.h"
#include "binaryUtils.h"
#include <iostream>

#define B 0
#define Z 2
#define P 3

void PayloadWriter::newPayload ( uint8 * buffer, const unsigned short size )
{

  _buffer = buffer;
  _endOfBuffer = buffer + size - 1;
  _emptyPayload = true;

  /* HEADER */

  _buffer[0] = 0x00;
  // we always consider a 12 bits LEN field ( this might be improved )
  setFlag ( & _buffer[0], B );

  _position = _buffer + 2;

}

#include "printingUtils.h"

short PayloadWriter::putCommand ( const MidiEvPtr command )
{

  // can we insert the command ?

  unsigned short deltaTimeSize = encodedDeltaTimeSize ( Date ( command ) );
  if ( _emptyPayload && Date ( command ) == 0 ) {
    deltaTimeSize = 0;
  }

  /* typeCtrl14b
   * creates the corresponding typeCtrlChange
   */

  if ( EvType ( command ) == typeCtrl14b ) {
    if ( 2 * ( deltaTimeSize + 3 ) > spaceLeft ( ) ) {
      return 0;
    }

    MidiEvPtr ctrl1, ctrl2;
    ctrl1 = MidiNewEv ( typeCtrlChange );
    ctrl2 = MidiNewEv ( typeCtrlChange );
    Date ( ctrl1 ) = Date ( command );
    Date ( ctrl2 ) = Date ( command );
    Chan ( ctrl1 ) = Chan ( command );
    Chan ( ctrl2 ) = Chan ( command );
    Data ( ctrl1 ) [0] = MidiGetField ( command, 0 );
    Data ( ctrl2 ) [0] = MidiGetField ( command, 0 ) + 32;
    Data ( ctrl1 ) [1] = ( MidiGetField ( command, 1 ) & 0x3F80 ) >> 7;
    Data ( ctrl2 ) [1] = ( MidiGetField ( command, 1 ) & 0x007F );

    _streamWriter->putCommand ( ctrl1 );
    _streamWriter->putCommand ( ctrl2 );
    
    MidiFreeEv ( ctrl1 );
    MidiFreeEv ( ctrl2 );
    return 6;

  }

  /* typeRegParam, typeNonRegParam
   * creates the corresponding typeCtrlChange
   */

  if ( EvType ( command ) == typeNonRegParam || EvType ( command ) == typeRegParam ) {
    if ( 4 * ( deltaTimeSize + 3 ) > spaceLeft ( ) ) {
      return 0;
    }

    MidiEvPtr ctrl1, ctrl2, ctrl3, ctrl4;
    ctrl1 = MidiNewEv ( typeCtrlChange );
    ctrl2 = MidiNewEv ( typeCtrlChange );
    ctrl3 = MidiNewEv ( typeCtrlChange );
    ctrl4 = MidiNewEv ( typeCtrlChange );
    Date ( ctrl1 ) = Date ( command );
    Date ( ctrl2 ) = Date ( command );
    Date ( ctrl3 ) = Date ( command );
    Date ( ctrl4 ) = Date ( command );
    Chan ( ctrl1 ) = Chan ( command );
    Chan ( ctrl2 ) = Chan ( command );
    Chan ( ctrl3 ) = Chan ( command );
    Chan ( ctrl4 ) = Chan ( command );
    
    if ( EvType ( command ) == typeNonRegParam ) {
      Data ( ctrl1 ) [0] = 0x62;
      Data ( ctrl2 ) [0] = 0x63;
    }
    else {
      Data ( ctrl1 ) [0] = 0x64;
      Data ( ctrl2 ) [0] = 0x65;
    }
    Data ( ctrl1 ) [1] = ( MidiGetField ( command, 0 ) & 0x3F80 ) >> 7;
    Data ( ctrl2 ) [1] = ( MidiGetField ( command, 0 ) & 0x007F );
    
    Data ( ctrl3 ) [0] = 0x6;
    Data ( ctrl4 ) [0] = 0x26;
    Data ( ctrl3 ) [1] = ( MidiGetField ( command, 1 ) & 0x3F80 ) >> 7;
    Data ( ctrl4 ) [1] = ( MidiGetField ( command, 1 ) & 0x007F );
    
    _streamWriter->putCommand ( ctrl1 );
    _streamWriter->putCommand ( ctrl2 );
    _streamWriter->putCommand ( ctrl3 );
    _streamWriter->putCommand ( ctrl4 );
    
    MidiFreeEv ( ctrl1 );
    MidiFreeEv ( ctrl2 );
    MidiFreeEv ( ctrl3 );
    MidiFreeEv ( ctrl4 );
    return 12;

  }

  // commandLength
  unsigned short commandLength;
  commandLength = lengthOf ( command );
  if ( ! commandLength ) {
    cerr << "unknown command type" << endl;
    return 0;
  }

  // in case of a SysEx segment, a new event will be created locally : to be freed
  bool isLocalEvent = false;
  MidiEvPtr const * commandAddress = & command;

  /*
   * typeSysEx
   * it might be segmented
   */

  /* sysex cancellation */
  if ( _segmentedSysEx && ( _segmentedSysEx != command ) ) {
    MidiEvPtr cancellation;
    cancellation = MidiNewEv ( typeStream );
    Date ( cancellation ) = Date ( command );
    MidiAddField ( cancellation, 0xF7 );
    MidiAddField ( cancellation, 0xF4 );
    _segmentedSysEx = NULL;
    putCommand ( cancellation );
  }

  if ( EvType ( command ) == typeSysEx ) {

    /* first segment */
    if ( ! _segmentedSysEx && ( commandLength + deltaTimeSize > spaceLeft ( ) ) ) {
      if ( spaceLeft ( ) - deltaTimeSize < 3 ) {
	return 0;
      }

      MidiEvPtr segment = MidiNewEv ( typeStream );
      isLocalEvent = true;
      Date ( segment ) = Date ( command );
      MidiAddField ( segment, 0xF0 );
      readSysEx ( command );
      unsigned int i;
      for ( i = 0 ; ( int ) i < spaceLeft ( ) - deltaTimeSize - 2 ; i++ ) {
	MidiAddField ( segment, getNextSysExField ( ) );
      }
      MidiAddField ( segment, 0xF0 );
      commandLength = i + 2;
      commandAddress = & segment;

    /* middle segment */
    } else if ( _segmentedSysEx && ( commandLength - _sysExPosition + deltaTimeSize > spaceLeft ( ) ) ) {
	if ( spaceLeft ( ) - deltaTimeSize < 3 ) {
	  return 0;
	}

	MidiEvPtr segment = MidiNewEv ( typeStream );
	isLocalEvent = true;
	Date ( segment ) = Date ( command );
	MidiAddField ( segment, 0xF7 );
	unsigned int i = 0;
	for ( unsigned int i = 0 ; ( int ) i < spaceLeft ( ) - deltaTimeSize - 2 ; i++ ) {
	  MidiAddField ( segment, getNextSysExField ( ) );
	}
	MidiAddField ( segment, 0xF0 );
	commandLength = i + 2;
	commandAddress = & segment;

    /* last segment */
    } else if ( _segmentedSysEx && ( commandLength - _sysExPosition + deltaTimeSize <= spaceLeft ( ) ) ) {
      if ( spaceLeft ( ) - deltaTimeSize < 3 ) {
	return 0;
      }

      MidiEvPtr segment = MidiNewEv ( typeStream );
      isLocalEvent = true;
      Date ( segment ) = Date ( command );
      MidiAddField ( segment, 0xF7 );
      while ( _sysExPosition < commandLength - 2 ) {
	MidiAddField ( segment, getNextSysExField ( ) );
      }
      MidiAddField ( segment, 0xF7 );
      commandLength = MidiCountFields ( segment );
      commandAddress = & segment;
      _segmentedSysEx = NULL;
    }
  }

  if ( commandLength + deltaTimeSize > spaceLeft ( ) ) {
    // payload full
    return 0;
  }

  /* DELTA TIME */

  if ( _emptyPayload ) {
    _emptyPayload = false;
    if ( deltaTimeSize ) {
      setFlag ( & _buffer[0], Z );
      _position += encodeDeltaTime ( Date ( * commandAddress ), _position );
    }
  }
  else {
    _position += encodeDeltaTime ( Date ( * commandAddress ), _position );
  }

  /* COMMAND */

  writeCommand ( * commandAddress , _position );
  _position += commandLength;

  // free the locally created event in case of SysEx segmentation
  if ( isLocalEvent ) {
    MidiFreeEv ( * commandAddress );
  }

  if ( _segmentedSysEx ) {
    return 0;
  }
  else {
    return lengthOf ( command );
  }

}

int PayloadWriter::finalizePayload ( )
{

  setLEN ( _position - _buffer - 2 );
  return _position - _buffer;

}

void PayloadWriter::setLEN ( unsigned short length )
{

  _buffer[0] |= ( length / 0x0100 ) & 0x0F;
  _buffer[1] = ( length & 0x00FF );

}

unsigned short PayloadWriter::spaceLeft ( )
{

  return _endOfBuffer - _position + 1;

}

void PayloadWriter::readSysEx ( MidiEvPtr sysEx )
{

  _segmentedSysEx = sysEx;
  _cacheCell = Link ( LinkSE ( _segmentedSysEx ) );
  _sysExPosition = 0;

}

short PayloadWriter::getNextSysExField ( )
{

  short field = -1;

  if ( _cacheCell != LinkSE ( _segmentedSysEx ) ) {
    field = _cacheCell->data[_sysExPosition%cellSize];
    if ( _sysExPosition%cellSize == cellSize - 1 ) {
      _cacheCell = Link ( _cacheCell );
    }
    _sysExPosition++;
  }

  else if ( _sysExPosition%cellSize < LinkSE ( _segmentedSysEx )->data[cellSize-1] ) {
    field = _cacheCell->data[_sysExPosition%cellSize];
    _sysExPosition++;
  }

  return field;

}
