/*

  Copyright (C) 2005  Grame
  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

  This file is provided as an example of the rtp-midi Library use.
  
*/

#include "Chapter_Reader.h"

#include "[Channel|System]Reader.h"
#include <MidiShare.h>

#include "eventUtils.h"

#include <iostream>

Chapter_Reader::Chapter_Reader ( [Channel|System]Reader * [channel|system]Reader )
  : [Channel|System]ChapterReader ( [channel|system]Reader )
{

  _name = '_';
  _[channel|recoveryJournal]Reader->registerForCommandNotification ( this, type__ );

}

void Chapter_Reader::notifyCommand ( MidiEvPtr command )
{

  /* CommandType */
  if ( EvType ( command ) == type__ ) {

  }

}

void Chapter_Reader::notifyResetCommand ( MidiEvPtr command, short resetType )
{

  /* CommandType */
  if ( resetType == RESET___ ) {

  }

}

int Chapter_Reader::readChapter ( uint8 * buffer )
{

}

void Chapter_Reader::printHistory ( )
{

  cout << "Chapter _ history" << endl;

}
