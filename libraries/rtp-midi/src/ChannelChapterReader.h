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

#ifndef CHANNEL_CHAPTER_READER_H
#define CHANNEL_CHAPTER_READER_H

#include "types.h"
#include "ChapterReader.h"
#include "ChannelReader.h"
#include "RecoveryJournalReader.h"
#include "StreamReader.h"

class ChannelReader;

/**
 * A different implementation of this interface
 * will be used by a ChannelReader to read each chapter of a channel
 * journal section and to produce the corresponding recovery commands.
 */

class ChannelChapterReader : public ChapterReader
{

public :
  
  virtual ~ ChannelChapterReader ( ) { }

  /** The current payload number of the stream */
  uint32 currentPayloadNumber ( ) {
    return _channelReader->recoveryJournalReader ( )->streamReader ( )->currentPayloadNumber ( );
  }
  
  /** The recovery journal owning the chapter. */
  RecoveryJournal * recoveryJournal ( ) {
    return _channelReader->recoveryJournalReader ( );
  }

protected :

  /**
   * Constructor.
   */
  ChannelChapterReader ( ChannelReader * channelReader )
    : _channelReader ( channelReader ) { }

  void addRecoveryCommand ( MidiEvPtr command );

  /** The ChannelReader using this ChannelChapterReader object */
  ChannelReader * _channelReader;

};

#endif // CHANNEL_CHAPTER_READER_H
