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

#ifndef DELTA_TIME_H
#define DELTA_TIME_H

#include "types.h"

/**
 * Delta time encoding and decoding routines.
 */

/**
 * Encode a delta time.
 *
 * The decoded delta time provided will be encoded
 * and written in the buffer according to the format specified in
 * [1] Section 3.1.
 *
 * Only 28 bits decoded delta times can be fully encoded, the 4
 * first bits of the decoded delta time will be ignored during
 * encoding. But if they were not all zeros, the return value will
 * be false as a warning. Else it will be true.
 */
unsigned short encodeDeltaTime ( const uint32 decodedDeltaTime, uint8 * buffer );

/**
 * Decode a delta time.
 *
 * A delta time will be decoded from the buffer and stored at the in
 * the decodedDeltaTime argument.
 *
 * The return value will indicate the number of byte read to decode
 * the delta time.
 *
 * If the 4th first bytes of the buffer are all starting with 1, no
 * delta time value can be decoded and the call will return 0.
 */
unsigned short decodeDeltaTime ( const uint8 * encodedDeltaTime, uint32 * decodedDeltaTime );

/**
 * Tell the size in bytes of the encoded form of this delta time.
 *
 * If the decoded delta time value is bigger than 0x0FFFFFFF we cannot
 * encode it with 4 bytes and the return value will be 0.
 */
unsigned short encodedDeltaTimeSize ( const uint32 decodedDeltaTime );

/**
 * Tell if it's possible to encode this delta time value.
 *
 * Delta time values bigger than 0x0FFFFFFF cannot be encoded using
 * the coding technique specified in [1] Section 3.1.
 */
int canEncodeDeltaTime ( const uint32 decodedDeltaTime );

#endif // DELTA_TIME_H
