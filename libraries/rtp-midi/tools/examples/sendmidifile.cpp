/*

  Copyright (C) 2005  Grame
  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

  This file is provided as an example of the rtp-midi Library use.
  
*/

#include <Player.h>

#include <getopt.h>

#include "StreamWriter.h"
#include "printingUtils.h"

#include <iostream>

#define MAX_SIZE   10000

StreamWriter * streamWriter;

uint8 buffer[MAX_SIZE];
unsigned int bufferSize = 1400;
uint32 payloadNumber = 0;
TPayloadStats payloadStats;

double packetDuration = 200;
double packetTime = 0;
double lastCommandTime = 0;
unsigned int lastCommandTick = 0;
unsigned int lastTempo = 500000;
double checkpointRefreshInterval = -1;
double lastCheckpointRefresh = 0;

MidiSeqPtr sequence;
MidiFileInfos info;
MidiEvPtr command;

const char * options = "v:s:d:c:";
struct option long_options[] =
  {
    {"verbose", 1, 0, 'v'},
    {"payload_size", 1, 0, 's'},
    {"packet_duration", 1, 0, 'd'},
    {"checkpoint_refresh_delay", 1, 0, 'c'},
    {0, 0, 0, 0}
  };
int option_index;
int opt;
unsigned short verbose = 0;

double milliseconds ( unsigned int ticks );
unsigned int ticks ( double milliseconds );

MidiEvPtr getNextCommand ( );
void createNextPayload ( );

int main ( int argc, char * argv[] )
{

  while ( ( opt = getopt_long ( argc, argv, options, long_options, & option_index ) ) != EOF ) {
    switch ( opt ) {
    case 'v':
      if ( atoi ( optarg ) < 0 ) {
	fprintf ( stderr, "invalid verbose level size\n" );
	exit ( EXIT_FAILURE );
      }
      else {
	verbose = atoi ( optarg );
      }
      break;
    case 's':
      if ( atoi ( optarg ) <= 0 ) {
	fprintf ( stderr, "invalid buffer size\n" );
	exit ( EXIT_FAILURE );
      }
      else {
	bufferSize = atoi ( optarg );
      }
      break;
    case 'd':
      if ( atoi ( optarg ) <= 0 ) {
	fprintf ( stderr, "invalid packet duration\n" );
	exit ( EXIT_FAILURE );
      }
      else {
	packetDuration = atof ( optarg );
      }
      break;
    case 'c':
      if ( atoi ( optarg ) <= 0 ) {
	fprintf ( stderr, "invalid checkpoint refresh delay\n" );
	exit ( EXIT_FAILURE );
      }
      else {
	checkpointRefreshInterval = atoi ( optarg );
      }
      break;
    }
  }

  short myRefNum;
  if ( ( myRefNum = MidiOpen ( "sendmidifile" ) ) == 0 ) {
    cerr << "unable to open a midishare session" << endl;
  }

  sequence = MidiNewSeq ( );
  if ( MidiFileLoad ( argv[argc-1], sequence, & info ) > 0 ) {
    cerr << "error while loading midi file" << argv[argc-1] << endl;
    exit ( EXIT_FAILURE );
  }

  streamWriter = new StreamWriter;

  streamWriter->newPayload ( payloadNumber, buffer, bufferSize );

  command = First ( sequence );
  if ( verbose > 1 ) printCommand ( command );

  short result;
  while ( command ) {

    result = streamWriter->putCommand ( command );

    if ( result > 0 ) {
      if ( verbose > 1 ) printCommand ( command );
      command = getNextCommand ( );
    }
    else if ( result == 0 ) {
      createNextPayload ( );
    }
    else if ( result < 0 ) {
      if ( verbose > 1 ) printCommand ( command );
      command = getNextCommand ( );
    }

  }

  streamWriter->finalizePayload ( );

  if ( verbose > 2 ) printPayload ( buffer, & payloadStats );
  else getPayloadStats ( buffer, & payloadStats );
  if ( verbose > 2 ) printStats ( & payloadStats );
  if ( verbose > 2 ) cout << "$" << endl;
  if ( verbose > 0 ) cout << "$ number of payloads\t\t" << payloadNumber << endl;
  float duration = ( ( double ) packetTime + ( double ) packetDuration ) / 1000.0;
  if ( verbose > 0 ) cout << "$ total duration\t\t" << duration << "\t" << ( ( short ) duration ) / 60 << "'" << ( ( short ) duration ) % 60 << endl;

  MidiFreeSeq ( sequence );
  MidiClose ( myRefNum );
  exit ( EXIT_SUCCESS );

}

double milliseconds ( unsigned int ticks )
{

  return ( double ) ticks * ( double ) lastTempo / ( double ) info.clicks / 1000.0;

}

unsigned int ticks ( double milliseconds )
{

  return ( unsigned int ) ( milliseconds * ( double ) info.clicks / ( double ) lastTempo * 1000.0 );

}

MidiEvPtr getNextCommand ( )
{

  command = Link ( command );

  if ( command ) {

    if ( EvType ( command ) == typeTempo ) {
      lastTempo = MidiGetField ( command, 0 );
      if ( verbose > 0 ) cout << "tempo changed to " << 60000000.0 / ( float ) lastTempo << endl;
    }

    lastCommandTime += milliseconds ( Date ( command ) - lastCommandTick );
    bool firstPacketCommand = false;
    while ( lastCommandTime > packetTime + packetDuration ) {
      createNextPayload ( );
      firstPacketCommand = true;
    }
    unsigned int commandDate = Date ( command );
    if ( firstPacketCommand ) {
      //      cout << "first" << endl;
      //      cout << "lastCommandTick : " << lastCommandTick << endl;
      Date ( command ) = ticks ( lastCommandTime - packetTime );
    }
    else {
      //      cout << "not first" << endl;
      //      cout << "commandDate : " << commandDate << endl;
      //      cout << "lastCommandTick : " << lastCommandTick << endl;
      Date ( command ) = commandDate - lastCommandTick;
    }
    lastCommandTick = commandDate;

  }

  return command;

}

void createNextPayload ( )
{

  streamWriter->finalizePayload ( );

  getPayloadStats ( buffer, & payloadStats );
  if ( payloadStats.numberOfCommands == 0 ) {

    packetTime += packetDuration;

    if ( checkpointRefreshInterval != -1 && packetTime - lastCheckpointRefresh >= checkpointRefreshInterval ) {
      lastCheckpointRefresh = packetTime;
      if ( verbose > 1 ) cout << "change checkpoint : " << payloadNumber << endl << endl;
      streamWriter->changeCheckpoint ( payloadNumber );
    }

    memset ( buffer, 0, MAX_SIZE );
    streamWriter->newPayload ( payloadNumber, buffer, bufferSize );
    //    if ( verbose > 1 ) cout << "# empty payload\t" << payloadNumber << "\t" << ( double ) ( packetTime / 1000.0 ) << endl;

    return;

  }
  else {

    if ( verbose > 1 ) cout << "# payload\t" << payloadNumber << "\t" << ( double ) ( packetTime / 1000.0 ) << endl;
    if ( verbose > 2 ) printPayload ( buffer, & payloadStats );
    if ( verbose > 2 ) printStats ( & payloadStats );
    if ( verbose > 1 ) cout << endl;

    packetTime += packetDuration;

    if ( checkpointRefreshInterval != -1 && packetTime - lastCheckpointRefresh >= checkpointRefreshInterval ) {
      lastCheckpointRefresh = packetTime;
      if ( verbose > 1 ) cout << "change checkpoint : " << payloadNumber << endl << endl;
      streamWriter->changeCheckpoint ( payloadNumber );
    }

    memset ( buffer, 0, MAX_SIZE );
    streamWriter->newPayload ( ++payloadNumber, buffer, bufferSize );

  }

}
