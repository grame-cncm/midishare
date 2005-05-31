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

#include "ChapterTReader.h"

#include "ChannelReader.h"
#include <MidiShare.h>

#include "eventUtils.h"

#include <iostream>

ChapterTReader::ChapterTReader ( ChannelReader * channelReader )
  : ChannelChapterReader ( channelReader )
{

  _name = 'T';
  _channelReader->registerForCommandNotification ( this, typeChanPress );

}

void ChapterTReader::notifyCommand ( MidiEvPtr command )
{

  /* ChanPress */
  if ( EvType ( command ) == typeChanPress ) {
    _lastPressure = Data ( command ) [0];
  }

}

void ChapterTReader::notifyResetCommand ( MidiEvPtr command, short resetType )
{

  /* Reset N-Active */
  if ( resetType == RESET_N_ACTIVE ) {
    _lastPressure = -1;
  }

}

int ChapterTReader::readChapter ( uint8 * buffer )
{

  short pressure = buffer[0] & 0x7f;
  if ( _lastPressure != pressure ) {
    addRecoveryCommand ( newChanPress ( 0, _channelReader->channelNumber ( ), pressure ) );
  }

  return 1;

}

void ChapterTReader::printHistory ( )
{

  cout << "# CHAPTER T ( " << _channelReader->channelNumber ( ) << " ) READER HISTORY" << endl;
  cout << "   lastPressure" << _lastPressure << endl;

}
