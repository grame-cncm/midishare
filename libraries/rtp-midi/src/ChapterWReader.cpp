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

#include "ChapterWReader.h"

#include "ChannelReader.h"
#include <MidiShare.h>

#include "eventUtils.h"

#include <iostream>

ChapterWReader::ChapterWReader ( ChannelReader * channelReader )
  : ChannelChapterReader ( channelReader )
{

  _name = 'W';
  _channelReader->registerForCommandNotification ( this, typePitchWheel );

  _lastPitchWheelLSB = -1;
  _lastPitchWheelMSB = -1;

}

void ChapterWReader::notifyCommand ( MidiEvPtr command )
{

  /* PitchWheel */
  if ( EvType ( command ) == typePitchWheel ) {
    _lastPitchWheelLSB = Data ( command ) [0];
    _lastPitchWheelMSB = Data ( command ) [1];
  }

}

void ChapterWReader::notifyResetCommand ( MidiEvPtr command, short resetType )
{

}

int ChapterWReader::readChapter ( uint8 * buffer )
{

  short first = buffer[0] & 0x7f;
  short second = buffer[1] & 0x7f;
  if ( _lastPitchWheelLSB != first ||
       _lastPitchWheelMSB != second ) {
    addRecoveryCommand ( newPitchWheel ( 0, _channelReader->channelNumber ( ), first, second ) );
  }

  return 2;

}

void ChapterWReader::printHistory ( )
{

  cout << "# CHAPTER W ( " << _channelReader->channelNumber ( ) << " ) READER HISTORY" << endl;
  cout << "   lastPitchWheelLSB\t" << _lastPitchWheelLSB << endl;
  cout << "   lastPitchWheelMSB\t" << _lastPitchWheelMSB << endl;

}
