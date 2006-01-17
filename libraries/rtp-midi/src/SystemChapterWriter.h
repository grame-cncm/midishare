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

#ifndef SYSTEM_CHAPTER_WRITER_H
#define SYSTEM_CHAPTER_WRITER_H

#include "types.h"
#include "ChapterWriter.h"
#include "RecoveryJournalWriter.h"
#include "StreamWriter.h"

class RecoveryJournalWriter;

/**
 * A system chapter writer.
 *
 * This is a common interface to any specialized system chapter writer.
 *
 * A different implementation of this interface
 * will be used by a RecoveryJournalWriter to write each system
 * chapter of a recovery journal section.
 */

class SystemChapterWriter : public ChapterWriter
{

public :
  
  virtual ~ SystemChapterWriter ( ) { }

  /** The current payload number of the stream. */
  uint32 currentPayloadNumber ( ) {
    return _recoveryJournalWriter->streamWriter ( )->currentPayloadNumber ( );
  }

  /** The recovery journal owning this chapter. */
  RecoveryJournal * recoveryJournal ( ) {
    return _recoveryJournalWriter;
  }

  void unsetParentSBit ( ) {
    _recoveryJournalWriter->unsetSystemSBit ( );
  }

  /**
   * @see ChapterWriter::setChapterInclusion
   */
  virtual void setChapterInclusion ( chapterInclusion c );

protected :

  /**
   * Constructor.
   */
  SystemChapterWriter ( RecoveryJournalWriter * recoveryJournalWriter )
    : _recoveryJournalWriter ( recoveryJournalWriter ) { }

  /** The RecoveryJournalWriter using this chapter. */
  RecoveryJournalWriter * _recoveryJournalWriter;

};

#endif // SYSTEM_CHAPTER_WRITER_H
