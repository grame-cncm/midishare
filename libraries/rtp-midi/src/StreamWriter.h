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

#ifndef STREAM_WRITER_H
#define STREAM_WRITER_H

#include "Stream.h"
#include <MidiShare.h>

class CommandFilter;
class PayloadWriter;
class RecoveryJournalWriter;

/**
 * A client to create RTP MIDI payloads.
 *
 * A StreamWriter allows the client to manage the creation of a
 * succession of payloads according to the RTP MIDI payload format as
 * described in the RTP MIDI standard. The MIDI commands used to
 * create the different payloads of a same StreamWriter are
 * considered as being parts of a unique and coherent MIDI stream.
 *
 * This StreamWriter object offers a unique interface to the
 * services of the library for payload creation. This includes:
 * - incoming MIDI data
 * - payload succession management
 * - stream options
 *
 * The client has to provide the incoming commands as MidiShare
 * events. The date field of the MidiShare event will be considered as
 * the delta time value of the command.
 *
 * The StreamWriter will then be in charge of:
 * - using a PayloadWriter object to create the payloads, one after
 * the other, and thus, in charge of redirecting the MIDI commands
 * from the client to the PayloadWriter object.
 * - using a RecoveryJournalWriter to keep track of the history of the
 * stream and to write the recovery journal section of the payloads.
 * - notifying the incoming commands to its RecoveryJournal.
 */

class StreamWriter : public Stream
{

public :

  StreamWriter ( );

  /**
   * Starts the creation of a new payload.
   *
   * The library will use the given buffer to create a RTP MIDI
   * payload. The created payload will be restricted in size to fit
   * in the given buffer.
   *
   * @param payloadNumber It is a unique identifier for the payload.
   * It will be used to number the created payloads and to store the
   * history of the stream. A good and simple solution is to use the
   * extended sequence number of the RTP packet in which the payload
   * will be sent.
   * @param buffer The buffer to write to.
   * @param size The size of the buffer.
   *
   * @see PayloadWriter::newPayload
   */
  void newPayload ( const uint32 payloadNumber, uint8 * buffer, const unsigned short size );

  /**
   * Adds a MIDI command to the current payload.
   *
   * @param command The MidiShare event corresponding to the command
   * to be added.
   *
   * @note The delta time value of the command in the payload will
   * correspond to the date field of the MidiShare event. Due to the
   * encoding format of delta time values in the payload only the 28
   * last bits of this value will be considered.
   *
   * @return If the command was inserted in the payload it returns the
   * number of bytes inserted (apart from delta time value). If the
   * command was not inserted because the payload was full, it returns
   * 0. It returns -1 if the command was not accepted because the
   * corresponding chapter policy is set to UNUSED (See @ref
   * StreamConfiguration). It returns -2 if the command type is not
   * supported by the library.
   *
   * @see PayloadWriter::putCommand
   * @see CommandFilter::acceptCommand
   */
  short putCommand ( const MidiEvPtr command );

  /**
   * Finalizes the current payload.
   *
   * This will ask for the finalization of the current payload (update
   * the header and write the recovery journal section).
   * Before that step, the payload is not in a state in which it is
   * supposed to be sent.
   *
   * @return Indicates the actual size of the created payload.
   *
   * @see PayloadWriter::finalizePayload
   */
  int finalizePayload ( );

  /**
   * Changes the checkpoint of the history.
   *
   * @see RecoveryJournal::changeCheckpoint
   */
  void changeCheckpoint ( unsigned int checkpoint );

  /**
   * Sets the journalling method for the stream.
   *
   * Two possible values are :
   * - RECJ : all the payloads will contain a journal section that uses
   * the recovery journal format described in the standard
   * - NONE : all the payloads won't contain any journal section
   *
   * This parameter correspond to the j_sec parameter described in [1]
   * Appendix C.1.1.
   * @see RTP MIDI Appendix C.1.1
   * @see @ref StreamConfiguration
   */
  void setJournallingMethod ( journallingMethod j );

  /**
   * Sets the inclusion for a chapter.
   *
   * The possible values correspond to the parameters defined in [1]
   * Appendix C.1.3 :
   * - UNUSED : the chapter will not appear in the recovery journal
   * section and the commands coded by this chapter won't be accepted
   * any more on the stream.
   * - NEVER : the chapter will never appear in the recovery journal
   * section but the corresponding commands can still appear in the
   * MIDI command section.
   * - DEFAULT : the chapter follows the default semantics defined by
   * the standard.
   * - ANCHOR : the purge of the history up to the checkpoint packet
   * is disabled.
   * - ACTIVE : some special reset commands loose their semantics
   *   - commands with All Notes Off semantics (Control Change
   * 123-127) and All Sound Off command (Control Change 120)
   *   - Reset All Controllers command (Control Change 121)
   *
   * @param chapterName The chapter name.
   * @param c The new inclusion rule for the chapter.
   * @param channel On which channel should this apply. If no channel
   * is provided this will take effect on the corresponding chapters
   * of all channels. This is ignored in case of a system chapter.
   *
   * For more detail @see RTP MIDI Appendix C.1.3
   *
   * @see RecoveryJournalWriter::setChapterInclusion
   */
  void setChapterInclusion ( char chapterName, chapterInclusion c, short channel = -1 );

  /**
   * Changes the log tools for a controller.
   *
   * @param controller The controller number.
   * @param tool Which tools should be used. This can be a combinaison
   * of VALUE_TOOL, TOGGLE_TOOL and COUNT_TOOL.
   * @param channel On which channel should this apply. If no channel
   * is provided this will take effect on the corresponding chapters
   * of all channels. This is ignored in case of a system chapter.
   *
   * @see @ref StreamConfiguration for the default tools for
   * each controller.
   *
   * @see RecoveryJournalWriter::setControllerLogTool
   */
  void setControllerLogTool ( unsigned short controller, unsigned short tool, short channel = -1 );

  /**
   * Prints some historical informations about a chapter.
   *
   * @param chapter The name of the chapter.
   * @param channel On which channel should this apply. If no channel
   * is provided this will take effect on the corresponding chapters
   * of all channels. This is ignored in case of a system chapter.
   *
   * @see RecoveryJournal::printChapterHistory
   */
  void printChapterHistory ( char chapter, short channel = -1 );

protected :

  /**
   * Returns the recovery journal writer of this stream.
   */
  RecoveryJournalWriter * recoveryJournalWriter ( ) const;

  /** The filter for the incoming commands. */
  CommandFilter * _commandFilter;

  /** PayloadWriter of the stream */
  PayloadWriter * _payloadWriter;

  /** RecoveryJournalReader of the stream */
  RecoveryJournalWriter * _recoveryJournalWriter;

  /** The payload buffer to write to */
  uint8 * _payloadBuffer;

  /** The payload buffer size */
  unsigned short _payloadBufferSize;

  /** The current journalling method */
  journallingMethod _journallingMethod;

};

#endif // STREAM_WRITER_H
