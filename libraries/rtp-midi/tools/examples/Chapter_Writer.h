/*

  Copyright (C) 2005  Grame
  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

  This file is provided as an example of the rtp-midi Library use.
  
*/

#ifndef CHAPTER___WRITER
#define CHAPTER___WRITER

#include "[Channel|System]ChapterWriter.h"

/**
 * A writer for channel chapter _.
 */

class Chapter_Writer : public [Channel|System]ChapterWriter
{

public :

  virtual ~ Chapter_Writer ( ) { }

  /**
   * Constructor.

@verbatim
@endverbatim

   */
  Chapter_Writer ( [Channel|RecoveryJournal]Writer * [channel|recoveryJournal]Writer );

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
   * Calculate chapter.

@verbatim
@endverbatim

  */
  unsigned int calculateChapter ( );

  /**
   * Print the historical informations kept by the chapter.
   */
  void printHistory ( );

protected :

};

#endif // CHAPTER___WRITER
