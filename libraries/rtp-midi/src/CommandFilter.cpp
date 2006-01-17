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

#include "CommandFilter.h"

#include "eventUtils.h"
#include "binaryUtils.h"
#include "printingUtils.h"

CommandFilter::CommandFilter ( )
{

  for ( unsigned short i = 0 ; i < 256 ; i++ ) {

    switch ( commandCategory ( i ) ) {

    case SYSTEM_COMMAND :
      setBufferFlag ( _systemFilter, i, true );
      for ( unsigned short j = 0 ; j < 16 ; j++ ) {
	setBufferFlag ( _channelFilter[j], i, false );
      }
      break;

    case CHANNEL_COMMAND :
      setBufferFlag ( _systemFilter, i, false );
      for ( unsigned short j = 0 ; j < 16 ; j++ ) {
	setBufferFlag ( _channelFilter[j], i, true );
      }
      break;

    case UNSUPPORTED_COMMAND :
      setBufferFlag ( _systemFilter, i, false );
      for ( unsigned short j = 0 ; j < 16 ; j++ ) {
	setBufferFlag ( _channelFilter[j], i, false );
      }
      break;

    }

  }

}

short CommandFilter::acceptCommand ( MidiEvPtr command )
{

  switch ( commandCategory ( EvType ( command ) ) ) {

  case SYSTEM_COMMAND :
    if ( getBufferFlag ( _systemFilter, EvType ( command ) ) ) {
      return 1;
    }
    else {
      return -1;
    }

  case CHANNEL_COMMAND :
    if ( getBufferFlag ( _channelFilter[Chan ( command )], EvType ( command ) ) ) {
      return 1;
    }
    else {
      return -1;
    }

  case UNSUPPORTED_COMMAND :
    return -2;

  }

  return -1;

}

void CommandFilter::setChapterUnused ( char chapterName, short channel )
{

  if ( channel == -1 ) {
    for ( unsigned short i = 0 ; i < 16 ; i++ ) {
      setChapterUnused ( chapterName, i );
    }
    return;
  }

  switch ( chapterName ) {

  case 'P' :
    setBufferFlag ( _channelFilter[channel], typeProgChange, false );
    break;

  case 'C' :
    setBufferFlag ( _channelFilter[channel], typeCtrlChange, false );
    break;

  case 'W' :
    setBufferFlag ( _channelFilter[channel], typePitchWheel, false );
    break;

  case 'N' :
    setBufferFlag ( _channelFilter[channel], typeKeyOn, false );
    setBufferFlag ( _channelFilter[channel], typeKeyOff, false );
    break;

  case 'T' :
    setBufferFlag ( _channelFilter[channel], typeChanPress, false );
    break;

  case 'A' :
    setBufferFlag ( _channelFilter[channel], typeKeyPress, false );
    break;

  }

}

void CommandFilter::setChapterUsed ( char chapterName, short channel )
{

  if ( channel == -1 ) {
    for ( unsigned short i = 0 ; i < 16 ; i++ ) {
      setChapterUsed ( chapterName, i );
    }
    return;
  }

  switch ( chapterName ) {

  case 'P' :
    setBufferFlag ( _channelFilter[channel], typeProgChange, true );
    break;

  case 'C' :
    setBufferFlag ( _channelFilter[channel], typeCtrlChange, true );
    break;

  case 'W' :
    setBufferFlag ( _channelFilter[channel], typePitchWheel, true );
    break;

  case 'N' :
    setBufferFlag ( _channelFilter[channel], typeKeyOn, true );
    setBufferFlag ( _channelFilter[channel], typeKeyOff, true );
    break;

  case 'T' :
    setBufferFlag ( _channelFilter[channel], typeChanPress, true );
    break;

  case 'A' :
    setBufferFlag ( _channelFilter[channel], typeKeyPress, true );
    break;

  }

}
