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
   [13-09-99] DF - 'PascalNames' definition moved to platform dependent header

*/

#ifndef __msTypes__
#define __msTypes__

#ifdef __WIN16__
# define NEAR  near
# define FAR   far
#else
# define NEAR
# define FAR
#endif


#ifdef __Macintosh__
# include "MacTypes.h"
# define MSFunctionType(type)   pascal type
#else
# define MSFunctionType(type)   type
#endif

#define FarPtr(type)           type FAR *

typedef unsigned long ulong;
typedef unsigned char uchar;


typedef struct cell {
	struct cell* link;		/*+ next cell in the list +*/
						    /*+ any data here		  +*/
} cell;


#endif
