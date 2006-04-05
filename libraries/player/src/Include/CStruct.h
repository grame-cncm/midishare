/*

  Copyright © Grame 1996-2006

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
  research@grame.fr

*/


// ===========================================================================
//	CStruct.h			    
// ===========================================================================

/*!
\brief	Definition of C structures used for the external API.
*/

#ifndef __CStruct__
#define __CStruct__

#include "PlayerDefs.h"
#include "MidiShare.h"

/*--------------------------------------------------------------------------*/

typedef struct PlayerState  FAR * PlayerStatePtr;
typedef struct PlayerState{
	long date;
	long tempo;
	short tsnum;
	short tsdenom;
	short tsclick;
	short tsquarter;
	short bar;
	short beat;
	short unit;
	short state;
	short syncin;
	short syncout;
        long ticks;
}PlayerState;

/*--------------------------------------------------------------------------*/

typedef struct Pos  FAR * PosPtr;
typedef struct Pos{
	short bar;
	short beat;
	short unit;
}Pos;

#endif
