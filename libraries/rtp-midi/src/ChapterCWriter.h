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

#ifndef CHAPTER_C_WRITER
#define CHAPTER_C_WRITER

#include "ChannelChapterWriter.h"

#include <list>

/**
 * A writer for channel chapter C.
 *
 * By default, the value tool is used for the logs. Except for
 * the controllers 64 to 69 (pedals) where the toggle tool with be used by
 * default and for the controllers 96 (Data increment), 97 (Data
 * decrement), 120 (All Sound Off), 121 (Reset All Controllers), 123
 * (All Notes Off), 124 (Omni Mode Off), 125 (Omni Mode On) and 127
 * (Poly Mode On).
 * @see @ref ControllerLogTools
 */

class ChapterCWriter : public ChannelChapterWriter
{

public :

  virtual ~ ChapterCWriter ( ) { }

  /**
   * Constructor.
   */
  ChapterCWriter ( ChannelWriter * channelWriter );

  /**
   * Notifications.
   *
   * @li CtrlChange

@verbatim
for all ctrlInfo in _history :
	if ctrlInfo.number = CtrlChange.Number :
		if ( ctrlInfo.value == on && CtrlChange.Value == off ||
		     ctrlInfo.value == off && CtrlChangeValue == on ) :
			ctrlInfo.toggle = ( ctrlInfo.toggle + 1 ) % 64
		ctrlInfo.value = CtrlChange.Value
		ctrlInfo.count = ( ctrlInfo.count + 1 ) % 64
		ctrlInfo.payload = currentPayloadNumber
		_history.move ( ctrlInfo, end )
		break
if _history.end :
	ctrlInfo.number = CtrlChange.Number
	ctrlInfo.value = CtrlChange.Value
	ctrlInfo.toggle = isOn ( CtrlChange.Value )
	ctrlInfo.count = 1
	ctrlInfo.payload = currentPayloadNumber
	_history.add ( noteInfo )
@endverbatim

   */
  void notifyCommand ( MidiEvPtr command );

  /**
   * Reset notifications.
   *
   * @li Reset State

@verbatim
_history.clear
@endverbatim

   * @li Reset C-Active

@verbatim
for all ctrlInfo in _history :
	if ctrlInfo.number < 120 :
		_history.erase ( ctrlInfo )
@endverbatim

   */
  void notifyResetCommand ( MidiEvPtr command, short resetType );

  /**
   * Calculate chapter.

@verbatim
add 1 octet header
len = 0
for all ctrlInfo in _history :
	if ctrlInfo.payload >= checkpoint :
		if ( tool[ctrlInfo.number] & TOGGLE )
			len++
			add the corresponding log with toggle tool
			if ctrlInfo.payload == currentPayloadNumber - 1 : set S bit
		if ( tool[ctrlInfo.number] & VALUE )
			len++
			add the corresponding log with value tool
			if ctrlInfo.payload == currentPayloadNumber - 1 : set S bit
		if ( tool[ctrlInfo.number] & COUNT )
			len++
			add the corresponding log with count tool
			if ctrlInfo.payload == currentPayloadNumber - 1 : set S bit

LEN = len - 1
if len != 0 : _chapterLength = 1 + len * 2
else : _chapterLength = 0
@endverbatim

  */
  void calculateChapter ( );

  uint8 * chapter ( );

  unsigned int chapterLength ( );

  /**
   * Print the historical informations kept by the chapter.
   */
  void printHistory ( );

  /**
   * Sets the controller log tool for a given controller and channel.
   *
   * @param controller The number of the concerned controller.
   * @param tool The tool or set of tools that must be used to log the
   * controller.
   *
   * @see @ref StreamConfiguration
   * @see StreamWriter::setControllerLogTool
   * @see RecoveryJournalWriter::setControllerLogTool
   * @see ChannelWriter::setControllerLogTool
   */
  void setControllerLogTool ( unsigned short controller, unsigned short tool );

protected :

  /**
   * Returns the default tool for the controller @a controller.
   */
  short defaultTool ( short controller );

  /** The buffer storing the last calculated chapter. */
  uint8 * _chapter;

 /** The length of the last calculated chapter. */
  unsigned int _chapterLength;

  /**
   * A structure to keep the historical information about a
   * controller number.
   */
  typedef struct TCtrlInfo
  {
    /** Controller number. */
    unsigned short number;
    /** Last value of the controller. */
    unsigned short value;
    /** Toggle value for the controller. */
    unsigned short toggle;
    /** Count value for the controller. */
    unsigned short count;
    /** Last payload in which this controller appears. */
    uint32 payload;
  } TCtrlInfo;
 
  /** Historical data for the chapter. */
  list < TCtrlInfo > _history;

  /** Array to store the tools used for each controller. */
  unsigned short _tool[128];

};

#endif // CHAPTER_C_WRITER
