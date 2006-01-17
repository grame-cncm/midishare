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

#ifndef RECOVERY_JOURNAL_WRITER_H
#define RECOVERY_JOURNAL_WRITER_H

#include "RecoveryJournal.h"

class StreamWriter;
class ChapterWriter;
class ChannelWriter;
class SystemChapterWriter;

/**
 * A recovery journal writer.
 *
 * A RecoveryJournalWriter is owned by a StreamWriter and will be
 * used by a PayloadWriter to write the recovery journal section of a
 * payload.
 *
 * The same RecoveryJournalWriter object must be used for the creation
 * of all the payloads of a StreamWriter since it is used to store
 * data about the history of the stream.
 *
 * The recovery journal will be written at the end of the payload but,
 * right from the beginning of the payload creation, we will need to
 * know its size to limit the size of the MIDI command section. We
 * will use for that the RecoveryJournalWriter::calculateJournal call at the beginning
 * of the payload creation and then the RecoveryJournalWriter::writeJournal call by the
 * time of payload finalization.
 *
 * A RecoveryJournalWriter is using a hierarchical structure of
 * ChannelWriter and SystemChapterWriter mapped to the actual sequence
 * of the recovery journal to create each chapter of the section.
 * @see @ref RecoveryJournalStructure
 *
 * It will also be notified with all the incoming commands by the
 * StreamWriter. @see @ref NotificationSystem
 */

class RecoveryJournalWriter : public RecoveryJournal
{

public :

  /**
   * Constructor
   */
  RecoveryJournalWriter ( StreamWriter * streamWriter );

  /**
   * Calculates the recovery journal.
   *
   * This will calculate the recovery journal section for a new
   * payload.
   *
   * @return The size in bytes of the calculated recovery journal
   * section.
   *
   * @see ChapterWriter::calculateChapter
   * @see ChannelWriter::calculateChannel
   */
  unsigned int calculateJournal ( );

  /**
   * Writes the recovery journal section to a buffer.
   *
   * @param buffer The buffer in which the last recovery
   * journal section calculated by RecoveryJournalWriter::calculateJournal will be written
   * to.
   *
   * @return The size in bytes of the recovery journal section.
   *
   * @see ChapterWriter::writeChapter
   * @see ChannelWriter::writeChannel
   */
  int writeJournal ( uint8 * buffer );

  /** Unset the journal S bit. */
  void unsetSBit ( );

  /** Unset the journal system and journal S bit. */
  void unsetSystemSBit ( );

  /**
   * The StreamWriter object using this RecoveryJournalWriter.
   */
  StreamWriter * streamWriter ( );

  /**
   * The system chapters of the recovery journal.
   */
  vector < Chapter * > * systemChapters ( );

  /**
   * The n_th channel of the recovery journal.
   */
  Channel * channel ( unsigned short n );

  /**
   * @see ChannelWriter::notifyResetCommand
   * @see ChapterWriter::notifyResetCommand
   */
  virtual void notifyResetCommand ( const MidiEvPtr command, short resetType );

  /**
   * Sets the chapter inclusion rule for a given chapter and channel.
   *
   * @param chapterName The name of the chapter which is concerned.
   * @param c The new inclusion rule for the chapter.
   * @param channel In case of a channel command, the channel which is
   * concerned by this change. By default all the channels will be
   * concerned.
   *
   * @see @ref StreamConfiguration
   * @see StreamWriter::setChapterInclusion
   * @see ChannelWriter::setChapterInclusion
   */
  void setChapterInclusion ( char chapterName, chapterInclusion c, short channel = -1 );

  /**
   * Sets the controller log tool for a given controller and channel.
   *
   * @param controller The number of the concerned controller.
   * @param tool The tool or set of tools that must be used to log the
   * controller.
   * @param channel The channel which is concerned by this change. By
   * default all the channels will be concerned.
   *
   * @see @ref StreamConfiguration
   * @see StreamWriter::setControllerLogTool
   * @see ChannelWriter::setControllerLogTool
   * @see ChapterCWriter::setControllerLogTool
   */
  void setControllerLogTool ( unsigned short controller, unsigned short tool, short channel = -1 );

protected :

  /** The StreamWriter object using this RecoveryJournalWriter object. */
  StreamWriter * _streamWriter;

  /**
   * The system chapter writers for the recovery journal.
   *
   * This vector holds the sequence of SystemChapterWriter objects that will
   * be used to write the chapters of the recovery journal section.
   */
  vector < SystemChapterWriter * > _systemChapters;

  /** The channel writers for the recovery journal. */
  ChannelWriter * _channels[CHANNELS];

  /** S bit */
  short _sBit;

  /** S bit for the journal system */
  short _systemSBit;

};

#endif // RECOVERY_JOURNAL_WRITER_H
