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

#ifndef CHAPTER_A_READER_H
#define CHAPTER_A_READER_H

#include "ChannelChapterReader.h"

#include <list>

/**
 * A reader for channel chapter A.
 */

class ChapterAReader : public ChannelChapterReader
{

public :

  virtual ~ ChapterAReader ( ) { }

  /**
   * Constructor.
   */ 
  ChapterAReader ( ChannelReader * channelReader );

  /**
   * Notifications.
   *
   * @li KeyPress

@verbatim
for all notePressInfo in _history :
	if notePressInfo.pitch = KeyPress.Pitch :
		_history.erase ( notePressInfo )

notePressInfo.pitch = KeyPress.Pitch
notePressInfo.press = KeyPress.Press
notePressInfo.payload = currentPayloadNumber
_history.add ( notePressInfo )
@endverbatim

   */
  void notifyCommand ( MidiEvPtr command );

  /**
   * Reset notifications.
   *
   * @li Reset N-Active

@verbatim
_history.clear
@endverbatim

   */
  void notifyResetCommand ( MidiEvPtr command, short resetType );

  /**
   * Read chapter.

@verbatim
for all NOTE_LOG :
	for all notePressInfo in _history :
		if notePressInfo.pitch = PITCH :
			if notePressInfo.press != PRESSURE :
				addRecoveryCommand ( KeyPress ( PITCH, PRESSURE )
			break
	if _history.end :
		addRecoveryCommand ( KeyPress ( PITCH, PRESSURE )
@endverbatim

   */
  int readChapter ( uint8 * buffer );

  /**
   * Print the historical informations kept by the chapter.
   */
  void printHistory ( );

protected :

  /**
   * A structure to keep the historical information about KeyPress
   * events for one pitch.
   */
  typedef struct TNotePressInfo
  {
    /** Pitch of the note. */
    unsigned short pitch;
    /** Pressure of the note. */
    unsigned short press;
    /** Payload in which this note appears. */
    uint32 payload;
  } TNotePressInfo;

  /** Historical data for the chapter. */
 list < TNotePressInfo > _history;

};

#endif // CHAPTER_A_READER_H
