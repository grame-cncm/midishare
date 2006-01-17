/*

  Copyright (C) 2005  Grame
  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

  This file is provided as an example of the rtp-midi Library use.
  
*/

#include "StreamWriter.h"
#include "StreamReader.h"
#include "printingUtils.h"
#include "eventUtils.h"
#include "binaryUtils.h"
#include <fstream>
#include <iostream>

#define NOTHING      0
#define SENDING      1
#define RECOVERING   2
#define RECEIVING    3

#define MAX_SIZE   10000

StreamWriter * streamWriter;
StreamReader * streamReader;
unsigned int size = MAX_SIZE;
uint8 payload[MAX_SIZE];
uint32 number;

void newStream ( );
void newPayload ( unsigned int payloadNumber );
void sendCommand ( MidiEvPtr command );
void sendPayload ( );
void receivePayload ( );
void isNextRecoveryCommand ( MidiEvPtr command );
void noMoreRecoveryCommand ( );
void isNextCommand ( MidiEvPtr command );
void noMoreCommand ( );

bool booleanStringToBoolean ( string booleanString );
unsigned short eventTypeStringToEventType ( string eventTypeString );
unsigned short toolStringToTool ( string toolString );
chapterInclusion inclusionStringToInclusion ( string inclusionString );
journallingMethod journallingStringToJournalling ( string journallingString );

int main ( int argc, char * argv[] )
{

  short myRefNum;
  myRefNum = MidiOpen ( "test" );

  ifstream input ( argv[1] );
  if ( ! input.good ( ) ) {
    cerr << "error : cannot open input file " << argv[1] << endl;
  }

  unsigned int payloadNumber = 0;
  unsigned short state;
  MidiEvPtr command;

  do {

    string flag;
    input >> flag;

    /* payload succession */

    if ( flag == "#" ) {
      string command;
      input >> command;

      if ( command == "newStream" ) {
	if ( state == SENDING ) {
	  sendPayload ( );
	  receivePayload ( );
	  state = RECOVERING;
	}
	if ( state == RECOVERING ) {
	  noMoreRecoveryCommand ( );
	  state = RECEIVING;
	}
	if ( state == RECEIVING ) {
	  noMoreCommand ( );
	}
	cout << endl;
	newStream ( );
	state = NOTHING;
      }

      else if ( command == "newPayload" ) {
	if ( state == SENDING ) {
	  sendPayload ( );
	  receivePayload ( );
	  state = RECOVERING;
	}
	if ( state == RECOVERING ) {
	  noMoreRecoveryCommand ( );
	  state = RECEIVING;
	}
	if ( state == RECEIVING ) {
	  noMoreCommand ( );
	}
	input >> payloadNumber;
	if ( input.fail ( ) ) {
	  cerr << "impossible to read payload number" << endl;
	  exit ( EXIT_FAILURE );
	}
	memset ( payload, 0, size * sizeof ( uint8 ) );
	newPayload ( payloadNumber );
	state = SENDING;
      }

      else {
	cerr << "unknown command " << command << endl;
	exit ( EXIT_FAILURE );
      }
    }

    /* input commands */

    else if ( flag == ">" ) {
      if ( state != SENDING ) {
	cerr << "cannot send a command here" << endl;
	exit ( EXIT_FAILURE );
      }
      else {
	command = readCommand ( input );
	sendCommand ( command );
      }
    }

    /* recovery commands */

    else if ( flag == "?" ) {
      if ( state == SENDING ) {
	sendPayload ( );
	receivePayload ( );
	state = RECOVERING;
      }
      if ( state != RECOVERING ) {
	cerr << "cannot ask for recovery commands here" << endl;
	exit ( EXIT_FAILURE );
      }
      else {
	command = readCommand ( input );
	isNextRecoveryCommand ( command );
      }
    }

    /* midi commands */

    else if ( flag == "<" ) {
      if ( state == SENDING ) {
	sendPayload ( );
	receivePayload ( );
	state = RECOVERING;
      }	
      if ( state == RECOVERING ) {
	noMoreRecoveryCommand ( );
	state = RECEIVING;
      }
      if ( state != RECEIVING ) {
	cerr << "cannot receive commands here" << endl;
	exit ( EXIT_FAILURE );
      }
      command = readCommand ( input );
      isNextCommand ( command );
    }

    /* specific actions */

    else if ( flag == "!" ) {
      string action;
      input >> action;

      if ( action == "dropPacket" ) {
	if ( state != SENDING ) {
	  cerr << "cannot drop a packet here" << endl;
	  exit ( EXIT_FAILURE );
	}
	sendPayload ( );
	cout << "! dropPacket" << endl;
	//	cout << endl;
	state = NOTHING;
      }

      else if ( action == "changeCheckpoint" ) {
	if ( state == SENDING ) {
	  sendPayload ( );
	  receivePayload ( );
	  state = RECOVERING;
	}
	if ( state == RECOVERING ) {
	  noMoreRecoveryCommand ( );
	  state = RECEIVING;
	}
	if ( state == RECEIVING ) {
	  noMoreCommand ( );
	}
	unsigned int checkpoint = 0;
	input >> checkpoint;
	streamWriter->changeCheckpoint ( checkpoint );
	streamReader->changeCheckpoint ( checkpoint );
	cout << "! changeCheckpoint\t" << checkpoint << endl;
      }

      else if ( action == "printPayload" ) {
	cout << "! printPayload" << endl;
	if ( state == NOTHING ) {
	  cerr << "cannot print a payload here" << endl;
	  exit ( EXIT_FAILURE );
	}
	sendPayload ( );
	printPayload ( payload );
      }

      else if ( action == "printBuffer" ) {
	cout << "! printBuffer" << endl;
	if ( state == NOTHING ) {
	  cerr << "cannot print a payload here" << endl;
	  exit ( EXIT_FAILURE );
	}
	sendPayload ( );
	printBuffer ( payload, size );
      }

      else if ( action == "chapterTable" ) {
	if ( state == NOTHING ) {
	  cerr << "cannot compare chapter tables here" << endl;
	  exit ( EXIT_FAILURE );
	}
	sendPayload ( );
	short journal;
	if ( input.peek ( ) == 'S' ) {
	  journal = -1;
	  string journalString;
	  input >> journalString;
	}
	else {
	  input >> hex >> journal >> dec;
	}
	char line[255];
	input.get ( );
	input.getline ( line, 255 );
	char * table = chapterTable ( payload, journal );
	cout << "! chapterTable\t" << journal << "\t" << line <<endl;
	if ( strcmp ( line, table ) ) {
	  cout << "error : chapter tables do not match !" << endl;
	  cout << "error : payload chapter table\t" << table << endl;
	  exit ( EXIT_FAILURE );
	}
      }

      else if ( action == "printChapterHistory" ) {
	short channel;
	char chapter;
	if ( input.peek ( ) == 'S' ) {
	  channel = -1;
	  string channelString;
	  input >> channelString;
	}
	else {
	  input >> hex >> channel >> dec;
	}
	input >> chapter;
	cout << "! printChapterHistory\t" << channel << "\t" << chapter << endl;
	streamWriter->printChapterHistory ( chapter, channel );
	streamReader->printChapterHistory ( chapter, channel );
      }

      else if ( action == "stopTest" ) {
	cout << "! stopTest" << endl;
	cout << endl;
	exit ( EXIT_SUCCESS );
      }

      /* customization */

      else if ( action == "setJournallingMethod" ) {
	string journallingString;
	journallingMethod journalling;
	input >> journallingString;
	journalling = journallingStringToJournalling ( journallingString );
	cout << "! setJournallingString\t" << journallingString << endl;
	streamWriter->setJournallingMethod ( journalling );
      }

      else if ( action == "setChapterInclusion" ) {
	short channel;
	char chapter;
	string inclusionString;
	chapterInclusion inclusion;
	if ( input.peek ( ) == 'S' ) {
	  channel = -1;
	  string channelString;
	  input >> channelString;
	}
	else {
	  input >> hex >> channel >> dec;
	}
	input >> chapter;
	input >> inclusionString;
	inclusion = inclusionStringToInclusion ( inclusionString );
	cout << "! setChapterInclusion\t" << hex << channel << dec << "\t" << chapter << "\t" << inclusionString << endl;
	streamWriter->setChapterInclusion ( chapter, inclusion, channel );
      }

      else if ( action == "setControllerLogTool" ) {
	short channel;
	short controller;
	string toolString;
	unsigned short tool;
	input >> hex >> channel >> dec;
	input >> hex >> controller >> dec;
	input >> toolString >> dec;
	tool = toolStringToTool ( toolString );
	cout << "! setControllerLogTool\t" << hex << channel << dec << "\t" << hex << controller << dec << "\t" << toolString << endl;
	streamWriter->setControllerLogTool ( controller, tool, channel );
      }

      else if ( action == "setPayloadSize" ) {
	input >> size;
	cout << endl << "! setPayloadSize\t" << size << endl;
      }

      else {
	cerr << "unknown action " << action << endl;
	exit ( EXIT_FAILURE );
      }
    }

    else if ( flag == "/" ) {
      char line[255];
      input.getline ( line, 255 );
      cout << endl << "/" << line << endl;
    }

    else {
      if ( input.eof ( ) ) {

	if ( state == SENDING ) {
	  sendPayload ( );
	  receivePayload ( );
	  state = RECOVERING;
	}
	if ( state == RECOVERING ) {
	  noMoreRecoveryCommand ( );
	  state = RECEIVING;
	}
	if ( state == RECEIVING ) {
	  noMoreCommand ( );
	}
	break;
      }
      else {
	cerr << "unknown flag " << flag << endl;
	exit ( EXIT_FAILURE );
      }
    }

  }
  while ( true );

  cout << endl << "!!! TEST SUCCESSFUL !!!" << endl << endl;

  MidiClose ( myRefNum );

  exit ( EXIT_SUCCESS );

}

void newStream ( )
{

  cout << "# newStream" << endl;
  streamWriter = new StreamWriter;
  streamReader = new StreamReader;
  //  cout << endl;

}

void newPayload ( unsigned int payloadNumber )
{

  cout << endl;
  cout << "# newPayload\t" << payloadNumber << endl;
  number = payloadNumber;
  streamWriter->newPayload ( number, payload, size );

}

void sendCommand ( MidiEvPtr command )
{

  static MidiEvPtr lastSysEx;

  if ( lastSysEx && EvType ( command ) == typeSysEx ) {
    if ( MidiCountFields ( command ) == MidiCountFields ( lastSysEx ) ) {
      for ( int i = 0 ; i < MidiCountFields ( command ) ; i++ ) {
	if ( MidiGetField ( command, i ) != MidiGetField ( lastSysEx, i ) ) {
	  break;
	}
	command = lastSysEx;
      }
    }
  }

  if ( EvType ( command ) == typeSysEx ) {
    lastSysEx = command;
  }
  else {
    lastSysEx = NULL;
  }

  cout << "> ";
  short result;
  if ( ( result = streamWriter->putCommand ( command ) ) > 0 ) {
    printCommand ( command );
  }
  else if ( result == 0 ) {
    printCommand ( command, "\tFULL PAYLOAD !" );
  }
  else if ( result == -1 ) {
    printCommand ( command, "\tUNUSED CHAPTER !" );
  }
  else if ( result == -2 ) {
    printCommand ( command, "\tUNSUPPORTED COMMAND !" );
  }

}

void sendPayload ( )
{

  streamWriter->finalizePayload ( );
  //  cout << endl;

}

void receivePayload ( )
{

  streamReader->newPayload ( number, payload, size );

}

void isNextRecoveryCommand ( MidiEvPtr command )
{

  MidiEvPtr payloadCommand;
  if ( ! ( payloadCommand = streamReader->getRecoveryCommand ( ) ) ) {
    cout << "error : cannot get recovery command from payload !" << endl;
    exit ( EXIT_FAILURE );
  }
  Date ( command ) = 0;
  if ( ! equal ( command, payloadCommand ) ) {
    cout << "error : recovery command does no match !" << endl;
    cout << "error : file command\t";
    printCommand ( command );
    cout << "error : payload command\t";
    printCommand ( payloadCommand );
    exit ( EXIT_FAILURE );
  }
  else {
    cout << "? ";
    printCommand ( command, "\t[OK]" );
  }

}

void noMoreRecoveryCommand ( )
{

  MidiEvPtr recoveryCommand;
  if ( ( recoveryCommand = streamReader->getRecoveryCommand ( ) ) ) {
    cout << "error : extra recovery command !" << endl;
    cout << "error : recovery command\t";
    printCommand ( recoveryCommand );
    while ( ( recoveryCommand = streamReader->getRecoveryCommand ( ) ) ) {
      cout << "error : recovery command\t";
      printCommand ( recoveryCommand );
    }
    exit ( EXIT_FAILURE );
  }
  else {
    //    cout << "! noMoreRecoveryCommand" << endl;
  }

}

void isNextCommand ( MidiEvPtr command )
{

  MidiEvPtr payloadCommand;
  if ( ! ( payloadCommand = streamReader->getCommand ( ) ) ) {
    cout << "error : cannot get command from payload !" << endl;
    exit ( EXIT_FAILURE );
  }
  if ( ! equal ( command, payloadCommand ) ) {
    cout << "error : midi command does no match !" << endl;
    cout << "error : file command\t";
    printCommand ( command );
    cout << "error : payload command\t";
    printCommand ( payloadCommand );
    exit ( EXIT_FAILURE );
  }
  else {
    cout << "< ";
    printCommand ( command );
  }

}

void noMoreCommand ( )
{

  MidiEvPtr payloadCommand;
  if ( ( payloadCommand = streamReader->getCommand ( ) ) ) {
    cout << "error : extra payload command !" << endl;
    cout << "error : payload command\t";
    printCommand ( payloadCommand );
    while ( ( payloadCommand = streamReader->getCommand ( ) ) ) {
      cout << "error : payload command\t";
      printCommand ( payloadCommand );
    }
    exit ( EXIT_FAILURE );
  }
  else {
    //    cout << "! noMoreCommand" << endl;
    //    cout << endl;
  }

}

bool booleanStringToBoolean ( string booleanString )
{

  if ( booleanString == "on" ||
       booleanString == "On" ||
       booleanString == "ON" ||
       booleanString == "true" || 
       booleanString == "True" ||
       booleanString == "TRUE" ) {
    return true;
  }

  return false;

}

unsigned short eventTypeStringToEventType ( string eventTypeString )
{

  if ( eventTypeString == "KeyOn" ) {
    return typeKeyOn;
  }

  if ( eventTypeString == "KeyOff" ) {
    return typeKeyOff;
  }

  if ( eventTypeString == "KeyPress" ) {
    return typeKeyPress;
  }

  if ( eventTypeString == "CtrlChange" ) {
    return typeCtrlChange;
  }

  if ( eventTypeString == "ProgChange" ) {
    return typeProgChange;
  }

  if ( eventTypeString == "ChanPress" ) {
    return typeChanPress;
  }

  if ( eventTypeString == "PitchWheel" ) {
    return typePitchWheel;
  }

  if ( eventTypeString == "SongPos" ) {
    return typeSongPos;
  }

  if ( eventTypeString == "SongSel" ) {
    return typeSongSel;
  }

  if ( eventTypeString == "Clock" ) {
    return typeClock;
  }

  if ( eventTypeString == "Start" ) {
    return typeStart;
  }

  if ( eventTypeString == "Continue" ) {
    return typeContinue;
  }

  if ( eventTypeString == "Stop" ) {
    return typeStop;
  }

  if ( eventTypeString == "Tune" ) {
    return typeTune;
  }

  if ( eventTypeString == "ActiveSens" ) {
    return typeActiveSens;
  }

  if ( eventTypeString == "Reset" ) {
    return typeReset;
  }

  if ( eventTypeString == "SysEx" ) {
    return typeSysEx;
  }

  if ( eventTypeString == "Stream" ) {
    return typeStream;
  }

  if ( eventTypeString == "Ctrl14b" ) {
    return typeCtrl14b;
  }

  if ( eventTypeString == "NonRegParam" ) {
    return typeNonRegParam;
  }

  if ( eventTypeString == "RegParam" ) {
    return typeRegParam;
  }

  return typeDead;

}

unsigned short toolStringToTool ( string toolString )
{

  unsigned short tool = 0;
  for ( unsigned short i = 0 ; i < toolString.size ( ) ; i++ ) {
    if ( toolString[i] == 'V' ) {
      tool |= VALUE_TOOL;
    }
    if ( toolString[i] == 'T' ) {
      tool |= TOGGLE_TOOL;
    }
    if ( toolString[i] == 'C' ) {
      tool |= COUNT_TOOL;
    }
  }
  return tool;

}

chapterInclusion inclusionStringToInclusion ( string inclusionString )
{

  if ( inclusionString == "unused" ) {
    return UNUSED;
  }

  if ( inclusionString == "never" ) {
    return NEVER;
  }

  if ( inclusionString == "default" ) {
    return DEFAULT;
  }

  if ( inclusionString == "anchor" ) {
    return ANCHOR;
  }

  if ( inclusionString == "active" ) {
    return ACTIVE;
  }

  if ( inclusionString == "anchor_active" ) {
    return ANCHOR_ACTIVE;
  }

  return DEFAULT;

}

journallingMethod journallingStringToJournalling ( string journallingString )
{

  if ( journallingString == "none" ) {
    return NONE;
  }

  if ( journallingString == "recj" ) {
    return RECJ;
  }

  return RECJ;

}
