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

#ifndef COMMAND_FILTER
#define COMMAND_FILTER

#include <MidiShare.h>
#include "types.h"

/**
 * A filter for MIDI commands.
 *
 * A CommandFilter must be used by a StreamWriter object to filter the
 * commands that will appear on the stream. The rejected commands can
 * be commands of an unsupported type or commands for which the
 * corresponding chapter inclusion rule is set to UNUSED.
 */

class CommandFilter
{

public :

  CommandFilter ( );
 
  /**
   * Can the command appear on the stream ?
   *
   * @return This returns 1 if the command can appear on the stream,
   * -2 if the command is of an unsupported type and -1 if the policy
   * of the chapter encoding this command is set to UNUSED.
   */
  short acceptCommand ( MidiEvPtr command );

  /**
   * Forbids some commands.
   *
   * The commands encoded by the chapter @a chapterName will now be
   * rejected by the filter.
   */
  void setChapterUnused ( char chapterName, short channel = -1 );

  /**
   * Allows some commands.
   *
   * The commands encoded by the chapter @a chapterName will now be
   * allowed by the filter.
   */
  void setChapterUsed ( char chapterName, short channel = -1 );

protected :

  /** The bitfield used to filter system commands. */
  uint8 _systemFilter[32];

  /** The bitfield array used to filter channel commands. */
  uint8 _channelFilter[16][32];

};

#endif // COMMAND_FILTER
