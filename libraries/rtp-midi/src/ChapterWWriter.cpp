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

#include "ChapterWWriter.h"

#include "ChannelWriter.h"
#include <MidiShare.h>

#include <iostream>

ChapterWWriter::ChapterWWriter ( ChannelWriter * channelWriter )
  : ChannelChapterWriter ( channelWriter )
{

  _name = 'W';
  _channelWriter->registerForCommandNotification ( this, typePitchWheel );

  _pitchWheel = false;

}

void ChapterWWriter::notifyCommand ( MidiEvPtr command )
{

  /* PitchWheel */
  if ( EvType ( command ) == typePitchWheel ) {
    _pitchWheel = true;
    _lastPitchWheelLSB = Data ( command ) [0];
    _lastPitchWheelMSB = Data ( command ) [1];
    _lastPitchWheelPayload = currentPayloadNumber ( );
  }

}

void ChapterWWriter::notifyResetCommand ( MidiEvPtr command, short reset )
{

}

void ChapterWWriter::calculateChapter ( )
{

  if ( _pitchWheel ) {
    _chapter[0] = _lastPitchWheelLSB;
    _chapter[1] = _lastPitchWheelMSB;
    // S bit
    if ( _lastPitchWheelPayload < currentPayloadNumber ( ) - 1 ) {
      setFlag ( & _chapter[0], 0, 0 );
      unsetParentSBit ( );
    }
    else {
      setFlag ( & _chapter[0], 0, 1 );
    }
    _chapterLength = 2;
  }
  else {
    _chapterLength = 0;
  }

}

uint8 * ChapterWWriter::chapter ( )
{

  return _chapter;

}

unsigned int ChapterWWriter::chapterLength ( )
{

  return _chapterLength;

}

void ChapterWWriter::printHistory ( )
{

  cout << "# CHAPTER W ( " << _channelWriter->channelNumber ( ) << " ) WRITER HISTORY" << endl;
  cout << "   lastPitchWheelLSB\t" << _lastPitchWheelLSB << endl;
  cout << "   lastPitchWheelMSB\t" << _lastPitchWheelMSB << endl;
  cout << "   pitchWheel\t" << _pitchWheel << endl;

}
