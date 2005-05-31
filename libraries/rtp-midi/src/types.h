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

#ifndef TYPES
#define TYPES

using namespace std;

typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned int   uint32;

typedef enum { RECJ = 0,
	       NONE = 1} journallingMethod;

typedef enum { UNUSED = 0,
	       NEVER = 1,
	       DEFAULT = 2,
	       ANCHOR = 3,
	       ACTIVE = 4,
	       ANCHOR_ACTIVE = 5 } chapterInclusion;

#define VALUE_TOOL  0x01
#define TOGGLE_TOOL 0x02
#define COUNT_TOOL  0x04

/**
 * A structure holding informations about a channel journal.
 */

typedef struct TChannelJournalStats
{
  /** Total channel journal size */
  unsigned short channelJournalSize;
  /** Chatper P size */
  unsigned short chapterPSize;
  /** Chatper C size */
  unsigned short chapterCSize;
  /** Chatper M size */
  unsigned short chapterMSize;
  /** Chatper W size */
  unsigned short chapterWSize;
  /** Chatper N size */
  unsigned short chapterNSize;
  /** Chatper E size */
  unsigned short chapterESize;
  /** Chatper T size */
  unsigned short chapterTSize;
  /** Chatper A size */
  unsigned short chapterASize;
} TChannelJournalStats;

/**
 * A structure holding informations about a payload.
 */

typedef struct TPayloadStats
{
  /** Total size of thje payload */
  unsigned short totalSize;
  /** Command section size */
  unsigned short commandSectionSize;
  /** Number of commands */
  unsigned short numberOfCommands;
  /** Journal section size */
  unsigned short journalSectionSize;
  /** System journal size */
  unsigned short systemJournalSize;
  /** Chatper D size */
  unsigned short chapterDSize;
  /** Chatper S size */
  unsigned short chapterVSize;
  /** Chatper Q size */
  unsigned short chapterQSize;
  /** Chatper F size */
  unsigned short chapterFSize;
  /** Chatper X size */
  unsigned short chapterXSize;
  /** Number of channel journals */
  unsigned short numberOfChannels;
  /** Informations about the channel journals */
  TChannelJournalStats channel[16];
} TPayloadStats;

#endif // TYPES
