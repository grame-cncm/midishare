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

#ifndef CHANNEL_H
#define CHANNEL_H

using namespace std;
#include <MidiShare.h>
#include <vector>
#include "types.h"

class Chapter;

#define CHANNEL_CHAPTERS   8

/**
 * A recovery journal channel.
 *
 * This is a common interface to both ChannelWriter and
 * ChannelReader.
 *
 * A Channel is notified with all the commands notified to the
 * RecoveryJournal.
 * It will, in turn, notify the Chapter objects (corresponding to its
 * channel chapters) that registered with him for the corresponding
 * type of commands.
 */

class Channel
{

public :

  /**
   * Constructor
   */
  Channel ( unsigned short channelNumber );

  virtual ~ Channel ( ) { }

  /**
   * The channel number of this Channel object.
   */
  unsigned short channelNumber ( );

  /**
   * Notifies a command.
   *
   * This will call in turn
   * Chapter::notifyCommand on the channel chapters
   * that registered with the Channel for the corresponding type of
   * command.
   *
   * @param command The command to notify.
   *
   * @see registerForCommandNotification
   * @see RecoveryJournal::notifyCommand
   * @see Chapter::notifyCommand
   */
  void notifyCommand ( const MidiEvPtr command );

  /**
   * Notifies a reset command.
   *
   * This will call in turn
   * Chapter::notifyResetCommand on each of its
   * channel chapters.
   *
   * @param command The command to notify.
   * @param resetType The type of the reset command.
   *
   * @see RecoveryJournal::notifyResetCommand
   * @see Chapter::notifyResetCommand
   * @see ChannelWriter::notifyResetCommand
   * @see ChapterWriter::notifyResetCommand
   */
  void notifyResetCommand ( const MidiEvPtr command, short resetType );

  /**
   * Register a Chapter to be notified.
   *
   * A Chapter object can register with a Channel for a
   * given type of command. After that, when the Channel will
   * be notified of a command which corresponds to this type, it will
   * call Chapter::notifyCommand on this Chapter object.
   *
   * @param chapter The Chapter to be notified.
   * @param commandType The type of command for which the chapter
   * wants to be notified.
   *
   * @see Channel::notifyCommand
   * @see Chapter::notifyCommand
   */
  void registerForCommandNotification ( Chapter * chapter, unsigned short commandType );

  /**
   * Prints some historical informations about a chapter.
   *
   * @see StreamWriter::printChapterHistory
   * @see Chapter::printHistory
   */
  void printChapterHistory ( char chapter );

  /**
   * An array containing the name of the channel chapters in their
   * order of appearance in the journal.
   */
  static char channelChapterName[CHANNEL_CHAPTERS];

protected :

  /** The channel number of this Channel object */
  unsigned short _channelNumber;

  /** The channel chapters of this channel */
  virtual vector < Chapter * > * channelChapters ( ) = 0;

  /** The chapters registered for typeKeyOff commands */
  vector < Chapter * > _regKeyOff;
  /** The chapters registered for typeKeyOn commands */
  vector < Chapter * > _regKeyOn;
  /** The chapters registered for typeKeyPress commands */
  vector < Chapter * > _regKeyPress;
  /** The chapters registered for typeCtrlChange commands */
  vector < Chapter * > _regCtrlChange;
  /** The chapters registered for typeProgChange commands */
  vector < Chapter * > _regProgChange;
  /** The chapters registered for typeChanPress commands */
  vector < Chapter * > _regChanPress;
  /** The chapters registered for typePitchWheel commands */
  vector < Chapter * > _regPitchWheel;
  /** The chapters registered for typeCtrl14b commands */
  vector < Chapter * > _regCtrl14b;
  /** The chapters registered for typeRegParam commands */
  vector < Chapter * > _regRegParam;
  /** The chapters registered for typeNonRegParam commands */
  vector < Chapter * > _regNonRegParam;

};

#endif // CHANNEL_H
