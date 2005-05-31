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

#ifndef SYSTEM_CHAPTER_READER_H
#define SYSTEM_CHAPTER_READER_H

#include "types.h"
#include "ChapterReader.h"
#include "RecoveryJournalReader.h"
#include "StreamReader.h"

class RecoveryJournalReader;

/**
 * A different implementation of this interface
 * will be used by a RecoveryJournalReader to read each chapter of a
 * recovery journal section and to produce the corresponding recovery
 * commands.
 */

class SystemChapterReader : public ChapterReader
{

public :
  
  virtual ~ SystemChapterReader ( ) { }

  /** The current payload number of the stream */ 
  uint32 currentPayloadNumber ( ) {
    return _recoveryJournalReader->streamReader ( )->currentPayloadNumber ( );
  }
  
  /** The recovery journal owning the chapter. */
  RecoveryJournal * recoveryJournal ( ) {
    return _recoveryJournalReader;
  }

protected :

  /**
   * Constructor.
   */
  SystemChapterReader ( RecoveryJournalReader * recoveryJournalReader )
    : _recoveryJournalReader ( recoveryJournalReader ) { }

  void addRecoveryCommand ( MidiEvPtr command );

  /** The RecoveryJournalReader using this SystemChapterReader */
  RecoveryJournalReader * _recoveryJournalReader;

};

#endif // SYSTEM_CHAPTER_READER_H
