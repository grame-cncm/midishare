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

#ifndef CHAPTER_W_READER_H
#define CHAPTER_W_READER_H

#include "ChannelChapterReader.h"

/**
 * A reader for channel chapter W.
 */

class ChapterWReader : public ChannelChapterReader
{

public :

  virtual ~ ChapterWReader ( ) { }

  /**
   * Constructor.

@verbatim
_lastPitchWheelLSB = -1
_lastPitchWheelMSB = -1
@endverbatim

   */ 
  ChapterWReader ( ChannelReader * channelReader );

  /**
   * Notifications.
   *
   * @li PitchWheel

@verbatim
_lastPitchWheelLSB = PitchWheel.LSB
_lastPitchWheelMSB = PitchWheel.MSB
@endverbatim

   */
  void notifyCommand ( MidiEvPtr command );

  /**
   * Reset notifications.
   */
  void notifyResetCommand ( MidiEvPtr command, short resetType );

  /**
   * Read chapter.

@verbatim
if ( _lastPitchWheelLSB != FIRST || _lastPitchWheelMSB != SECOND )
	addRecoveryCommand ( PitchWheel ( FIRST, SECOND ) )
@endverbatim

   */
  int readChapter ( uint8 * buffer );

  /**
   * Print the historical informations kept by the chapter.
   */
  void printHistory ( );

protected :

  /** LSB of the last PitchWheel command */
  short _lastPitchWheelLSB;
  /** MSB of the last PitchWheel command */
  short _lastPitchWheelMSB;

};

#endif // CHAPTER_W_READER_H
