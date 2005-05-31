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

#ifndef CHAPTER_T_READER_H
#define CHAPTER_T_READER_H

#include "ChannelChapterReader.h"

/**
 * A reader for channel chapter T.
 */

class ChapterTReader : public ChannelChapterReader
{

public :

  virtual ~ ChapterTReader ( ) { }

  /**
   * Constructor.
   */ 
  ChapterTReader ( ChannelReader * channelReader );

  /**
   * Notifications.
   *
   * @li ChanPress

@verbatim
_lastPressure = ChanPress.Pressure
@endverbatim

   */
  void notifyCommand ( MidiEvPtr command );

  /**
   * Reset notifications.
   * @li Reset N-Active

@verbatim
_lastPressure = -1
@endverbatim

   */
  void notifyResetCommand ( MidiEvPtr command, short resetType );

  /**
   * Read chapter.

@verbatim
if ( _lastPressure != PRESSURE )
	addRecoveryCommand ( ChanPress ( PRESSURE ) )
@endverbatim

   */
  int readChapter ( uint8 * buffer );

  /**
   * Print the historical informations kept by the chapter.
   */
  void printHistory ( );

protected :

  /** Pressure of the last ChanPress command */
  short _lastPressure;

};

#endif // CHAPTER_T_READER_H
