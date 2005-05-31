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

#ifndef PRINTING_UTILS
#define PRINTING_UTILS

#include "types.h"
#include <MidiShare.h>

using namespace std;
#include <iostream>

void printByte ( uint8 byte, ostream & out = cout );

void printHexa ( uint8 byte, ostream & out = cout );

void printBuffer ( uint8 * buffer, unsigned int size, ostream & out = cout );

void printCommand ( MidiEvPtr command, string append = "", ostream & out = cout );

void printFlag ( uint8 byte, ostream & out = cout );

MidiEvPtr readCommand ( istream & in = cin );

void printPayload ( uint8 * buffer, TPayloadStats * stats = NULL, ostream & out = cout );

unsigned short printJournal ( uint8 * buffer, TPayloadStats * stats = NULL, ostream & out = cout );

unsigned short printSystemJournal ( uint8 * buffer, TPayloadStats * stats = NULL, ostream & out = cout );

unsigned short printChannelJournal ( uint8 * buffer, TPayloadStats * stats = NULL, ostream & out = cout );

unsigned short printChapterP ( uint8 * buffer, TPayloadStats * stats = NULL, ostream & out = cout );
unsigned short printChapterC ( uint8 * buffer, TPayloadStats * stats = NULL, ostream & out = cout );
unsigned short printChapterW ( uint8 * buffer, TPayloadStats * stats = NULL, ostream & out = cout );
unsigned short printChapterN ( uint8 * buffer, TPayloadStats * stats = NULL, ostream & out = cout );
unsigned short printChapterT ( uint8 * buffer, TPayloadStats * stats = NULL, ostream & out = cout );
unsigned short printChapterA ( uint8 * buffer, TPayloadStats * stats = NULL, ostream & out = cout );

char * chapterTable ( uint8 * buffer, short journal );

void getPayloadStats ( uint8 * buffer, TPayloadStats * stats );

void printStats ( TPayloadStats * stats, ostream & out = cout );

#endif // PRINTING_UTILS
