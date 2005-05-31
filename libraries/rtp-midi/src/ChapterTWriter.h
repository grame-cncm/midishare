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

#ifndef CHAPTER_T_WRITER
#define CHAPTER_T_WRITER

#include "ChannelChapterWriter.h"

/**
 * A writer for channel chapter T.
 */

class ChapterTWriter : public ChannelChapterWriter
{

public :

  virtual ~ ChapterTWriter ( ) { }

  /**
   * Constructor.

@verbatim
_chanPress = false
@endverbatim

   */
  ChapterTWriter ( ChannelWriter * channelWriter );

  /**
   * Notifications.
   *
   * @li ChanPress

@verbatim
_chanPress = true
_lastPressure = ChanPress.Pressure
_lastChanPressPayload = currentPayloadNumber
@endverbatim

   */
  void notifyCommand ( MidiEvPtr command );

  /**
   * Reset notifications.
   *
   * @li Reset N-Active

@verbatim
_chanPress = false
@endverbatim

   */
  void notifyResetCommand ( MidiEvPtr command, short resetType );

  /**
   * Calculate chapter.

@verbatim
if _chanPress :
	PRESSURE = _lastPressure
	if _lastChanPressPayload = currentPayloadNumber - 1 : set S bit
	_chapterLength = 1
else :
	_chapterLength = 0
@endverbatim

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
  uint8 _chapter[1];

 /** The length of the last calculated chapter. */
  unsigned int _chapterLength;

  /** Pressure of the last ChanPress command */
  short _lastPressure;
  /** Is there a N-active ChanPress in the checkpoint history ? */
  bool _chanPress;
  /** Payload number of the last ChanPress */
  uint32 _lastChanPressPayload;

};

#endif // CHAPTER_T_WRITER
