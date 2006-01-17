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

#ifndef CHAPTER_N_READER_H
#define CHAPTER_N_READER_H

#include "ChannelChapterReader.h"

#include <list>

/**
 * A reader for channel chapter N.
 */

class ChapterNReader : public ChannelChapterReader
{

public :

  virtual ~ ChapterNReader ( ) { }

  /**
   * Constructor.
   */
  ChapterNReader ( ChannelReader * channelReader );

  /**
   * Notifications.
   *
   * @li KeyOn, KeyOff

@verbatim
for all noteInfo in _history :
	if noteInfo.pitch = Key.Pitch :
		_history.erase ( noteInfo )

noteInfo.pitch = Key.Pitch
if KeyOff :
	noteInfo.vel = 0
else :
	noteInfo.vel = Key.Velocity
noteInfo.payload = currentPayloadNumber
_history.add ( noteInfo )
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
	for all noteInfo in _history :
		if noteInfo.pitch = PITCH :
			if noteInfo.velocity != VELOCITY :
				addRecoveryCommand ( KeyOn ( PITCH, VELOCITY )
			break
	if _history.end :
		addRecoveryCommand ( KeyOn ( PITCH, VELOCITY )

if ! ( LOW = 15 && HIGH = 0 ) && ! ( LOW = 15 && HIGH = 1 ) :
	for all OFFBIT :
		if OFFBIT = 1 :
			for all noteInfo in _history :
				if noteInfo.pitch = OFFBIT.Pitch :
					if noteInfo.vel != 0 :
						addRecoveryCommand ( KeyOff ( OFFBIT.Pitch ) )
					break
			if _history.end :
				addRecoveryCommand ( KeyOff ( OFFBIT.Pitch ) )
@endverbatim

   */
  int readChapter ( uint8 * buffer );

  /**
   * Print the historical informations kept by the chapter.
   */
  void printHistory ( );

protected :

  /**
   * A structure to keep the historical information about note events
   * for one pitch.
   */
  typedef struct TNoteInfo
  {
    /** Pitch of the note. */
    unsigned short pitch;
    /** Velocity of the note. */
    unsigned short vel;
    /** Payload in which this note appears. */
    uint32 payload;
  } TNoteInfo;

  /** Historical data for the chapter. */
  list < TNoteInfo > _history;

};

#endif // CHAPTER_N_READER_H
