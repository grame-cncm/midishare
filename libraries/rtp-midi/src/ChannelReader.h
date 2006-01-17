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

#ifndef CHANNEL_READER_H
#define CHANNEL_READER_H

#include "Channel.h"
#include "types.h"

class RecoveryJournalReader;
class ChannelChapterReader;

/**
 * A channel journal reader.
 *
 * A ChannelReader is owned by a RecoveryJournalReader and will be
 * used to read the different channel chapters of a channel journal
 * section.
 */

class ChannelReader : public Channel
{

public :

  /**
   * Constructor.
   */
  ChannelReader ( RecoveryJournalReader * recoveryJournalReader, unsigned short channelNumber );

  /**
   * Reads the channel journal section from a buffer.
   *
   * @param buffer Buffer to read from.
   */
  int readChannel ( uint8 * buffer );

  /**
   * Retrieves the next recovery command for the channel.
   *
   * @see StreamReader::getRecoveryCommand
   */
  MidiEvPtr getRecoveryCommand ( );

  /** The RecoveryJournalReader object using this ChannelReader. */
  RecoveryJournalReader * recoveryJournalReader ( );

  /** The chapters of the channel. */
  vector < Chapter * > * channelChapters ( );

protected :

  /** The RecoveryJournalReader object using this ChannelReader. */
  RecoveryJournalReader * _recoveryJournalReader;
  
  /** The chapter readers for the channel. */
  vector < ChannelChapterReader * > _channelChapters;

};

#endif // CHANNEL_READER_H
