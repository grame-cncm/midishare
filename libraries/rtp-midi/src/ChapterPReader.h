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

#ifndef CHAPTER_P_READER_H
#define CHAPTER_P_READER_H

#include "ChannelChapterReader.h"

/**
 * A reader for channel chapter P.
 */

class ChapterPReader : public ChannelChapterReader
{

public :

  virtual ~ ChapterPReader ( ) { }

  /**
   * Constructor.

@verbatim
_lastProgChange		= -1
_activeBankMSB		= -1
_activeBankLSB		= -1
_lastBankMSB		= -1
_lastBankLSB		= -1
_bankReset		= false
@endverbatim

   */ 
  ChapterPReader ( ChannelReader * channelReader );

  /**
   * Notifications.
   *
   * @li ProgChange

@verbatim
_lastProgChange = ProgChange
_activeBankMSB = lastBankMSB
_activeBankLSB = lastBankLSB
@endverbatim

   * @li BankMSB

@verbatim
_lastBankMSB = BankMSB
_lastBankLSB = 0
_bankReset = false
@endverbatim

   * @li BankLSB

@verbatim
_lastBankLSB = BankLSB
@endverbatim

   */
  void notifyCommand ( MidiEvPtr command );

  /**
   * Reset notifications.
   *
   * @li ResetAllControllers

@verbatim
_bankReset = true
@endverbatim

   */
  void notifyResetCommand ( MidiEvPtr command, short resetType );

  /**
   * Read chapter.

@verbatim
if B && ( PROGRAM != _lastProgChange ||

1.	Bank1 - Prog1
	Prog2		[lost]
2.	Bank1 - Prog1
	Bank2 - Prog2	[lost]

   BANK-MSB != _activeBankMSB || BANK-LSB != _activeBankLSB ) :

3.	Bank1 - Prog1
	Bank2 - Prog1	[lost]

	if BANK-MSB != activeBankMSB :
		addRecoveryCommand ( BankMSB ( BANK-MSB ) )
	if BANK-LSB != activeBankLSB :
		addRecoveryCommand ( BankLSB ( BANK-LSB ) )
	if X && ! _bankReset : // we lost a ResetAllControllers
		addRecoveryCommand ( ResetAllControllers )
	addRecoveryCommand ( ProgChange )

else if PROGRAM != _lastProgChange :

4.	Prog1
	Prog2		[lost]

	addRecoveryCommand ( ProgChange ( PROGRAM ) )
@endverbatim

   */
  int readChapter ( uint8 * buffer );

  /**
   * Print the historical informations kept by the chapter.
   */
  void printHistory ( );

protected :

  /** Last ProgChange command. */
  short _lastProgChange;
  /** Last BankMSB before _lastProgChange */
  short _activeBankMSB;
  /** Last BankLSB before _lastProgChange */
  short _activeBankLSB;
  /** Last BankMSB */
  short _lastBankMSB;
  /** Last BankLSB */
  short _lastBankLSB;
  /** Was there a ResetAllController since _lastBankMSB ? */
  bool _bankReset;

};

#endif // CHAPTER_P_READER_H
