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

#ifndef CHANNEL_WRITER_H
#define CHANNEL_WRITER_H

#include "Channel.h"

#define CHANNEL_CHAPTERS   8

class RecoveryJournalWriter;
class ChannelChapterWriter;

/**
 * A channel journal writer.
 *
 * A ChannelWriter is owned by a RecoveryJournalWriter and will be
 * used to write the different channel chapters of a channel journal
 * section.
 */

class ChannelWriter : public Channel
{

public :

  /**
   * Constructor.
   */
  ChannelWriter ( RecoveryJournalWriter * recoveryJournalWriter, unsigned short channelNumber );

  /**
   * Calculate the channel journal.
   *
   * This will calculate the channel journal section for a new
   * payload.
   *
   * @return The size in bytes of the calculated recovery journal
   * section.
   */
  unsigned int calculateChannel ( );

  /**
   * Writes the channel journal section to a buffer.
   *
   * @param buffer The buffer in which the last channel journal
   * section calculated by ChannelWriter::calculateChannel will be written to.
   *
   * @return The size in bytes of the channel journal section.
   */
  int writeChannel ( uint8 * buffer );

  /** Unset the channel journal and journal S bit. */
  void unsetSBit ( );

  /** The RecoveryJournalWriter object using this ChannelReader. */
  RecoveryJournalWriter * recoveryJournalWriter ( );

  /**
   * The channel chapter writers for the channel journal.
   */
  vector < Chapter * > * channelChapters ( );

  /**
   * Is the S bit set for this channel ?
   */
  virtual bool sBit ( );

  /**
   * @see Channel::notifyResetCommand
   */
  void notifyResetCommand ( const MidiEvPtr command, short resetType );
  
  /**
   * Sets the chapter inclusion rule for a given chapter.
   *
   * @param chapterName The name of the chapter which is concerned.
   * @param c The new inclusion rule for the chapter.
   *
   * @see @ref StreamConfiguration
   * @see StreamWriter::setChapterInclusion
   * @see RecoveryJournalWriter::setChapterInclusion
   */
  void setChapterInclusion ( char chapterName, chapterInclusion c );

  /**
   * Sets the controller log tool for a given controller.
   *
   * @param controller The number of the concerned controller.
   * @param tool The tool or set of tools that must be used to log the
   * controller.
   *
   * @see @ref StreamConfiguration
   * @see StreamWriter::setControllerLogTool
   * @see RecoveryJournalWriter::setControllerLogTool
   * @see ChapterCWriter::setControllerLogTool
   */
  void setControllerLogTool ( unsigned short controller, unsigned short tool );
  
protected :

  /**
   * The channel chapter writers for the channel journal.
   *
   * This vector holds the sequence of ChapterWriter objects that will
   * be used to write the chapters of the recovery journal section.
   */
  vector < ChannelChapterWriter * > _channelChapters;

  /** The RecoveryJournalWriter object using this ChannelReader. */
  RecoveryJournalWriter * _recoveryJournalWriter;

  /** S bit for the channel journal */
  short _sBit;

  /** The bitfield used to filter command types. */
  uint8 _filter[32];

};

#endif // CHANNEL_WRITER_H
