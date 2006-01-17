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

#include "binaryUtils.h"

#include <iostream>

bool getFlag ( uint8 byte, unsigned short position ) {

  return ( byte & ( 0x80 >> position ) ) >> ( 7 - position );

}

bool getBufferFlag ( uint8 * buffer, unsigned short position ) {

  return getFlag ( buffer[position/8], position%8 );

}

void setFlag ( uint8 * byte, unsigned short position, bool value )
{

  if ( value ) {
    ( * byte ) |= 0x80 >> position;
  }
  else {
    if ( getFlag ( * byte, position ) ) {
      ( * byte ) ^= 0x80 >> position;
    }
  }

}

void setBufferFlag ( uint8 * buffer, unsigned short position, bool value )
{

  setFlag ( & buffer[position/8], position%8, value );

}
