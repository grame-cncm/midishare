/*

  Copyright (C) 2005  Grame
  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

  This file is provided as an example of the rtp-midi Library use.
  
*/

#include "Chapter_Writer.h"

#include "[Channel|System]Writer.h"
#include <MidiShare.h>

#include <iostream>

Chapter_Writer::Chapter_Writer ( [Channel|RecoveryJournal]Writer * [channel|recoveryJournal]Writer )
  : [Channel|System]ChapterWriter ( [channel|system]Writer )
{

  _name = '_';
  _[channel|recoveryJournal]Writer->registerForCommandNotification ( this, type__ );

}

void Chapter_Writer::notifyCommand ( MidiEvPtr command )
{

  /* CommandType */
  if ( EvType ( command ) == type__ ) {

  }

}

void Chapter_Writer::notifyResetCommand ( MidiEvPtr command, short resetType )
{

  /* CommandType */
  if ( resetType == RESET___ ) {

  }

}

unsigned int Chapter_Writer::calculateChapter ( )
{

  return _chapterLength;

}

void Chapter_Writer::printHistory ( )
{

  cout << "Chapter _ history" << endl;

}
