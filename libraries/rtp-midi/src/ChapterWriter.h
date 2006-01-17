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

#ifndef CHAPTER_WRITER_H
#define CHAPTER_WRITER_H

#include "types.h"
#include "Chapter.h"
#include "binaryUtils.h"

/**
 * A recovery journal chapter writer.
 *
 * This is a common interface to both SystemChapterReader and
 * ChannelChapterReader.
 */

class ChapterWriter : public Chapter
{

public :

  ChapterWriter ( ) : _chapterInclusion ( DEFAULT ) { }

  virtual ~ ChapterWriter ( ) { }

  /**
   * Calculates a recovery journal chapter.
   *
   * A ChapterWriter implementation must implement this call. This
   * will be called at the beginning of a payload creation and must be
   * used to calculate the content of the corresponding chapter for a
   * new recovery journal section.
   *
   * @return The size in bytes of the calculated chapter.
   */
  virtual void calculateChapter ( ) = 0;
  
  /** Returns the buffer where the last chapter was calculated. */
  virtual uint8 * chapter ( ) = 0;

  /** Return the size of the chapter in bytes. */
  virtual unsigned int chapterLength ( ) = 0;

  /**
   * Writes the recovery journal section chapter to a buffer.
   *
   * This doesn't have to be reimplemented in the various chapters
   * implementations.
   *
   * @param buffer The buffer in which the last recovery journal
   * section chapter calculated by calculateChapter( ) will be written
   * to.
   *
   * @result TRUE if the inclusion rule for the chapter allows it to
   * figure in the recovery journal.
   */
  bool writeChapter ( uint8 * buffer );

  /** Sets the S bit of the parent structure of the chapter to 0. */
  virtual void unsetParentSBit ( ) = 0;

  /**
   * @see Chapter::notifyResetCommand
   *
   * This implementation for chapter writers also deals with the
   * ACTIVE chapter inclusion rule that alters some reset command
   * semantics.
   */
  virtual void notifyResetCommand ( MidiEvPtr command, short resetType );

  /**
   * Sets the chapter inclusion rule for a given chapter.
   *
   * @param c The new inclusion rule for the chapter.
   *
   * The details of the various inclusion rules can be found in [1]
   * Appendix C.1.3. Here is a short summary :
   * - UNUSED : The chapter is not figuring in the recovery journal
   * and the corresponding commands must not appears in the
   * payload. This is ensured by a CommandFilter object.
   * - NEVER : The chapter never appear in the journal section.
   * - DEFAULT : The default behaviour should be applied.
   * - ANCHOR : The checkpoint is set to the first packet in the
   * stream. There is no more purge of the history by checkpoint.
   * - ACTIVE : The Reset C-Active and Reset N-Active commands with
   * are not usual reset commands lose there semantics.
   * - ACTIVE_ANCHOR : This policy combines the ANCHOR and the ACTIVE policy.
   *
   * This doesn't have to be reimplemented in the various chapters
   * implementations.
   *
   * @see @ref StreamConfiguration
   * @see StreamWriter::setChapterInclusion
   * @see RecoveryJournalWriter::setChapterInclusion
   */
  virtual void setChapterInclusion ( chapterInclusion c ) = 0;

protected :

  /** The current checkpoint for the stream. */
  virtual uint32 checkpoint ( );

  /** The inclusion rule for the chapter */
  chapterInclusion _chapterInclusion;

};

#endif // CHAPTER_WRITER_H
