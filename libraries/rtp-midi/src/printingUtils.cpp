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

#include "printingUtils.h"
#include "eventUtils.h"
#include "binaryUtils.h"
#include "deltaTime.h"
#include <string>
#include <fstream>

void printByte ( uint8 byte, ostream & out )
{

  out << ( ( byte & 0x80 ) >> 7 );
  out << ( ( byte & 0x40 ) >> 6 );
  out << ( ( byte & 0x20 ) >> 5 );
  out << ( ( byte & 0x10 ) >> 4 );
  out << ( ( byte & 0x08 ) >> 3 );
  out << ( ( byte & 0x04 ) >> 2 );
  out << ( ( byte & 0x02 ) >> 1 );
  out << ( ( byte & 0x01 ) >> 0 );

}

void printHexa ( uint8 byte, ostream & out )
{

  out << "0x" << hex << ( short ) byte << dec;

}

void printBuffer ( uint8 * buffer, unsigned int size, ostream & out )
{

  for ( unsigned int i = 0 ; i < size ; i++ ) {
    printHexa( buffer[i], out );
    out << " ";
  }
  out << endl;

}

void printCommand ( MidiEvPtr command, string append, ostream & out )
{

  switch EvType ( command ) {

  case typeKeyOff :
    out << ( long ) Date ( command ) << "\t" << "KeyOff\t" << hex << ( short ) Chan ( command ) << dec;
    out << "\t" << hex << ( short ) Pitch ( command ) << dec;
    out << "\t" << hex << ( short ) Vel ( command ) << dec;
    break;

  case typeKeyOn :
    out << ( long ) Date ( command ) << "\t" << "KeyOn\t" << hex << ( short ) Chan ( command ) << dec;
    out << "\t" << hex << ( short ) Pitch ( command ) << dec;
    out << "\t" << hex << ( short ) Vel ( command ) << dec;
    break;

  case typeKeyPress :
    out << ( long ) Date ( command ) << "\t" << "KeyPress\t" << hex << ( short ) Chan ( command ) << dec;
    out << "\t" << hex << ( short ) Pitch ( command ) << dec ;
    out << "\t" << hex << ( short ) Vel ( command ) << dec;
    break;

  case typeCtrlChange :
    out << ( long ) Date ( command ) << "\t" << "CtrlChange\t" << hex << ( short ) Chan ( command ) << dec;
    out << "\t" << hex << ( short ) Data ( command ) [0] << dec;
    out << "\t" << hex << ( short ) Data ( command ) [1] << dec;
    break;

  case typeProgChange :
    out << ( long ) Date ( command ) << "\t" << "ProgChange\t" << hex << ( short ) Chan ( command ) << dec;
    out << "\t" << hex << ( short ) Data ( command ) [0] << dec;
    break;

  case typeChanPress :
    out << ( long ) Date ( command ) << "\t" << "ChanPress\t" << hex << ( short ) Chan ( command ) << dec;
    out << "\t" << hex << ( short ) Data ( command ) [0] << dec;
    break;

  case typePitchWheel :
    out << ( long ) Date ( command ) << "\t" << "PitchWheel\t" << hex << ( short ) Chan ( command ) << dec;
    out << "\t" << hex << ( short ) Data ( command ) [0] << dec;
    out << "\t" << hex << ( short ) Data ( command ) [1] << dec;
    break;

  case typeSongPos :
    out << ( long ) Date ( command ) << "\t" << "SongPos";
    out << "\t" << hex << ( short ) Data ( command ) [0] << dec;
    out << "\t" << hex << ( short ) Data ( command ) [1] << dec;
    break;

  case typeSongSel :
    out << ( long ) Date ( command ) << "\t" << "SongSel";
    out << "\t" << hex << ( short ) Data ( command ) [0] << dec;
    break;

  case typeClock :
    out << ( long ) Date ( command ) << "\t" << "Clock";
    break;

  case typeStart :
    out << ( long ) Date ( command ) << "\t" << "Start";
    break;

  case typeContinue :
    out << ( long ) Date ( command ) << "\t" << "Continue";
    break;

  case typeStop :
    out << ( long ) Date ( command ) << "\t" << "Stop";
    break;

  case typeTune :
    out << ( long ) Date ( command ) << "\t" << "Tune";
    break;

  case typeActiveSens :
    out << ( long ) Date ( command ) << "\t" << "ActiveSens";
    break;

  case typeReset :
    out << ( long ) Date ( command ) << "\t" << "Reset";
    break;

  case typeStream :
    out << ( long ) Date ( command ) << "\t" << "Stream";
    for ( long i = 0 ; i < MidiCountFields ( command ) ; i++ ) {
      out << "\t" << hex << ( short ) MidiGetField ( command, i ) << dec << " ";
    }
    break;

  case typeSysEx :
    out << ( long ) Date ( command ) << "\t" << "SysEx";
    for ( long i = 0 ; i < MidiCountFields ( command ) ; i++ ) {
      out << "\t" << hex << ( short ) MidiGetField ( command, i ) << dec << " ";
    }
    break;

  case typeCtrl14b :
    out << ( long ) Date ( command ) << "\t" << "Ctrl14b\t" << hex << ( short ) Chan ( command ) << dec;
    out << "\t" << hex << MidiGetField ( command, 0 ) << dec;
    out << "\t" << hex << MidiGetField ( command, 1 ) << dec;
    break;

  case typeRegParam :
    out << ( long ) Date ( command ) << "\t" << "RegParam\t" << hex << ( short ) Chan ( command ) << dec;
    out << "\t" << hex << MidiGetField ( command, 0 ) << dec;
    out << "\t" << hex << MidiGetField ( command, 1 ) << dec;
    break;

  case typeNonRegParam :
    out << ( long ) Date ( command ) << "\t" << "NonRegParam\t" << hex << ( short ) Chan ( command ) << dec;
    out << "\t" << hex << MidiGetField ( command, 0 ) << dec;
    out << "\t" << hex << MidiGetField ( command, 1 ) << dec;
    break;

  default :
    out << ( long ) Date ( command ) << "\t" << ( short ) EvType ( command ) << "\tunsupported command type";
    break;

  }

  out << append << endl;

}

MidiEvPtr readCommand ( istream & in )
{

  MidiEvPtr command = NULL;
  uint32 deltaTime;
  string type;

  if ( ( char ) in.peek ( ) == '\n' ) {
    in.get ( );
    if ( ( char ) in.peek ( ) == '\n' ) {
      return NULL;
    }
  }
  in >> deltaTime;
  in >> type;

  if ( type == "KeyOff" ) {
    unsigned short channel, pitch, velocity;
    in >> hex >> channel >> dec;
    in >> hex >> pitch;
    in >> hex >> velocity;
    command = newKeyOff ( deltaTime, channel, pitch, velocity );
  }

  if ( type == "KeyOn" ) {
    unsigned short channel, pitch, velocity;
    in >> hex >> channel >> dec;
    in >> hex >> pitch >> dec;
    in >> hex >> velocity >> dec;
    command = newKeyOn ( deltaTime, channel, pitch, velocity );
  }
  
  if ( type == "KeyPress" ) {
    unsigned short channel, pitch, velocity;
    in >> hex >> channel >> dec;
    in >> hex >> pitch >> dec;
    in >> hex >> velocity >> dec;
    command = newKeyPress ( deltaTime, channel, pitch, velocity );
  }
  
  if ( type == "CtrlChange" ) {
    unsigned short channel, controller, value;
    in >> hex >> channel >> dec;
    in >> hex >> controller >> dec;
    in >> hex >> value >> dec;
    command = newCtrlChange ( deltaTime, channel, controller, value );
  }
  
  if ( type == "ProgChange" ) {
    unsigned short channel, program;
    in >> hex >> channel >> dec;
    in >> hex >> program >> dec;
    command = newProgChange ( deltaTime, channel, program );
  }
  
  if ( type == "ChanPress" ) {
    unsigned short channel, pressure;
    in >> hex >> channel >> dec;
    in >> hex >> pressure >> dec;
    command = newChanPress ( deltaTime, channel, pressure );
  }
  
  if ( type == "PitchWheel" ) {
    unsigned short channel, lsb, msb;
    in >> hex >> channel >> dec;
    in >> hex >> lsb >> dec;
    in >> hex >> msb >> dec;
    command = newPitchWheel ( deltaTime, channel, lsb, msb );
  }
  
  if ( type == "SongPos" ) {
    unsigned short lsb, msb;
    in >> hex >> lsb >> dec;
    in >> hex >> msb >> dec;
    command = newSongPos ( deltaTime, lsb, msb );
  }
  
  if ( type == "SongSel" ) {
    unsigned short song;
    in >> hex >> song >> dec;
    command = newSongSel ( deltaTime, song );
  }
  
  if ( type == "Clock" ) {
    command = newClock ( deltaTime );
  }
  
  if ( type == "Start" ) {
    command = newStart ( deltaTime );
  }
  
  if ( type == "Continue" ) {
    command = newContinue ( deltaTime );
  }

  if ( type == "Stop" ) {
    command = newStop ( deltaTime );
  }

  if ( type == "Tune" ) {
    command = newTune ( deltaTime );
  }

  if ( type == "ActiveSens" ) {
    command = newActiveSens ( deltaTime );
  }

  if ( type == "Reset" ) {
    command = newReset ( deltaTime );
  }

  if ( type == "Ctrl14b" ) {
    unsigned short channel, control, value;
    in >> hex >> channel >> dec;
    in >> hex >> control >> dec;
    in >> hex >> value >> dec;
    command = newCtrl14b ( deltaTime, channel, control, value );
  }

  if ( type == "NonRegParam" ) {
    unsigned short channel, parameter, value;
    in >> hex >> channel >> dec;
    in >> hex >> parameter >> dec;
    in >> hex >> value >> dec;
    command = newNonRegParam ( deltaTime, channel, parameter, value );
  }
  
  if ( type == "RegParam" ) {
    unsigned short channel, parameter, value;
    in >> hex >> channel >> dec;
    in >> hex >> parameter >> dec;
    in >> hex >> value >> dec;
    command = newRegParam ( deltaTime, channel, parameter, value );
  }

  if ( type == "Stream" ) {
    unsigned short byte;
    command = MidiNewEv ( typeStream );
    Date ( command ) = deltaTime;
    do {
      in >> hex >> byte >> dec;
      MidiAddField ( command, byte );
    }
    while ( in.peek ( ) != '\n' );
  }

  if ( type == "SysEx" ) {
    unsigned short byte;
    command = MidiNewEv ( typeSysEx );
    Date ( command ) = deltaTime;
    do {
      in >> hex >> byte >> dec;
      MidiAddField ( command, byte );
    }
    while ( in.peek ( ) != '\n' );
  }

  if ( in.peek ( ) != '\n' ) {
    cerr << "incorrect command line" << endl;
    exit ( 0 );
  }
  
  return command;
  
}

void printPayload ( uint8 * buffer, TPayloadStats * stats, ostream & out )
{

  if ( ! stats ) {
    static TPayloadStats mesStats;
    stats = & mesStats;
  }
  memset ( stats, 0, sizeof ( TPayloadStats ) );
  for ( unsigned short i = 0 ; i < 16 ; i++ ) {
    memset ( & stats->channel[i], 0, sizeof ( TChannelJournalStats ) );
  }

  out << endl << "### COMMAND LIST ###" << endl;

  out << "B (" << getFlag ( buffer[0], 0 ) << ")\t";
  bool journal = getFlag ( buffer[0], 1 );
  out << "J (" << journal << ")\t";
  out << "Z (" << getFlag ( buffer[0], 2 ) << ")\t";
  out << "P (" << getFlag ( buffer[0], 3 ) << ")" << endl;
  unsigned int len = ( buffer[0] & 0x0F ) * 0x0100 + buffer[1];
  stats->commandSectionSize = len + 2;
  out << "LEN\t" << len << endl;

  uint8 * end = buffer + len + 1;
  buffer += 2;

  if ( len > 0 ) {

    MidiEvPtr command;
    uint32 deltaTime;

    if ( ! getFlag ( buffer[-2], 2 ) ) {
      buffer += readCommand ( buffer, & command );
      stats->numberOfCommands++;
      Date ( command ) = 0;
      printCommand ( command, "", out );
      MidiFreeEv ( command );
    }
    
    while ( buffer < end ) {
      buffer += decodeDeltaTime ( buffer, & deltaTime );
      buffer += readCommand ( buffer, & command );
      stats->numberOfCommands++;
      Date ( command ) = deltaTime;
      printCommand ( command, "", out );
      MidiFreeEv ( command );
    }

  }

  if ( journal ) {
    printJournal ( buffer, stats, out );
  }
  stats->totalSize = stats->commandSectionSize + stats->journalSectionSize;
  out << endl;

}

unsigned short printJournal ( uint8 * buffer, TPayloadStats * stats, ostream & out )
{

  out << "### RECOVERY JOURNAL ### S (" << getFlag ( buffer[0], 0 ) << ")" << endl;

  out << "Y (" << getFlag ( buffer[0], 1 ) << ")\t";
  out << "A (" << getFlag ( buffer[0], 2 ) << ")\t";
  out << "R (" << getFlag ( buffer[0], 3 ) << ")" << endl;
  unsigned short totchan = ( buffer[0] & 0x0F ) + 1;
  stats->numberOfChannels = totchan;
  out << "TOTCHAN (" << totchan << ")\t";
  unsigned short checkpoint = buffer[1] * 256 + buffer[2];
  out << "CHECKPOINT (" << checkpoint << ")" << endl;

  uint8 * position = buffer + 3;

  if ( getFlag ( buffer[0], 1 ) ) {
    unsigned short systemJournalSize = printSystemJournal ( position, stats, out );
    stats->systemJournalSize = systemJournalSize;
    position += systemJournalSize;
  }

  if ( getFlag ( buffer[0], 2 ) ) {
    for ( unsigned int i = 0 ; i < totchan ; i++ ) {
      position += printChannelJournal ( position, stats, out );
    }
  }

  stats->journalSectionSize = position - buffer;
  return position - buffer;

}

#define D 1
#define V 2
#define Q 3
#define F 4
#define X 5

unsigned short printSystemJournal ( uint8 * buffer, TPayloadStats * stats, ostream & out )
{

  out << "### SYSTEM JOURNAL ###" << endl;

  out << "S (" << getFlag ( buffer[0], 0 ) << ")\t";
  out << "D (" << getFlag ( buffer[0], D ) << ")\t";
  out << "V (" << getFlag ( buffer[0], V ) << ")\t";
  out << "Q (" << getFlag ( buffer[0], Q ) << ")\t";
  out << "F (" << getFlag ( buffer[0], F ) << ")\t";
  out << "X (" << getFlag ( buffer[0], X ) << ")" << endl;
  unsigned short length = ( buffer[0] & 0x03 ) * 256 + buffer[1];
  stats->systemJournalSize = length;
  out << "LENGTH (" << length << ")\t";

  uint8 * position = buffer + 2;

  if ( getFlag ( buffer[0], D ) ) {
  }

  if ( getFlag ( buffer[0], V ) ) {
  }

  if ( getFlag ( buffer[0], Q ) ) {
  }

  if ( getFlag ( buffer[0], F ) ) {
  }

  if ( getFlag ( buffer[0], X ) ) {
  }

  return position - buffer;

}

#define P 0
#define C 1
#define M 2
#define W 3
#define N 4
#define E 5
#define T 6
#define A 7

unsigned short printChannelJournal ( uint8 * buffer, TPayloadStats * stats, ostream & out )
{

  unsigned short chan = ( buffer[0] & 0x78 ) >> 3;
  out << "### CHANNEL " << chan << " ### S (" << getFlag ( buffer[0], 0 ) << ")" << endl;

  out << "R (" << getFlag ( buffer[0], 5 ) << ")\t";
  unsigned short length = ( buffer[0] & 0x03 ) * 256 + buffer[1];
  stats->channel[chan].channelJournalSize = length;
  out << "LENGTH (" << length << ")" << endl;
  out << "P (" << getFlag ( buffer[2], P ) << ")\t";
  out << "C (" << getFlag ( buffer[2], C ) << ")\t";
  out << "M (" << getFlag ( buffer[2], M ) << ")\t";
  out << "W (" << getFlag ( buffer[2], W ) << ")\t";
  out << "N (" << getFlag ( buffer[2], N ) << ")\t";
  out << "E (" << getFlag ( buffer[2], E ) << ")\t";
  out << "T (" << getFlag ( buffer[2], T ) << ")\t";
  out << "A (" << getFlag ( buffer[2], A ) << ")" << endl;

  uint8 * position = buffer + 3;
  unsigned short chapterSize;

  if ( getFlag ( buffer[2], P ) ) {
    chapterSize = printChapterP ( position, stats, out );
    stats->channel[chan].chapterPSize = chapterSize;
    position += chapterSize;
  }

  if ( getFlag ( buffer[2], C ) ) {
    chapterSize = printChapterC ( position, stats, out );
    stats->channel[chan].chapterCSize = chapterSize;
    position += chapterSize;
  }

  if ( getFlag ( buffer[2], M ) ) {
  }

  if ( getFlag ( buffer[2], W ) ) {
    chapterSize = printChapterW ( position, stats, out );
    stats->channel[chan].chapterWSize = chapterSize;
    position += chapterSize;
  }

  if ( getFlag ( buffer[2], N ) ) {
    chapterSize = printChapterN ( position, stats, out );
    stats->channel[chan].chapterNSize = chapterSize;
    position += chapterSize;
  }

  if ( getFlag ( buffer[2], E ) ) {
  }

  if ( getFlag ( buffer[2], T ) ) {
    chapterSize = printChapterT ( position, stats, out );
    stats->channel[chan].chapterTSize = chapterSize;
    position += chapterSize;
  }

  if ( getFlag ( buffer[2], A ) ) {
    chapterSize = printChapterA ( position, stats, out );
    stats->channel[chan].chapterASize = chapterSize;
    position += chapterSize;
  }

  return position - buffer;

}

unsigned short printChapterP ( uint8 * buffer, TPayloadStats * stats, ostream & out )
{

  out << "### CHAPTER P ### S (" << getFlag ( buffer[0], 0 ) << ")" << endl;

  out << "PROGRAM (" << hex << ( buffer[0] & 0x7f ) << dec << ")\t";
  out << "B (" << getFlag ( buffer[1], 0 ) << ")\t";
  out << "BANK-MSB (" << hex << ( buffer[1] & 0x7f ) << dec << ")\t";
  out << "X (" << getFlag ( buffer[2], 0 ) << ")\t";
  out << "BANK-LSB (" << hex << ( buffer[2] & 0x7f ) << dec << ")" << endl;

  return 3;

}

unsigned short printChapterC ( uint8 * buffer, TPayloadStats * stats, ostream & out )
{

  out << "### CHAPTER C ### S (" << getFlag ( buffer[0], 0 ) << ")" << endl;

  unsigned short len = buffer[0] & 0x7F;
  out << "LEN (" << len << ")" << endl;

  uint8 * position = buffer + 1;
  for ( unsigned short i = 0 ; i <= len ; i++ ) {
    out << "Ctrl\t" << hex << ( position[0] & 0x7F ) << dec << "\t";
    if ( ! ( position[1] & 0x80 ) ) {
      out << "Value\t" << hex << ( position[1] & 0x7F ) << dec << "\t";
    }
    if ( ( position[1] & 0x80 ) && ( position[1] & 0x40 ) ) {
      out << "Toggle\t" << hex << ( position[1] & 0x3F ) << dec << "\t";
    }
    if ( ( position[1] & 0x80 ) && ! ( position[1] & 0x40 ) ) {
      out << "Count\t" << hex << ( position[1] & 0x3F ) << dec << "\t";
    }
    out << "S(" << ( ( position[0] & 0x80 ) >> 7 ) << ")" << endl;
    position += 2;
  }

  return position - buffer;

}

unsigned short printChapterW ( uint8 * buffer, TPayloadStats * stats, ostream & out )
{

  out << "### CHAPTER W ### S (" << getFlag ( buffer[0], 0 ) << ")" << endl;

  out << "FIRST (" << hex << ( buffer[0] & 0x7f ) << dec << ")\t";
  out << "SECOND (" << hex << ( buffer[1] & 0x7f ) << dec << ")" << endl;

  return 2;

}

unsigned short printChapterN ( uint8 * buffer, TPayloadStats * stats, ostream & out )
{

  out << "### CHAPTER N ### B (" << getFlag ( buffer[0], 0 ) << ")" << endl;

  unsigned short len = buffer[0] & 0x7F;
  out << "LEN (" << len << ")\t";
  unsigned short low = ( buffer[1] & 0xF0 ) >> 4;
  out << "LOW (" << low << ")\t";
  unsigned short high = buffer[1] & 0x0F;
  out << "HIGH (" << high << ")" << endl;

  uint8 * position = buffer + 2;
  for ( unsigned short i = 0 ; i < len ; i++ ) {
    out << "\tKeyOn\t\t";
    out << hex << ( position[0] & 0x7F ) << "\t" << dec;
    out << hex << ( position[1] & 0x7F ) << "\t" << dec;
    out << "S(" << ( ( position[0] & 0x80 ) >> 7 ) << ") Y(" << ( ( position[1] & 0x80 ) >> 7 ) << ")" << endl;
    position += 2;
  }

  if ( ! ( high == 0 && low == 15 ) && ! ( high == 1 && low == 15 ) ) {
    for ( unsigned short i = 0 ; i <= high - low ; i++ ) {
      for ( unsigned short j = 0 ; j < 8 ; j++ ) {
	if ( position[i] & ( 0x80 >> j ) ) {
	  out << "\tKeyOff\t\t" << hex << ( i + low ) * 8 + j << dec << endl;
	}
      }
    }
    position += high - low + 1;
  }

  return position - buffer;

}

unsigned short printChapterT ( uint8 * buffer, TPayloadStats * stats, ostream & out )
{

  out << "### CHAPTER T ### S (" << getFlag ( buffer[0], 0 ) << ")" << endl;

  out << "PRESSURE (" << hex << ( buffer[0] & 0x7f ) << dec << ")" << endl;

  return 1;

}

unsigned short printChapterA ( uint8 * buffer, TPayloadStats * stats, ostream & out )
{

  out << "### CHAPTER A ### S (" << getFlag ( buffer[0], 0 ) << ")" << endl;

  unsigned short len = buffer[0] & 0x7F;
  out << "LEN (" << len << ")\t" << endl;

  uint8 * position = buffer + 1;
  for ( unsigned short i = 0 ; i < len + 1 ; i++ ) {
    out << "\tKeyPress\t";
    out << hex << ( position[0] & 0x7F ) << "\t" << dec;
    out << hex << ( position[1] & 0x7F ) << "\t" << dec;
    out << "S(" << ( ( position[0] & 0x80 ) >> 7 ) << ") X(" << ( ( position[1] & 0x80 ) >> 7 ) << ")" << endl;
    position += 2;
  }

  return position - buffer;

}

char * chapterTable ( uint8 * buffer, short journal )
{

  static char table[13];
  table[0] = 0;

  if ( getFlag ( buffer[0], 1 ) ) {

    unsigned int len = ( buffer[0] & 0x0F ) * 0x0100 + buffer[1];
    buffer += len + 2;

    bool y = getFlag ( buffer[0], 1 );
    bool a = getFlag ( buffer[0], 2 );
    unsigned short totchan = ( buffer[0] & 0x0F ) + 1;
    buffer += 3;

    if ( y ) {

    if ( journal == -1 ) {
      unsigned short position = 0;
      if ( getFlag ( buffer[0], D ) ) {
	table[position] = 'D';
	table[position+1] = ' ';
	position += 2;
      }
      if ( getFlag ( buffer[0], V ) ) {
	table[position] = 'V';
	table[position+1] = ' ';
	position += 2;
      }
      if ( getFlag ( buffer[0], Q ) ) {
	table[position] = 'Q';
	table[position+1] = ' ';
	position += 2;
      }
      if ( getFlag ( buffer[0], F ) ) {
	table[position] = 'F';
	table[position+1] = ' ';
	position += 2;
      }
      if ( getFlag ( buffer[0], X ) ) {
	table[position] = 'X';
	table[position+1] = ' ';
	position += 2;
      }
      if ( position > 0 ) {
	position -= 1;
	table[position] = 0;
      }
    }
    
    buffer += ( buffer[0] & 0x03 ) * 256 + buffer[1];

    }

    if ( a ) {
      for ( unsigned int i = 0 ; i < totchan ; i++ ) {
	short chan = ( buffer[0] & 0x78 ) >> 3;
	if ( journal == chan ) {
	  unsigned short position = 0;
	  if ( getFlag ( buffer[2], P ) ) {
	    table[position] = 'P';
	    table[position+1] = ' ';
	    position += 2;
	  }
	  if ( getFlag ( buffer[2], C ) ) {
	    table[position] = 'C';
	    table[position+1] = ' ';
	    position += 2;
	  }
	  if ( getFlag ( buffer[2], M ) ) {
	    table[position] = 'M';
	    table[position+1] = ' ';
	    position += 2;
	  }
	  if ( getFlag ( buffer[2], W ) ) {
	    table[position] = 'W';
	    table[position+1] = ' ';
	    position += 2;
	  }
	  if ( getFlag ( buffer[2], N ) ) {
	    table[position] = 'N';
	    table[position+1] = ' ';
	    position += 2;
	  }
	  if ( getFlag ( buffer[2], E ) ) {
	    table[position] = 'E';
	    table[position+1] = ' ';
	    position += 2;
	  }
	  if ( getFlag ( buffer[2], T ) ) {
	    table[position] = 'T';
	    table[position+1] = ' ';
	    position += 2;
	  }
	  if ( getFlag ( buffer[2], A ) ) {
	    table[position] = 'A';
	    table[position+1] = ' ';
	    position += 2;
	  }
	  if ( position > 0 ) {
	    position -= 1;
	    table[position] = 0;
	  }
	}
	
	buffer += ( buffer[0] & 0x03 ) * 256 + buffer[1];
	
      }
    }

  }

  return table;
  
}

void getPayloadStats ( uint8 * buffer, TPayloadStats * stats )
{

  ofstream null;
  null.open ( "/dev/null" );

  printPayload ( buffer, stats, null );

}

void printStats ( TPayloadStats * stats, ostream & out )
{

  out << endl;
  out << "$ total size\t\t\t" << stats->totalSize << endl;
  out << "$ command section size\t\t" << stats->commandSectionSize << endl;
  out << "$ number of commands\t\t" << stats->numberOfCommands << endl;
  out << "$ journal section size\t\t" << stats->journalSectionSize << endl;
  if ( stats->systemJournalSize != 0 ) out << "$   system journal size\t\t" << stats->systemJournalSize << endl;
  if ( stats->chapterDSize != 0 ) out << "$   chapter d size\t\t" << stats->chapterDSize << endl;
  if ( stats->chapterVSize != 0 ) out << "$   chapter v size\t\t" << stats->chapterVSize << endl;
  if ( stats->chapterQSize != 0 ) out << "$   chapter q size\t\t" << stats->chapterQSize << endl;
  if ( stats->chapterFSize != 0 ) out << "$   chapter f size\t\t" << stats->chapterFSize << endl;
  if ( stats->chapterXSize != 0 ) out << "$   chapter x size\t\t" << stats->chapterXSize << endl;
  if ( stats->numberOfChannels != 0 ) out << "$   number of channels\t\t" << stats->numberOfChannels << endl;
  for ( unsigned short i = 0 ; i < 16 ; i++ ) {
    if ( stats->channel[i].channelJournalSize != 0 ) {
      out << "$   channel " << i << endl;
      out << "$     channel " << i << " journal size\t" << stats->channel[i].channelJournalSize << endl;
      if ( stats->channel[i].chapterPSize != 0 ) out << "$     chapter p size\t\t" << stats->channel[i].chapterPSize << endl;
      if ( stats->channel[i].chapterCSize != 0 ) out << "$     chapter c size\t\t" << stats->channel[i].chapterCSize << endl;
      if ( stats->channel[i].chapterMSize != 0 ) out << "$     chapter m size\t\t" << stats->channel[i].chapterMSize << endl;
      if ( stats->channel[i].chapterWSize != 0 ) out << "$     chapter w size\t\t" << stats->channel[i].chapterWSize << endl;
      if ( stats->channel[i].chapterNSize != 0 ) out << "$     chapter n size\t\t" << stats->channel[i].chapterNSize << endl;
      if ( stats->channel[i].chapterESize != 0 ) out << "$     chapter e size\t\t" << stats->channel[i].chapterESize << endl;
      if ( stats->channel[i].chapterTSize != 0 ) out << "$     chapter t size\t\t" << stats->channel[i].chapterTSize << endl;
      if ( stats->channel[i].chapterASize != 0 ) out << "$     chapter a size\t\t" << stats->channel[i].chapterASize << endl;
    }
  }

}
