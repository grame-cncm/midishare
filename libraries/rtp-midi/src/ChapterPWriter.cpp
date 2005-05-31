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

#include "ChapterPWriter.h"

#include "ChannelWriter.h"
#include <MidiShare.h>

#include "printingUtils.h"

#include <iostream>

ChapterPWriter::ChapterPWriter ( ChannelWriter * channelWriter )
  : ChannelChapterWriter ( channelWriter )
{

  _name = 'P';
  _channelWriter->registerForCommandNotification ( this, typeProgChange );
  _channelWriter->registerForCommandNotification ( this, typeCtrlChange );

  _lastBankMSB = -1;
  _bankReset = false;
  _progChange = false;

}

void ChapterPWriter::notifyCommand ( MidiEvPtr command )
{

  /* ProgChange */
  if ( EvType ( command ) == typeProgChange ) {

    _nextChapter[0] = Data ( command ) [0];
    if ( _lastBankMSB != -1 ) {
      _nextChapter[1] = 0x80;
      _nextChapter[1] |= _lastBankMSB;
      _nextChapter[2] = _lastBankLSB;
      if ( _bankReset ) {
	_nextChapter[2] |= 0x80;
      }
    }
    _lastProgChangePayload = currentPayloadNumber ( );
    _progChange = true;

  }

  /* BankMSB */
  if ( EvType ( command ) == typeCtrlChange && Data ( command ) [0] == 0 ) {

    _lastBankMSB = Data ( command ) [1];
    _lastBankLSB = 0;
    _bankReset = false;

  }

  /* BankLSB */
  if ( EvType ( command ) == typeCtrlChange && Data ( command ) [0] == 32 ) {

    _lastBankLSB = Data ( command ) [1];

  }

}

void ChapterPWriter::notifyResetCommand ( MidiEvPtr command, short resetType )
{

  /* ResetAllControllers

     bankReset = true
  */
  if ( EvType ( command ) == typeCtrlChange && Data ( command ) [0] == 121 ) {

    _bankReset = true;

  }

}

void ChapterPWriter::calculateChapter ( )
{

  if ( _progChange && _lastProgChangePayload >= checkpoint ( ) ) {
    memcpy ( _chapter, _nextChapter, 3 );
    if ( _lastProgChangePayload == currentPayloadNumber ( ) - 1 ) {
      setFlag ( & _chapter[0], 0, 0 );
      unsetParentSBit ( );
    }
    else {
      setFlag ( & _chapter[0], 0, 1 );
    }
    _chapterLength = 3;
  }
  else {
    _chapterLength = 0;
  }

}

uint8 * ChapterPWriter::chapter ( )
{

  return _chapter;

}

unsigned int ChapterPWriter::chapterLength ( )
{

  return _chapterLength;

}

void ChapterPWriter::printHistory ( )
{

  cout << "# CHAPTER P ( " << _channelWriter->channelNumber ( ) << " ) WRITER HISTORY" << endl;
  cout << "   progChange\t" << _progChange << endl;
  cout << "   lastProgChangePayload\t" << _lastProgChangePayload;
  cout << "   Chapter P history" << endl;
  cout << "   lastBankMSB\t" << _lastBankMSB;
  cout << "   lastBankLSB\t" << _lastBankLSB;
  cout << "   bankReset\t" << _bankReset << endl;

}
