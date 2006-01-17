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

#ifndef STREAM_READER_H
#define STREAM_READER_H

#include "Stream.h"
#include <MidiShare.h>

class PayloadReader;
class RecoveryJournalReader;

/**
 * A client to analyse RTP MIDI payloads.
 *
 * A StreamReader allows the client to manage the analyse of a
 * succession of payloads according to the RTP MIDI payload format as
 * described in the RTP MIDI draft. The MIDI commands analysed from the
 * different payloads of a same StreamReader are considered as being
 * parts of a unique and coherent MIDI stream.
 *
 * This StreamReader object offers a unique interface to the
 * services of the library for payload analyse. This includes:
 * - outgoing MIDI data
 * - outgoing MIDI recovery data
 * - payload succession management
 *
 * The client will receive the outgoing MIDI data (both usual and
 * recovery data) in two ways as MidiShare events. The date field of
 * the MidiShare event will represent the delta time value of the
 * command. This will be set to 0 in case of recovery commands.
 *
 * Recovery commands might be computed to repear the stream after a
 * packet loss. If the packet is not arriving just after a packet
 * loss there won't be any recovery command.
 * If the client wants to use the recovery commands it should ask
 * for the recovery commands before asking for the usual commands.
 * The recovery commands calculated from a payload might not be
 * available anymore after a call to StreamReader::getCommand for this payload or
 * after the submission of a new payload with the StreamReader::newPayload
 * method.
 *
 * The StreamReader will then be in charge of :
 * - using a PayloadReader object to analyse the payloads,
 * one after the other, and thus, in charge of redirecting the MIDI
 * data coming from the PayloadReader object actually reading the
 * current payload.
 * - using a RecoveryJournalReader object to read the recovery journal
 * section of the payload.
 * - notifying the outgoing commands (both usual and recovery
 * commands) to its RecoveryJournalReader.
 */

class StreamReader : public Stream
{

public :

  StreamReader ( );

  /**
   * Submit a new payload to be analysed.
   *
   * The library will use the payload in the given buffer to extract
   * MIDI commands, delta time values and recovery commands.
   *
   * After calling this, the data included in the previous payload of
   * the stream (in case of usual commands) or which could have
   * been infered from the previous payload (in case of recovery
   * commands)  cannot be retrieved anymore.
   *
   * @param payloadNumber It is a unique identifier of the
   * payload. It will be used to number the incoming payloads and
   * to store the history of the stream. A good and simple solution is
   * the use the extended sequence number of the RTP packet in which
   * this payload was received.
   * @param buffer The buffer to read from.
   * @param size The length of the payload in the buffer.
   *
   * @see PayloadReader::newPayload
   */
  void newPayload ( const uint32 payloadNumber, uint8 * buffer, const unsigned short size );
  
  /**
   * Retrieves the next MIDI command of the current payload.
   *
   * @return A command if there was still a command to be read in the
   * payload, otherwise NULL.
   *
   * @note The delta time value associated to the command will figure
   * in the @a date filed of the MidiShare event.
   *
   * @see PayloadReader::getCommand
   */
  MidiEvPtr getCommand ( );

  /**
   * Retrieves the next recovery command for the current payload.
   *
   * @return The MidiShare event corresponding to the next
   * recovery command for the payload, otherwise NULL if no more
   * recovery command has to be computed for this payload.
   *
   * @see RecoveryJournalReader::getRecoveryCommand
   */
  MidiEvPtr getRecoveryCommand ( );

  /**
   * Changes the checkpoint of the history.
   *
   * @see RecoveryJournal::changeCheckpoint
   */
  void changeCheckpoint ( unsigned int checkpoint );

  /**
   * Returns the recovery journal reader for this stream.
   */
  RecoveryJournalReader * recoveryJournalReader ( ) const;

  /**
   * Prints some historical informations about a chapter.
   *
   * @param chapter The name of the chapter.
   * @param channel On which channel should this apply. If no channel
   * is provided this will take effect on the corresponding chapters
   * of all channels. This is ignored in case of a system chapter.
   */
  void printChapterHistory ( char chapter, short channel = -1 );

protected :

  /** RecoveryJournalReader for the stream */
  RecoveryJournalReader * _recoveryJournalReader;

  /** PayloadReader for the stream */
  PayloadReader * _payloadReader;

  /** The payload buffer to read from */
  uint8 * _payloadBuffer;

};

#endif // STREAM_READER_H
