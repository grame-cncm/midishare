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

#ifndef RECOVERY_JOURNAL_H
#define RECOVERY_JOURNAL_H

using namespace std;
#include <MidiShare.h>
#include <vector>
#include "types.h"

class Stream;
class Chapter;
class Channel;

/** The number of channels to create */
#define CHANNELS 16

#define SYSTEM_CHAPTERS   6

#define RESET_STATE 0x01
#define RESET_N_ACTIVE 0x02
#define RESET_C_ACTIVE 0x04

/**
 * A recovery journal.
 *
 * This is a common interface to both RecoveryJournalWriter and
 * RecoveryJournalReader.
 *
 * A RecoveryJournal is notified with all the commands happening in
 * the stream (incoming commands in case of a StreamWriter and a
 * RecoveryJournalWriter or outgoing commands in case of a
 * StreamReader and a RecoveryJournalReader).
 *
 * It will, in turn, in case of a system command, notify the Chapter
 * objects (corresponding to its system chapters) that registered with
 * him for the corresponding type of command. It will also notify its
 * Channel objects.
 */

class RecoveryJournal
{

public :

  /**
   * Constructor
   */
  RecoveryJournal ( ) : _checkpoint ( 0 ) { };

  virtual ~ RecoveryJournal ( ) { };

  /**
   * The current checkpoint payload number.
   *
   * @see Chapter::checkpoint
   * @see ChapterWriter::checkpoint
   */
  virtual uint32 checkpoint ( );

  /**
   * Changes the checkpoint to @a checkpoint.
   *
   * @see StreamWriter::changeCheckpoint
   * @see StreamReader::changeCheckpoint
   */
  void changeCheckpoint ( unsigned int checkpoint );

  /**
   * Notifies a command.
   *
   * This will call in turn Chapter::notifyCommand
   * on the system chapters that registered with the
   * RecoveryJournal for the corresponding type of command as well as
   * Channel::notifyCommand on all its channels.
   *
   * In case of the reset command, this will also call
   * RecoveryJournal::notifyResetCommand.
   * The commands that we consider as reset commands are the
   * following commands :
   * - Reset State commands :
   *   - System Reset : 0xFF
   *   - General MIDI System Enable : 0xF0 0x7E 0xCC 0x09 0x01 0xF7
   *   - General MIDI 2 System Enable : 0xF0 0x7E 0xCC 0x09 0x03 0xF7
   *   - General MIDI System Disable : 0xF0 0x7E 0xCC 0x09 0x00 0xF7
   *   - Turn DLS On : 0xF0 0x7E 0xCC 0x0A 0x01 0xF7
   *   - Turn DLS Off : 0xF0 0x7E 0xCC 0x0A 0x02 0xF7
   * - N-Reset commands :
   *   - Control Change n°123, 124, 125, 126, 127
   *   - Control Change n°120
   * - C-Reset commands :
   *   - Control Change n°121
   *
   * @param command The command to notify.
   *
   * @see registerForCommandNotification
   * @see notifyResetCommand
   * @see Chapter::notifyCommand
   * @see Channel::notifyCommand
   */
  void notifyCommand ( const MidiEvPtr command );

  /**
   * Notifies a reset command.
   *
   * This will call in turn
   * Chapter::notifyResetCommand on the system
   * chapters of this recovery journal and
   * Channel::notifyResetCommand on all its
   * channels.
   *
   * @param command The command to notify.
   * @param resetType The type of the reset command.
   *
   * @see Channel::notifyResetCommand
   * @see Chapter::notifyResetCommand
   * @see ChannelWriter::notifyResetCommand
   * @see ChapterWriter::notifyResetCommand
   */
  virtual void notifyResetCommand ( const MidiEvPtr command, short resetType );

  /**
   * Register a Chapter to be notified.
   *
   * A Chapter object can register with a RecoveryJournal for a
   * given type of command. After that, when the RecoveryJournal will
   * be notified of a command which corresponds to this type, it will
   * call Chapter::notifyCommand on this Chapter object.
   *
   * @param chapter The Chapter to be notified.
   * @param commandType The type of command for which the chapter
   * wants to be notified.
   *
   * @see RecoveryJournal::notifyCommand
   * @see Chapter::notifyCommand
   */
  void registerForCommandNotification ( Chapter * chapter, unsigned short commandType );

  /**
   * Prints some historical informations about a chapter.
   *
   * @see StreamWriter::printChapterHistory
   * @see StreamReader::printChapterHistory
   */
  void printChapterHistory ( char chapter, short channel = -1 );

  /**
   * An array containing the name of the system chapters in their
   * order of appearance in the journal.
   */
  static char systemChapterName[SYSTEM_CHAPTERS];

protected :

  /** The system chapters of the recovery journal */
  virtual vector < Chapter * > * systemChapters ( ) = 0;

  /** The channels of the recovery journal */
  virtual Channel * channel ( unsigned short n ) = 0;

  /** The chapters registered for typeSongPos commands */
  vector < Chapter * > _regSongPos;
  /** The chapters registered for typeSongSel commands */
  vector < Chapter * > _regSongSel;
  /** The chapters registered for typeClock commands */
  vector < Chapter * > _regClock;
  /** The chapters registered for typeStart commands */
  vector < Chapter * > _regStart;
  /** The chapters registered for typeContinue commands */
  vector < Chapter * > _regContinue;
  /** The chapters registered for typeStop commands */
  vector < Chapter * > _regStop;
  /** The chapters registered for typeTune commands */
  vector < Chapter * > _regTune;
  /** The chapters registered for typeStream commands */
  vector < Chapter * > _regStream;

  /** The payload number corresponding to the checkpoint for the recovery journal */
  uint32 _checkpoint;

};

#endif // RECOVERY_JOURNAL_H
