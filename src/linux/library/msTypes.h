/*

  Copyright © Grame 1999

  This library is free software; you can redistribute it and modify it under 
  the terms of the GNU Library General Public License as published by the 
  Free Software Foundation version 2 of the License, or any later version.

  This library is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public License 
  for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

  Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
  grame@rd.grame.fr
  
  modifications history:
   [08-09-99] DF - struct cell added

*/

#ifndef __msTypes__
#define __msTypes__

#include <sys/types.h>

# define NEAR
# define FAR

#define MSFunctionType(type)   type
#define FarPtr(type)           type FAR *

typedef unsigned char uchar;

typedef unsigned char Byte;
typedef char * Ptr;
typedef unsigned char Boolean;
enum { false = 0, true };


typedef struct cell {
	struct cell* link;		/*+ next cell in the list +*/
						    /*+ any data here		  +*/
} cell;


#endif
