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

#ifndef CHAPTER_READER_H
#define CHAPTER_READER_H

#include "types.h"
#include "Chapter.h"

#include <list>

#define COMMAND 0x01
#define S_BIT   0x02
#define Y_BIT   0x04

/**
 * A recovery journal chapter reader.
 *
 * This is a common interface to both SystemChapterReader and
 * ChannelChapterReader.
 */

class ChapterReader : public Chapter
{

public :

  virtual ~ ChapterReader ( ) { }

  /**
   * Reads a recovery journal chapter from a buffer.
   *
   * A ChapterReader implementation must implement this call. This
   * will be called at the beginning of a payload analyse and must be
   * used to read the content of the corresponding chapter and to
   * create the recovery commands needed.
   *
   * @param buffer Buffer to read from.
   *
   * @result The number of bytes that were read for this chapter.
   */
  virtual int readChapter ( uint8 * buffer ) = 0;

  /**
   * Retrieves the next recovery command for the chapter.
   *
   * This doesn't have to be reimplemented in the various chapters
   * implementations.
   *
   * @see StreamReader::getRecoveryCommand( MidiEvPtr * command )
   */
  MidiEvPtr getRecoveryCommand ( );

protected :

  /**
   * Adds a recovery command for the chapter.
   *
   * This method should be used while reading the chapter to store the
   * recovery commands. This will also notifies the created commands to
   * the RecoveryJournalReader.
   */
  virtual void addRecoveryCommand ( MidiEvPtr command ) = 0;

  /** The recovery commands created while reading the chapter */
  list < MidiEvPtr > _recoveryCommands;

};

#endif // CHAPTER_READER_H
