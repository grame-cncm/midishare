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

#include "ChapterPReader.h"

#include "ChannelReader.h"
#include <MidiShare.h>

#include "eventUtils.h"

#include <iostream>

ChapterPReader::ChapterPReader ( ChannelReader * channelReader )
  : ChannelChapterReader ( channelReader )
{

  _name = 'P';
  _channelReader->registerForCommandNotification ( this, typeProgChange );
  _channelReader->registerForCommandNotification ( this, typeCtrlChange );

  _lastProgChange = -1;
  _activeBankMSB = -1;
  _activeBankLSB = -1;
  _lastBankMSB = -1;
  _lastBankLSB = -1;
  _bankReset = false;

}

void ChapterPReader::notifyCommand ( MidiEvPtr command )
{

  if ( EvType ( command ) == typeProgChange ) {

    _lastProgChange = Data ( command ) [0];
    _activeBankMSB = _lastBankMSB;
    _activeBankLSB = _lastBankLSB;

  }

  if ( EvType ( command ) == typeCtrlChange && Data ( command ) [0] == 0 ) {

    _lastBankMSB = Data ( command ) [1];
    _lastBankLSB = 0;
    _bankReset = false;

  }

  if ( EvType ( command ) == typeCtrlChange && Data ( command ) [0] == 32 ) {

    _lastBankLSB = Data ( command ) [1];

  }

}

void ChapterPReader::notifyResetCommand ( MidiEvPtr command, short resetType )
{

  /** Reset All Controllers */
  if ( EvType ( command ) == typeCtrlChange && Data ( command ) [0] == 121 ) {
    _bankReset = true;
  }

}

int ChapterPReader::readChapter ( uint8 * buffer )
{

  if ( buffer[1] & 0x80 /* B */ &&
       ( ( buffer[0] & 0x7f ) /* PROGRAM */ != _lastProgChange ||
	 ( buffer[1] & 0x7f ) /* BANK-MSB */ != _activeBankMSB ||
	 ( buffer[2] & 0x7f ) /* BANK-LSB */ != _activeBankLSB ) ) {

    if ( ( buffer[1] & 0x7f ) /* BANK-MSB */ != _activeBankMSB ) {
      addRecoveryCommand ( newCtrlChange ( 0, _channelReader->channelNumber ( ), 0, buffer[1] & 0x7f ) );
    }

    if ( ( buffer[2] & 0x7f ) /* BANK-LSB */ != _activeBankLSB ) {
      addRecoveryCommand ( newCtrlChange ( 0, _channelReader->channelNumber ( ), 32, buffer[2] & 0x7f ) );
    }

    if ( ( buffer[2] & 0x80 ) /* X */ && ! _bankReset ) {
      addRecoveryCommand ( newCtrlChange ( 0, _channelReader->channelNumber ( ), 121, 0 ) );
      // on pourrait mettre un flag pour ce ResetAllController un peu particulier
    }

    addRecoveryCommand ( newProgChange ( 0, _channelReader->channelNumber ( ), buffer[0] & 0x7f ) );

  }
  else if ( ( buffer[0] & 0x7f ) /* PROGRAM */ != _lastProgChange ) {

    addRecoveryCommand ( newProgChange ( 0, _channelReader->channelNumber ( ), buffer[0] & 0x7f ) );

  }

  return 3;

}

void ChapterPReader::printHistory ( )
{

  cout << "# CHAPTER P ( " << _channelReader->channelNumber ( ) << " ) READER HISTORY" << endl;
  cout << "   lastProgChange\t" << _lastProgChange << endl;
  cout << "   activeBankMSB\t" << _activeBankMSB << endl;
  cout << "   activeBankLSB" << _activeBankLSB << endl;
  cout << "   lastBankMSB\t" << _lastBankMSB;
  cout << "   lastBankLSB\t" << _lastBankLSB;
  cout << "   bankReset\t" << _bankReset << endl;

}
