/*

  Copyright (C) 2005  Grame
  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

  This file is provided as an example of the rtp-midi Library use.
  
*/

#ifndef CHAPTER___READER_H
#define CHAPTER___READER_H

#include "[Channel|System]ChapterReader.h"

/**
 * A reader for channel chapter _.
 */

class Chapter_Reader : public [Channel|System]ChapterReader
{

public :

  virtual ~ Chapter_Reader ( ) { }

  /**
   * Constructor.

@verbatim
@endverbatim

   */ 
  Chapter_Reader ( [Channel|RecoveryJournal]Reader * [channel|recoveryJournal]Reader );

  /**
   * Notifications.
   *
   * @li CommandType

@verbatim
@endverbatim

   * @li CommandType

@verbatim
@endverbatim

   * @li CommandType

@verbatim
@endverbatim

   */
  void notifyCommand ( MidiEvPtr command );

  /**
   * Reset notifications.
   *
   * @li ResetCommand

@verbatim
@endverbatim

   */
  void notifyResetCommand ( MidiEvPtr command, short resetType );

  /**
   * Read chapter.

@verbatim
@endverbatim

   */
  int readChapter ( uint8 * buffer );

  /**
   * Print the historical informations kept by the chapter.
   */
  void printHistory ( );

protected :

};

#endif // CHAPTER___READER_H
