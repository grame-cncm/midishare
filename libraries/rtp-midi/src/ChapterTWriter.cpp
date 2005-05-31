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

#include "ChapterTWriter.h"

#include "ChannelWriter.h"
#include <MidiShare.h>

#include <iostream>

ChapterTWriter::ChapterTWriter ( ChannelWriter * channelWriter )
  : ChannelChapterWriter ( channelWriter )
{

  _name = 'T';
  _channelWriter->registerForCommandNotification ( this, typeChanPress );

  _chanPress = false;

}

void ChapterTWriter::notifyCommand ( MidiEvPtr command )
{

  /* ChanPress */
  if ( EvType ( command ) == typeChanPress ) {
    _chanPress = true;
    _lastPressure = Data ( command ) [0];
    _lastChanPressPayload = currentPayloadNumber ( );
  }

}

void ChapterTWriter::notifyResetCommand ( MidiEvPtr command, short resetType )
{

  /* CommandType */
  if ( resetType == RESET_N_ACTIVE || resetType == RESET_STATE ) {
    _chanPress = false;
  }

}

void ChapterTWriter::calculateChapter ( )
{

  if ( _chanPress ) {
    _chapter[0] = _lastPressure;
    // S bit
    if ( _lastChanPressPayload == currentPayloadNumber ( ) - 1 ) {
      setFlag ( & _chapter[0], 0, 0 );
      unsetParentSBit ( );
    }
    else {
      setFlag ( & _chapter[0], 0, 1 );
    }
    _chapterLength = 1;
  }
  else {
    _chapterLength = 0;
  }

}

uint8 * ChapterTWriter::chapter ( )
{

  return _chapter;

}

unsigned int ChapterTWriter::chapterLength ( )
{

  return _chapterLength;

}

void ChapterTWriter::printHistory ( )
{

  cout << "# CHAPTER T ( " << _channelWriter->channelNumber ( ) << " ) WRITER HISTORY" << endl;
  cout << "   lastPressure\t" << _lastPressure << endl;
  cout << "   chanPress\t" << _chanPress << endl;
  cout << "   lastChanPressPayload" << _lastChanPressPayload << endl;

}
