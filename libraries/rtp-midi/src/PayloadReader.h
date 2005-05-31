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

#ifndef PAYLOAD_READER_H
#define PAYLOAD_READER_H

#include "types.h"
#include <MidiShare.h>

class StreamReader;

/**
 * Reads RTP MIDI payloads.
 *
 * A PayloadReader is used to read the MIDI command section of RTP MIDI
 * payloads in given buffers. It will be used by an StreamReader
 * object which will forward the MIDI commands extracted from the
 * payload to the client. The same PayloadReader can be used for all
 * the payloads of a stream.
 *
 * It will be in charge of :
 * - analysing the content of the provided payload.
 * - creating MIDI commands from this content.
 * - indicating to the StreamReader, the location of the recovery
 * journal section in the payload.
 */

class PayloadReader
{

public :

  /**
   * Constructor.
   */
  PayloadReader ( StreamReader * streamReader ) : _streamReader ( streamReader ), _startPayload ( true ) { };

  /**
   * Starts the creation of a new payload.
   *
   * @see StreamReader::newPayload
   */
  void newPayload ( uint8 * buffer, const unsigned short size );

  /**
   * Retrieves the next MIDI command of the payload.
   *
   * @see StreamReader::getCommand
   */
  MidiEvPtr getCommand ( );

  /**
   * The location of the recovery journal section of this payload.
   */
  uint8 * recoveryJournalLocation( );

protected :

  /** The StreamReader using this PayloadReader */
  StreamReader * _streamReader;

  /** Payload buffer to use */
  uint8 * _buffer;

  /** End of the payload buffer */
  uint8 * _endOfMidiList;

  /** Current writing position in the buffer */
  uint8 * _position;

  /** Is the payload empty ?
   *
   * Is @c TRUE while no command has been written to the payload.
   *
   * This is used to read the Z flag of the MIDI command section
   * header.
   */
  bool _startPayload;

};

#endif // PAYLOAD_READER_H
