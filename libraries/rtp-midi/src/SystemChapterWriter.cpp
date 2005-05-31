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

#include "SystemChapterWriter.h"
#include "RecoveryJournalWriter.h"

void SystemChapterWriter::setChapterInclusion ( chapterInclusion c )
{

  _chapterInclusion = c;

  /*
  if ( _chapterInclusion == UNUSED ) {
    for ( unsigned int i = 0 ; i < _commandType.size ( ) ; i++ ) {
      ( ( RecoveryJournalWriter * ) recoveryJournal ( ) )->setFilter ( _commandType[i], false );
    }
  }
  else {
    for ( unsigned int i = 0 ; i < _commandType.size ( ) ; i++ ) {
      ( ( RecoveryJournalWriter * ) recoveryJournal ( ) )->setFilter ( _commandType[i], true );
    }
  }
  */

}
