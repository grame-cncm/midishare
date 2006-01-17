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

#ifndef CHAPTER_N_WRITER_H
#define CHAPTER_N_WRITER_H

#include "ChannelChapterWriter.h"

#include <list>

/**
 * A writer for channel chapter N.
 */

class ChapterNWriter : public ChannelChapterWriter
{

public :

  virtual ~ ChapterNWriter ( ) { }

  /**
   * Constructor.
   */
  ChapterNWriter ( ChannelWriter * channelWriter );

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
else
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
   * Calculate chapter.

@verbatim
low = 15
high = 0
len = 0

add 2 octet header
for all noteInfo in _history :
	if noteInfo.payload >= checkpoint :
		if noteInfo.vel > 0 :
			len++
			add the corresponding NOTELOG
			if noteInfo.payload = currentPayloadNumber - 1 : set S bit
		else :
			if noteInfo.payload = currentPayloadNumber - 1 : set B bit
		low = min ( low, noteInfo.pitch / 8 )
		high = max ( high, noteInfo.pitch / 8 )

LEN = len
LOW = low
HIGH = high
_chapterLength = 2 + len * 2

if low != 15 || high != 0 :
	add high - low + 1 OFFBITS
	_chapterLength += high - low + 1
	for all noteInfo in _history :
		if noteInfo.vel = 0 :
			set the corresponding OFFBIT

if _chapterLength = 2 : _chapterLength = 0
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
  uint8 * _chapter;

  /** The length of the last calculated chapter. */
 unsigned int _chapterLength;

  /**
   * A structure to keep the historical information about Note events
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

#endif // CHAPTER_N_WRITER_H
