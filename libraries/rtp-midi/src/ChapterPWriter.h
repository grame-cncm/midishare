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

#ifndef CHAPTER_P_WRITER
#define CHAPTER_P_WRITER

#include "ChannelChapterWriter.h"

/**
 * A writer for channel chapter P.
 */

class ChapterPWriter : public ChannelChapterWriter
{

public :

  virtual ~ ChapterPWriter ( ) { }

  /**
   * Constructor.

@verbatim
_lastBankMSB		= -1
_bankReset		= false
_progChange		= false
@endverbatim

   */
  ChapterPWriter ( ChannelWriter * channelWriter );

  /**
   * Notifications.
   *
   * @li ProgChange

@verbatim
PROGRAM = ProgChange
if _lastBankMSB != -1 :
	B = 1
	BANK-MSB = _lastBankMSB
	BANK-LSB = _lastBankLSB
	if _bankReset : X = 1
_lastProgChangePayload = currentPayloadNumber
_progChange = true
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
   * Calculate chapter.

@verbatim
if ! _progChange && _lastProgChangePayload >= checkpoint :
	copy ( _chapter, _nextChapter )
	if _lastProgChangePayload == currentPayloadNumber - 1 : set S bit
	_chapterLength = 3
else :
	_chapterLength = 0
@endverbatim

@note The updates on the chapter content are done on a local version
( _localChapter ) while notifying commands. This version is just copied
to _chapter during calculateChapter( ).
  */
  void calculateChapter ( );

  uint8 * chapter ( );

  unsigned int chapterLength ( );

  /**
   * Print the historical informations kept by the chapter.
   */
  void printHistory ( );

protected :
 
 /** The buffer storing the last calculated chapter. */
  uint8 _chapter[3];

 /** The length of the last calculated chapter. */
  unsigned int _chapterLength;

  /** The buffer used to calculate the next chapter. */
  uint8 _nextChapter[3];
  /** Is there a ProgChange in the checkpoint history ? */
  bool _progChange;
  /** Payload number of the last ProgChange */
  uint32 _lastProgChangePayload;
  /** Last BankMSB */
  short _lastBankMSB;
  /** Last BankLSB */
  short _lastBankLSB;
  /** Was there a ResetAllController since _mostRecentBankMSB ? */
  bool _bankReset;

};

#endif // CHAPTER_P_WRITER
