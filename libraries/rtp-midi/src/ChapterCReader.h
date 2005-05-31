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

#ifndef CHAPTER_C_READER_H
#define CHAPTER_C_READER_H

#include "ChannelChapterReader.h"

#include <list>

/**
 * A reader for channel chapter C.
 */

class ChapterCReader : public ChannelChapterReader
{

public :

  virtual ~ ChapterCReader ( ) { }

  /**
   * Constructor.
   */ 
  ChapterCReader ( ChannelReader * channelReader );

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
   * Read chapter.

@verbatim
for all CONTROLLER_LOG :
	for all ctrlInfo in _history :
		if ctrlInfo.number = NUMBER :
			if A && T && ctrlInfo.toggle != ALT :
				if ctrlInfo.toggle > ALT :
					ALT += 64
				while ctrlInfo.toggle < ALT :
					if isOn ( ctrlInfo.value ) :
						addRecoveryCommand ( CtrlChange ( NUMBER, off) )
					if isOff ( ctrlInfo.value ) :
						addRecoveryCommand ( CtrlChange ( NUMBER, on ) )

			if !A && ctrlInfo.value != VALUE :
				addRecoveryCommand ( CtrlChange ( NUMBER, VALUE ) )

			if A && !T && ctrlInfo.count != ALT :
				if ctrlInfo.count > ALT :
					ALT += 64
				while ctrlInfo.count < ALT :
					addRecoveryCommand ( CtrlChange ( NUMBER, default ) )
			break
	if _history.end :
		if A && T :
			ctrl = off
			repeat ALT times :
			if ctrl == on :
				addRecoveryCommand ( CtrlChange ( NUMBER, off) )
				ctrl = off
			if ctrl == off :
				addRecoveryCommand ( CtrlChange ( NUMBER, on ) )
				ctrl = on
		if !A :
			addRecoveryCommand ( CtrlChange ( NUMBER, VALUE ) )
		if A && !T :
			repeat ALT times :
				addRecoveryCommand ( CtrlChange ( NUMBER, default ) )
@endverbatim

   */
  int readChapter ( uint8 * buffer );

  /**
   * Print the historical informations kept by the chapter.
   */
  void printHistory ( );

protected :

  /**
   * Returns the default value for the controller @a controller.
   */
  short defaultValue ( short controller );

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

};

#endif // CHAPTER_C_READER_H
