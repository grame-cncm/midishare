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

#ifndef RECOVERY_JOURNAL_READER_H
#define RECOVERY_JOURNAL_READER_H

#include "RecoveryJournal.h"
#include "types.h"

class StreamReader;
class ChapterReader;
class ChannelReader;
class SystemChapterReader;

/**
 * A recovery journal reader.
 *
 * A RecoveryJournalReader is owned by an StreamReader and will be
 * used by a PayloadReader to read the recovery journal section of a
 * payload in case of a packet loss.
 *
 * The same RecoveryJournalReader object must be used for the analyse
 * of all the payload of an StreamReader since it is used to store
 * data about the history of the stream.
 *
 * It will provide a stream of recovery commands that can be used to
 * repair the stream.
 *
 * A RecoveryJournalWriter is using a hierarchical structure of
 * ChannelReader and SystemChapterReader mapped to the actual sequence
 * of the recovery journal to analyse each chapter of the section.
 * @see @ref RecoveryJournalStructure
 *
 * It will also be notified with all the outgoing commands by the
 * StreamReader. @see @ref NotificationSystem
 *
 * @see RecoveryJournal
 */

class RecoveryJournalReader : public RecoveryJournal
{

public :

  /**
   * Constructor
   */
  RecoveryJournalReader ( StreamReader * streamReader );

  /**
   * Reads the recovery journal section from a buffer.
   *
   * @param buffer Buffer to read from.
   *
   * @see ChannelReader::readChannel
   * @see ChapterReader::readChapter
   */
  int readJournal ( uint8 * buffer );

  /**
   * Retrieves the next recovery command for the current payload.
   *
   * @see StreamReader::getRecoveryCommand
   * @see ChannelReader::getRecoveryCommand
   * @see ChapterReader::getRecoveryCommand
   */
  MidiEvPtr getRecoveryCommand ( );

  /**
   * The StreamReader object using this RecoveryJournalReader.
   */
  StreamReader * streamReader ( );
  
  /** The system chapters of the recovery journal. */
  vector < Chapter * > * systemChapters ( );

  /** The n_th channel of the recovery journal. */
  Channel * channel ( unsigned short n );

protected :

  /** The StreamReader object using this RecoveryJournalReader object. */
  StreamReader * _streamReader;

  /**
   * The system chapter readers for the recovery journal.
   *
   * This vector holds the sequence of ChapterReader objects that will
   * be used to read the chapters of the recovery journal section.
   */
  vector < SystemChapterReader * > _systemChapters;

  /** The channel readers for the recovery journal. */
  ChannelReader * _channels[CHANNELS];

};

#endif // RECOVERY_JOURNAL_READER_H
