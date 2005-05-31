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

#include "RecoveryJournal.h"

#include "Chapter.h"
#include "Channel.h"

#include <iostream>

char RecoveryJournal::systemChapterName[] = { 'D', 'V', 'Q', 'F', 'X' };

uint32 RecoveryJournal::checkpoint ( )
{

  return _checkpoint;

}

void RecoveryJournal::changeCheckpoint ( unsigned int checkpoint )
{

  _checkpoint = checkpoint;

}

void RecoveryJournal::notifyCommand ( const MidiEvPtr command )
{

  // for Reset State command
  if ( EvType ( command ) == typeReset ||
       ( EvType ( command ) == typeStream && MidiGetField ( command, 0 ) == 0xF0 && MidiGetField ( command, 1 ) == 0x7E && MidiGetField ( command, 2 ) == 0xCC && MidiGetField ( command, 5 ) == 0xF7 &&
	 ( ( MidiGetField ( command, 3 ) == 0x09 && MidiGetField ( command, 4 ) == 0x01 ) ||
	   ( MidiGetField ( command, 3 ) == 0x09 && MidiGetField ( command, 4 ) == 0x03 ) ||
	   ( MidiGetField ( command, 3 ) == 0x09 && MidiGetField ( command, 4 ) == 0x00 ) ||
	   ( MidiGetField ( command, 3 ) == 0x0A && MidiGetField ( command, 4 ) == 0x01 ) ||
	   ( MidiGetField ( command, 3 ) == 0x0A && MidiGetField ( command, 4 ) == 0x02 ) ) ) ) {

    notifyResetCommand ( command, RESET_STATE );

  }

  // for N-Active Reset Command
  if ( EvType ( command ) == typeCtrlChange &&
       ( ( short ) Data ( command ) [0] == 120 ||
	 ( short ) Data ( command ) [0] == 123 ||
	 ( short ) Data ( command ) [0] == 124 ||
	 ( short ) Data ( command ) [0] == 125 ||
	 ( short ) Data ( command ) [0] == 126 ||
	 ( short ) Data ( command ) [0] == 127 ) ) {

    notifyResetCommand ( command, RESET_N_ACTIVE );

  }

  // for C-Active Reset Command
  if ( EvType ( command ) == typeCtrlChange &&
       ( ( short ) Data ( command ) [0] == 121 ) ) {

    notifyResetCommand ( command, RESET_C_ACTIVE );

  }

  switch EvType ( command ) {

  // notify channel commands

  case typeKeyOff :
  case typeKeyOn :
  case typeKeyPress :
  case typeCtrlChange :
  case typeProgChange :
  case typeChanPress :
  case typePitchWheel :
  case typeCtrl14b :
  case typeRegParam :
  case typeNonRegParam :
    if ( Chan ( command ) < CHANNELS ) {
      channel ( Chan ( command ) )->notifyCommand ( command );
    }
    break;

  // notify system commands

  case typeSongPos :
    for ( unsigned int i = 0 ; i < _regSongPos.size ( ) ; i++ ) {
      _regSongPos[i]->notifyCommand ( command );
    }
    break;

  case typeSongSel :
    for ( unsigned int i = 0 ; i < _regSongSel.size ( ) ; i++ ) {
      _regSongSel[i]->notifyCommand ( command );
    }
    break;

  case typeClock :
    for ( unsigned int i = 0 ; i < _regClock.size ( ) ; i++ ) {
      _regClock[i]->notifyCommand ( command );
    }
    break;

  case typeStart :
    for ( unsigned int i = 0 ; i < _regStart.size ( ) ; i++ ) {
      _regStart[i]->notifyCommand ( command );
    }
    break;

  case typeContinue :
    for ( unsigned int i = 0 ; i < _regContinue.size ( ) ; i++ ) {
      _regContinue[i]->notifyCommand ( command );
    }
    break;

  case typeStop :
    for ( unsigned int i = 0 ; i < _regStop.size ( ) ; i++ ) {
      _regStop[i]->notifyCommand ( command );
    }
    break;

  case typeTune :
    for ( unsigned int i = 0 ; i < _regTune.size ( ) ; i++ ) {
      _regTune[i]->notifyCommand ( command );
    }
    break;

  case typeStream :
    for ( unsigned int i = 0 ; i < _regStream.size ( ) ; i++ ) {
      _regStream[i]->notifyCommand ( command );
    }
    break;

  }

}

void RecoveryJournal::notifyResetCommand ( const MidiEvPtr command, short resetType )
{

  if ( resetType == RESET_STATE ) {
    for ( unsigned int i = 0 ; i < systemChapters ( )->size ( ) ; i++ ) {
      systemChapters ( )->at ( i )->notifyResetCommand ( command, resetType );
    }
  }

  for ( unsigned int i = 0 ; i < CHANNELS ; i++ ) {
    channel ( i )->notifyResetCommand ( command, resetType );
  }

}

void RecoveryJournal::registerForCommandNotification ( Chapter * chapter, unsigned short commandType )
{

  switch ( commandType ) {

  case typeSongPos :
    _regSongPos.push_back ( chapter );
    break;

  case typeSongSel :
    _regSongSel.push_back ( chapter );
    break;

  case typeClock :
    _regClock.push_back ( chapter );
    break;

  case typeStart :
    _regStart.push_back ( chapter );
    break;

  case typeContinue :
    _regContinue.push_back ( chapter );
    break;

  case typeStop :
    _regStop.push_back ( chapter );
    break;

  case typeTune :
    _regTune.push_back ( chapter );
    break;

  case typeStream :
    _regStream.push_back ( chapter );
    break;

  }

}

void RecoveryJournal::printChapterHistory ( char chapter, short chan )
{

  for ( unsigned int i = 0 ; i < systemChapters ( )->size ( ) ; i++ ) {
    if ( systemChapters ( )->at ( i )->name ( ) == chapter ) {
      systemChapters ( )->at ( i )->printHistory ( );
    }
  }

  if ( chan == -1 ) {
    for ( unsigned int i = 0 ; i < CHANNELS ; i++ ) {
      channel ( i )->printChapterHistory ( chapter );
    }
  }
  else if ( chan < CHANNELS ) {
    channel ( chan )->printChapterHistory ( chapter );
  }

}
