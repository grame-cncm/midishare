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

#ifndef CHAPTER_W_WRITER
#define CHAPTER_W_WRITER

#include "ChannelChapterWriter.h"

/**
 * A writer for channel chapter W.
 */

class ChapterWWriter : public ChannelChapterWriter
{

public :

  virtual ~ ChapterWWriter ( ) { }

  /**
   * Constructor.

@verbatim
_pitchWheel = false
@endverbatim

   */
  ChapterWWriter ( ChannelWriter * channelWriter );

  /**
   * Notifications.
   *
   * @li PitchWheel

@verbatim
_pitchWheel = true
_lastPitchWheelLSB = PitchWheel.LSB
_lastPitchWheelMSB = PitchWheel.MSB
_lastPitchWheelPayload = currentPayloadNumber
@endverbatim

   */
  void notifyCommand ( MidiEvPtr command );

  /**
   * Reset notifications.
   */
  void notifyResetCommand ( MidiEvPtr command, short resetType );

  /**
   * Calculate chapter.

@verbatim
if _pitchWheel :
	FIRST = _lastPitchWheelLSB
	SECOND = _lastPitchWheelMSB
	if _lastPitchWheelPayload < currentPayloadNumber - 1 : set S bit
	_chapterLength = 2
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
  uint8 _chapter[2];

 /** The length of the last calculated chapter. */
  unsigned int _chapterLength;

  /** LSB of the last PitchWheel command */
  short _lastPitchWheelLSB;
  /** MSB of the last PitchWheel command */
  short _lastPitchWheelMSB;
  /** Is there a PitchWheel in the checkpoint history ? */
  bool _pitchWheel;
  /** Payload number of the last PitchWheel */
  uint32 _lastPitchWheelPayload;

};

#endif // CHAPTER_W_WRITER
