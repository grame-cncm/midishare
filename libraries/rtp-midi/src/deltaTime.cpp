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

#include "deltaTime.h"

unsigned short encodeDeltaTime ( const uint32 decodedDeltaTime, uint8 * buffer )
{

  unsigned short n = 0;

  // if XXXXXXXX XXX00000 00000000 00000000 > 0
  if ( ( decodedDeltaTime & 0xFFE00000 ) > 0 ) {
    //  0000aaaa aaa00000 00000000 00000000 - write the byte 1aaaaaaa
    buffer[n] = 0x80 | ( ( decodedDeltaTime >> 21 ) & 0x0000007F );
    n++;
  }

  // if XXXXXXXX XXXXXXXX XX000000 00000000 > 0
  if ( ( decodedDeltaTime & 0xFFFFC000 ) > 0 ) {
    //  00000000 000bbbbb bb000000 00000000 - write the byte 1bbbbbbb
    buffer[n] = 0x80 | ( ( decodedDeltaTime >> 14 ) & 0x0000007F );
    n++;
  }

  // if XXXXXXXX XXXXXXXX XXXXXXXX X0000000 > 0
  if ( ( decodedDeltaTime & 0xFFFFFF80 ) > 0 ) {
    //  00000000 00000000 00cccccc c0000000 - write the byte 1ccccccc
    buffer[n] = 0x80 | ( ( decodedDeltaTime >> 7 ) & 0x0000007F );
    n++;
  }

  // if XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX > 0
  if ( decodedDeltaTime >= 0 ) {
    //  00000000 00000000 00000000 0aaaaaaa - write the byte 1ccccccc
    buffer[n] = decodedDeltaTime & 0x0000007F;
    n++;
  }
  else {
    buffer[0] = 0x00;
  }

  return n;

}

unsigned short decodeDeltaTime ( const uint8 * encodedDeltaTime, uint32 * decodedDeltaTime )
{

  unsigned short n = 0;
  ( * decodedDeltaTime ) = 0;

  do {
    // add the last 7 bits of buffer[n] to decoded
    ( * decodedDeltaTime ) = ( * decodedDeltaTime ) | ( encodedDeltaTime[n] & 0x7F );
    // if the first bit of the buffer[n] is 1
    if ( ( encodedDeltaTime[n] & 0x80 ) > 0 ) {
      // we will consider buffer[n+1]
      n++;
      // left shift of seven bits
      ( * decodedDeltaTime ) = ( ( * decodedDeltaTime ) << 7 );
      // put zeros on the last 7 bits ( is this needed ? )
      ( * decodedDeltaTime ) = ( * decodedDeltaTime ) & 0xFFFFFF80; 
    }
    else { // else stop decoding
      return n + 1;
    }
  }
  while ( n < 4 );

  // 4 first bytes starting with 1 : not a regular delta time
  return 0;

}

unsigned short encodedDeltaTimeSize ( const uint32 decodedDeltaTime )
{

  if ( decodedDeltaTime < 0x80 ) {
    return 1;
  }
  else if ( decodedDeltaTime < 0x4000 ) {
    return 2;
  }
  else if ( decodedDeltaTime < 0x200000 ) {
    return 3;
  }
  else if ( decodedDeltaTime < 0x10000000 ) {
    return 4;
  }
  else {
    return 0;
  }

}

int canEncodeDeltaTime ( const uint32 decodedDeltaTime )
{

  if ( decodedDeltaTime < 0x10000000 ) {
    return true;
  }
  else {
    return false;
  }
  
}
