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

#ifndef CHAPTER_H
#define CHAPTER_H

#include <MidiShare.h>
#include "types.h"
#include "RecoveryJournal.h"

/**
 * A recovery journal chapter.
 *
 * This is a common interface to both ChapterReader and ChapterWriter
 * objects.
 *
 * Each recovery journal chapter is in charge of keeping the
 * historical data of the stream that it might need to perform its
 * tasks.
 */

class Chapter
{

public :

  /** Name of the chapter. */
  char name ( );

  /**
   * Notify a command to the chapter.
   *
   * A Chapter implementation, either chapter reader or chapter
   * writer, must implement this call. This must be used to update the
   * historical data kept by the chapter.
   *
   * This will be called by a RecoveryJournal object or a Channel
   * object the Chapter had register with for command notification.
   *
   * @see RecoveryJournal::registerForCommandNotification
   * @see Channel::registerForCommandNotification
   * @see RecoveryJournal::notifyCommand
   */
  virtual void notifyCommand ( MidiEvPtr command ) = 0;

  /**
   * Notify a reset command to the chapter.
   *
   * A Chapter implementation, either chapter reader or chapter
   * writer, must implement this call. This must be used to update,
   * and probably purge, the historical data kept by the chapter.
   *
   * This will be called by a RecoveryJournal object or a Channel
   * object. All chapters will be notified of all reset commands ;
   * this doesn't need any registration.
   *
   * @see RecoveryJournal::notifyResetCommand
   */
  virtual void notifyResetCommand ( MidiEvPtr command, short resetType ) = 0;

  /**
   * Prints some historical informations about a chapter.
   *
   * A Chapter implementation, either chapter reader or chapter
   * writer, must implement this call.
   *
   * @see StreamWriter::printChapterHistory
   */
  virtual void printHistory ( ) = 0;

protected :

  /** The current payload number of the stream. */
  virtual uint32 currentPayloadNumber ( ) = 0;

  /** The RecoveryJournalWriter or RecoveryJournalReader object owing this chapter. */
  virtual RecoveryJournal * recoveryJournal ( ) = 0;

  /** The current checkpoint of the stream. */
  virtual uint32 checkpoint ( );

  /** Name of the chapter. */
  char _name;

};

#endif // CHAPTER_H
