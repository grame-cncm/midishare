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

#ifndef PAYLOAD_WRITER_H
#define PAYLOAD_WRITER_H

#include "types.h"
#include <MidiShare.h>

class StreamWriter;

/**
 * Writes RTP MIDI payloads.
 *
 * A PayloadWriter is used to write the MIDI command section of RTP MIDI
 * payloads in given buffers. It will be used by a StreamWriter
 * object which will redirect the incoming MIDI commands to this
 * object. The same PayloadWriter can be used for all the payloads of
 * a stream.
 *
 * It will be in charge of writing the content of the MIDI command
 * section of the payload according to the incoming commands.
 */

class PayloadWriter
{

public :

  /**
   * Constructor.
   */
  PayloadWriter ( StreamWriter * streamWriter )
    : _streamWriter ( streamWriter ), _emptyPayload ( true ) { }

  /**
   * Starts the creation of a new payload.
   *
   * @see StreamWriter::newPayload
   */
  void newPayload ( uint8 * buffer, const unsigned short size );

  /**
   * Adds a MIDI command to the current payload.
   *
   * @note In case of Ctrl14b, RegParam and NonRegParam, this will
   * create the corresponding CtrlChange commands and call
   * StreamWriter::putCommand for them.
   *
   * @result It returns the number of command bytes that were inserted
   * in the payload (apart from the delta time value).
   *
   * @see StreamWriter::putCommand
   */
  short putCommand ( const MidiEvPtr command );

  /**
   * Finalizes the current payload.
   *
   * @see StreamWriter::finalizePayload
   */
  int finalizePayload ( );

protected :

  /**
   * Sets the LEN field of the buffer
   *
   * @param length LEN value.
   */
  void setLEN ( unsigned short length );

  /**
   * The number of bytes left in the buffer.
   */
  unsigned short spaceLeft ( );

  /** Starts reading a SysEx for segmentation. */
  void readSysEx ( MidiEvPtr sysEx );
  /** Get the next field of the SysEx being segmented.
   * This call provides an optimized method to read the content of a
   * MidiShare typeSysEx event.
   */
  short getNextSysExField ( );

  /** The StreamWriter using this PayloadWriter */
  StreamWriter * _streamWriter;

  /** Payload buffer to use */
  uint8 * _buffer;

  /** End of the payload buffer */
  uint8 * _endOfBuffer;

  /** Current writing position in the buffer */
  uint8 * _position;

  /** Is the payload empty ?
   *
   * Is @c TRUE while no command has been written to the payload.
   *
   * This is used to set the Z flag of the MIDI command section
   * header.
   */
  bool _emptyPayload;

  /* for SysEx segmentation */

  /** SysEx being segmented */
  MidiEvPtr _segmentedSysEx;
  /** Current position in the SysEx */
  long _sysExPosition;
  /** Current SysEx cell */
  MidiSEXPtr _cacheCell;

  enum { cellSize = 12 };

};

#endif // PAYLOAD_WRITER_H
