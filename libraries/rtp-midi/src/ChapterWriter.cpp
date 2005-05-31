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

#include "ChapterWriter.h"
#include <string.h>
#include "RecoveryJournalWriter.h"

#include <iostream>

bool ChapterWriter::writeChapter ( uint8 * buffer )
{

  if ( _chapterInclusion != UNUSED && _chapterInclusion != NEVER ) {
    memcpy ( buffer, chapter ( ), chapterLength ( ) );
    return true;
  }
  return false;

}

uint32 ChapterWriter::checkpoint ( )
{

  if ( _chapterInclusion == ANCHOR || _chapterInclusion == ANCHOR_ACTIVE ) {
    return 0;
  }
  else {
    return Chapter::checkpoint ( );
  }

}

void ChapterWriter::notifyResetCommand ( MidiEvPtr command, short resetType )
{

  if ( _chapterInclusion == ACTIVE || _chapterInclusion == ANCHOR_ACTIVE ) {
    if ( resetType == RESET_STATE ) {
      this->notifyResetCommand ( command, resetType );
    }
    else {
      this->notifyResetCommand ( command, 0 );
    }
  }
  else {
    this->notifyResetCommand ( command, resetType );
  }

}
