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

#include "Chapter.h"
#include "Channel.h"

#include <iostream>

char Channel::channelChapterName[] = { 'P', 'C', 'M', 'W', 'N', 'E', 'T', 'A' };

Channel::Channel ( unsigned short channelNumber )
  : _channelNumber ( channelNumber )
{

  _regKeyOff.clear ( );
  _regKeyOn.clear ( );
  _regKeyPress.clear ( );
  _regCtrlChange.clear ( );
  _regProgChange.clear ( );
  _regChanPress.clear ( );
  _regPitchWheel.clear ( );
  _regCtrl14b.clear ( );
  _regRegParam.clear ( );
  _regNonRegParam.clear ( );

}

unsigned short Channel::channelNumber ( )
{

  return _channelNumber;

}

void Channel::notifyCommand ( const MidiEvPtr command )
{

  switch EvType ( command ) {

  case typeKeyOff :
    for ( unsigned int i = 0 ; i < _regKeyOff.size ( ) ; i++ ) {
      _regKeyOff[i]->notifyCommand ( command );
    }
    break;

  case typeKeyOn :
    for ( unsigned int i = 0 ; i < _regKeyOn.size ( ) ; i++ ) {
      _regKeyOn[i]->notifyCommand ( command );
    }
    break;

  case typeKeyPress :
    for ( unsigned int i = 0 ; i < _regKeyPress.size ( ) ; i++ ) {
      _regKeyPress[i]->notifyCommand ( command );
    }
    break;

  case typeCtrlChange :
    for ( unsigned int i = 0 ; i < _regCtrlChange.size ( ) ; i++ ) {
      _regCtrlChange[i]->notifyCommand ( command );
    }
    break;

  case typeProgChange :
    for ( unsigned int i = 0 ; i < _regProgChange.size ( ) ; i++ ) {
      _regProgChange[i]->notifyCommand ( command );
    }
    break;

  case typeChanPress :
    for ( unsigned int i = 0 ; i < _regChanPress.size ( ) ; i++ ) {
      _regChanPress[i]->notifyCommand ( command );
    }
    break;

  case typePitchWheel :
    for ( unsigned int i = 0 ; i < _regPitchWheel.size ( ) ; i++ ) {
      _regPitchWheel[i]->notifyCommand ( command );
    }
    break;

  case typeCtrl14b :
    for ( unsigned int i = 0 ; i < _regCtrl14b.size ( ) ; i++ ) {
      _regCtrl14b[i]->notifyCommand ( command );
    }
    break;

  case typeRegParam :
    for ( unsigned int i = 0 ; i < _regRegParam.size ( ) ; i++ ) {
      _regRegParam[i]->notifyCommand ( command );
    }
    break;

  case typeNonRegParam :
    for ( unsigned int i = 0 ; i < _regNonRegParam.size ( ) ; i++ ) {
      _regNonRegParam[i]->notifyCommand ( command );
    }
    break;

  }

}

void Channel::notifyResetCommand ( const MidiEvPtr command, short resetType )
{

  for ( unsigned int i = 0 ; i < channelChapters ( )->size ( ) ; i++ ) {
    channelChapters ( )->at ( i )->notifyResetCommand ( command, resetType );
  }

}

void Channel::registerForCommandNotification ( Chapter * chapter, unsigned short commandType )
{

  switch ( commandType ) {

  case typeKeyOff :
    _regKeyOff.push_back ( chapter );
    break;

  case typeKeyOn :
    _regKeyOn.push_back ( chapter );
    break;

  case typeKeyPress :
    _regKeyPress.push_back ( chapter );
    break;

  case typeCtrlChange :
    _regCtrlChange.push_back ( chapter );
    break;

  case typeProgChange :
    _regProgChange.push_back ( chapter );
    break;

  case typeChanPress :
    _regChanPress.push_back ( chapter );
    break;

  case typePitchWheel :
    _regPitchWheel.push_back ( chapter );
    break;

  case typeCtrl14b :
    _regCtrl14b.push_back ( chapter );
    break;

  case typeRegParam :
    _regRegParam.push_back ( chapter );
    break;

  case typeNonRegParam :
    _regNonRegParam.push_back ( chapter );
    break;

  }

}

void Channel::printChapterHistory ( char chapter )
{

  for ( unsigned int i = 0 ; i < channelChapters ( )->size ( ) ; i++ ) {
    if ( channelChapters ( )->at ( i )->name ( ) == chapter ) {
      channelChapters ( )->at ( i )->printHistory ( );
    }
  }

}
