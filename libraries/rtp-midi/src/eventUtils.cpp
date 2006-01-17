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

#include "eventUtils.h"
#include "printingUtils.h"

#include <MidiShare.h>

unsigned short commandCategory ( unsigned short eventType )
{

  // channel commands

  switch ( eventType ) {

  case typeKeyOff :
  case typeKeyOn :
  case typeKeyPress :
  case typeCtrlChange :
  case typeProgChange :
  case typeChanPress :
  case typePitchWheel :
  case typeCtrl14b :
  case typeRegParam :
  case typeNonRegParam :
    return CHANNEL_COMMAND;

  case typeSongPos :
  case typeSongSel :
  case typeClock :
  case typeStart :
  case typeContinue :
  case typeStop :
  case typeTune :
  case typeActiveSens :
  case typeReset :
  case typeSysEx :
    return SYSTEM_COMMAND;

  default :
    return UNSUPPORTED_COMMAND;

  }

}

MidiEvPtr newKeyOff ( unsigned int deltaTime, short channel, short pitch, short velocity )
{

  MidiEvPtr e;

  if ( ( e = MidiNewEv ( typeKeyOff ) ) ) {
    Date ( e ) = deltaTime;
    Chan ( e ) = channel;
    Pitch ( e ) = pitch;
    Vel ( e ) = velocity;
  }
  return e;

}

MidiEvPtr newKeyOn ( unsigned int deltaTime, short channel, short pitch, short velocity )
{

  MidiEvPtr e;

  if ( ( e = MidiNewEv ( typeKeyOn ) ) ) {
    Date ( e ) = deltaTime;
    Chan ( e ) = channel;
    Pitch ( e ) = pitch;
    Vel ( e ) = velocity;
  }
  return e;

}

MidiEvPtr newKeyPress ( unsigned int deltaTime, short channel, short pitch, short velocity )
{

  MidiEvPtr e;

  if ( ( e = MidiNewEv ( typeKeyPress ) ) ) {
    Date ( e ) = deltaTime;
    Chan ( e ) = channel;
    Pitch ( e ) = pitch;
    Vel ( e ) = velocity;
  }
  return e;

}

MidiEvPtr newCtrlChange ( unsigned int deltaTime, short channel, short control, short value )
{

  MidiEvPtr e;

  if ( ( e = MidiNewEv ( typeCtrlChange ) ) ) {
    Date ( e ) = deltaTime;
    Chan ( e ) = channel;
    Data ( e ) [0] = control;
    Data ( e ) [1] = value;
  }
  return e;

}

MidiEvPtr newProgChange ( unsigned int deltaTime, short channel, short program )
{

  MidiEvPtr e;

  if ( ( e = MidiNewEv ( typeProgChange ) ) ) {
    Date ( e ) = deltaTime;
    Chan ( e ) = channel;
    Data ( e ) [0] = program;
  }
  return e;

}

MidiEvPtr newChanPress ( unsigned int deltaTime, short channel, short pressure )
{

  MidiEvPtr e;

  if ( ( e = MidiNewEv ( typeChanPress ) ) ) {
    Date ( e ) = deltaTime;
    Chan ( e ) = channel;
    Data ( e ) [0] = pressure;
  }
  return e;

}

MidiEvPtr newPitchWheel ( unsigned int deltaTime, short channel, short lsb, short msb )
{

  MidiEvPtr e;

  if ( ( e = MidiNewEv ( typePitchWheel ) ) ) {
    Date ( e ) = deltaTime;
    Chan ( e ) = channel;
    Data ( e ) [0] = lsb;
    Data ( e ) [1] = msb;
  }
  return e;

}

MidiEvPtr newCtrl14b ( unsigned int deltaTime, unsigned short channel, unsigned short controller, unsigned short value )
{

  MidiEvPtr e;

  if ( ( e = MidiNewEv ( typeCtrl14b ) ) ) {
    Date ( e ) = deltaTime;
    Chan ( e ) = channel;
    MidiSetField ( e, 0, controller );
    MidiSetField ( e, 1, value );
  }
  return e;

}

MidiEvPtr newNonRegParam ( unsigned int deltaTime, unsigned short channel, unsigned short parameter, unsigned short value )
{

  MidiEvPtr e;

  if ( ( e = MidiNewEv ( typeNonRegParam ) ) ) {
    Date ( e ) = deltaTime;
    Chan ( e ) = channel;
    MidiSetField ( e, 0, parameter );
    MidiSetField ( e, 1, value );
  }
  return e;

}

MidiEvPtr newRegParam ( unsigned int deltaTime, unsigned short channel, unsigned short parameter, unsigned short value )
{

  MidiEvPtr e;

  if ( ( e = MidiNewEv ( typeRegParam ) ) ) {
    Date ( e ) = deltaTime;
    Chan ( e ) = channel;
    MidiSetField ( e, 0, parameter );
    MidiSetField ( e, 1, value );
  }
  return e;

}

MidiEvPtr newSongPos ( unsigned int deltaTime, short lsb, short msb )
{

  MidiEvPtr e;

  if ( ( e = MidiNewEv ( typeSongPos ) ) ) {
    Date ( e ) = deltaTime;
    Data ( e ) [0] = lsb;
    Data ( e ) [1] = msb;
  }
  return e;

}

MidiEvPtr newSongSel ( unsigned int deltaTime, short song )
{

  MidiEvPtr e;

  if ( ( e = MidiNewEv ( typeSongSel ) ) ) {
    Date ( e ) = deltaTime;
    Data ( e ) [0] = song;
  }
  return e;

}

MidiEvPtr newClock ( unsigned int deltaTime )
{

  MidiEvPtr e;

  if ( ( e = MidiNewEv ( typeClock ) ) ) {
    Date ( e ) = deltaTime;
  }
  return e;

}

MidiEvPtr newStart ( unsigned int deltaTime )
{

  MidiEvPtr e;

  if ( ( e = MidiNewEv ( typeStart ) ) ) {
    Date ( e ) = deltaTime;
  }
  return e;

}

MidiEvPtr newContinue ( unsigned int deltaTime )
{

  MidiEvPtr e;

  if ( ( e = MidiNewEv ( typeContinue ) ) ) {
    Date ( e ) = deltaTime;
  }
  return e;

}

MidiEvPtr newStop ( unsigned int deltaTime )
{

  MidiEvPtr e;

  if ( ( e = MidiNewEv ( typeStop ) ) ) {
    Date ( e ) = deltaTime;
  }
  return e;

}

MidiEvPtr newTune ( unsigned int deltaTime )
{

  MidiEvPtr e;

  if ( ( e = MidiNewEv ( typeTune ) ) ) {
    Date ( e ) = deltaTime;
  }
  return e;

}

MidiEvPtr newActiveSens ( unsigned int deltaTime )
{

  MidiEvPtr e;

  if ( ( e = MidiNewEv ( typeActiveSens ) ) ) {
    Date ( e ) = deltaTime;
  }
  return e;

}

MidiEvPtr newReset ( unsigned int deltaTime )
{

  MidiEvPtr e;

  if ( ( e = MidiNewEv ( typeReset ) ) ) {
    Date ( e ) = deltaTime;
  }
  return e;

}

unsigned short lengthOf ( const MidiEvPtr command )
{

  switch EvType ( command ) {

  case typeKeyOff :
    return 3;

  case typeKeyOn :
    return 3;

  case typeKeyPress :
    return 3;

  case typeCtrlChange :
    return 3;

  case typeProgChange :
    return 2;

  case typeChanPress :
    return 2;

  case typePitchWheel :
    return 3;

  case typeSongPos :
    return 3;

  case typeSongSel :
    return 2;
  
  case typeClock :
    return 1;

  case typeStart :
    return 1;

  case typeContinue :
    return 1;

  case typeStop :
    return 1;

  case typeTune :
    return 1;

  case typeActiveSens :
    return 1;

  case typeReset :
    return 1;

  case typeStream :
    return MidiCountFields ( command );

  case typeSysEx :
    return MidiCountFields ( command ) + 2;

  }

  return 0;

}

unsigned short writeCommand ( const MidiEvPtr command, uint8 * buffer )
{

  switch EvType ( command ) {

  case typeKeyOff :
    buffer[0] = 0x80 | Chan ( command );
    buffer[1] = Pitch ( command );
    buffer[2] = Vel ( command );
    return 3;

  case typeKeyOn :
    buffer[0] = 0x90 | Chan ( command );
    buffer[1] = Pitch ( command );
    buffer[2] = Vel ( command );
    return 3;

  case typeKeyPress :
    buffer[0] = 0xA0 | Chan ( command );
    buffer[1] = Pitch ( command );
    buffer[2] = Vel ( command );
    return 3;

  case typeCtrlChange :
    buffer[0] = 0xB0 | Chan ( command );
    buffer[1] = Data ( command ) [0];
    buffer[2] = Data ( command ) [1];
    return 3;

  case typeProgChange :
    buffer[0] = 0xC0 | Chan ( command );
    buffer[1] = Data ( command ) [0];
    return 2;

  case typeChanPress :
    buffer[0] = 0xD0 | Chan ( command );
    buffer[1] = Data ( command ) [0];
    return 2;

  case typePitchWheel :
    buffer[0] = 0xE0 | Chan ( command );
    buffer[1] = Data ( command ) [0];
    buffer[2] = Data ( command ) [1];
    return 3;

  case typeSongPos :
    buffer[0] = 0xF2;
    buffer[1] = Data ( command ) [0];
    buffer[2] = Data ( command ) [1];
    return 3;

  case typeSongSel :
    buffer[0] = 0xF3;
    buffer[1] = Data ( command ) [0];
    return 2;
  
  case typeClock :
    buffer[0] = 0xF8;
    return 1;

  case typeStart :
    buffer[0] = 0xF9;
    return 1;

  case typeContinue :
    buffer[0] = 0xFB;
    return 1;

  case typeStop :
    buffer[0] = 0xFC;
    return 1;

  case typeTune :
    buffer[0] = 0xF6;
    return 1;

  case typeActiveSens :
    buffer[0] = 0xFE;
    return 1;

  case typeReset :
    buffer[0] = 0xFF;
    return 1;

  case typeStream :
    for ( long i = 0 ; i < MidiCountFields ( command ) ; i++ ) {
      buffer[i] = MidiGetField ( command, i );
    }
    return MidiCountFields ( command );

  case typeSysEx :
    buffer[0] = 0xF0;
    long i;
    for ( i = 0 ; i < MidiCountFields ( command ) ; i++ ) {
      buffer[i+1] = MidiGetField ( command, i );
    }
    buffer[i+1] = 0xF7;
    return MidiCountFields ( command ) + 2;

  }

  return 0;

}

unsigned short readCommand ( uint8 * buffer, MidiEvPtr * commandAddress )
{

  MidiEvPtr command;

  switch ( buffer[0] & 0xF0 ) {

  case 0x80 :
    if ( ( command = MidiNewEv ( typeKeyOff ) ) ) {
      Chan ( command ) = buffer[0] & 0x0F;
      Pitch ( command ) = buffer[1];
      Vel ( command ) = buffer[2];
      ( * commandAddress ) = command;
    }
    return 3;

  case 0x90 :
    if ( ( command = MidiNewEv ( typeKeyOn ) ) ) {
      Chan ( command ) = buffer[0] & 0x0F;
      Pitch ( command ) = buffer[1];
      Vel ( command ) = buffer[2];
      ( * commandAddress ) = command;
    }
    return 3;

  case 0xA0 :
    if ( ( command = MidiNewEv ( typeKeyPress ) ) ) {
      Chan ( command ) = buffer[0] & 0x0F;
      Pitch ( command ) = buffer[1];
      Vel ( command ) = buffer[2];
      ( * commandAddress ) = command;
    }
    return 3;

  case 0xB0 :
    if ( ( command = MidiNewEv ( typeCtrlChange ) ) ) {
      Chan ( command ) = buffer[0] & 0x0F;
      Data ( command ) [0] = buffer[1];
      Data ( command ) [1] = buffer[2];
      ( * commandAddress ) = command;
    }
    return 3;

  case 0xC0 :
    if ( ( command = MidiNewEv ( typeProgChange ) ) ) {
      Chan ( command ) = buffer[0] & 0x0F;
      Data ( command ) [0] = buffer[1];
      ( * commandAddress ) = command;
    }
    return 2;

  case 0xD0 :
    if ( ( command = MidiNewEv ( typeChanPress ) ) ) {
      Chan ( command ) = buffer[0] & 0x0F;
      Data ( command ) [0] = buffer[1];
      ( * commandAddress ) = command;
    }
    return 2;

  case 0xE0 :
    if ( ( command = MidiNewEv ( typePitchWheel ) ) ) {
      Chan ( command ) = buffer[0] & 0x0F;
      Data ( command ) [0] = buffer[1];
      Data ( command ) [1] = buffer[2];
      ( * commandAddress ) = command;
    }
    return 3;

  case 0xF0 :

    switch ( buffer[0] ) {

    case 0xF2 :
      if ( ( command = MidiNewEv ( typeSongPos ) ) ) {
	Data ( command ) [0] = buffer[1];
	Data ( command ) [1] = buffer[2];
	( * commandAddress ) = command;
      }
      return 3;
      
    case 0xF3 :
      if ( ( command = MidiNewEv ( typeSongSel ) ) ) {
	Data ( command ) [0] = buffer[1];
	( * commandAddress ) = command;
      }
      return 2;
      
    case 0xF8 :
      if ( ( command = MidiNewEv ( typeClock ) ) ) {
	( * commandAddress ) = command;
      }
      return 1;
      
    case 0xF9 :
      if ( ( command = MidiNewEv ( typeStart ) ) ) {
	( * commandAddress ) = command;
      }
      return 1;

    case 0xFB :
      if ( ( command = MidiNewEv ( typeContinue ) ) ) {
      }
      return 1;
      
    case 0xFC :
      if ( ( command = MidiNewEv ( typeStop ) ) ) {
	( * commandAddress ) = command;
      }
      return 1;
      
    case 0xF6 :
      if ( ( command = MidiNewEv ( typeTune ) ) ) {
	( * commandAddress ) = command;
      }
      return 1;

    case 0xFE :
      if ( ( command = MidiNewEv ( typeActiveSens ) ) ) {
	( * commandAddress ) = command;
      }
      return 1;

    case 0xFF :
      if ( ( command = MidiNewEv ( typeReset ) ) ) {
	( * commandAddress ) = command;
      }
      return 1;

    case 0xF0 :
    case 0xF7 :
      unsigned long i;
      if ( ( command = MidiNewEv ( typeStream ) ) ) {
	if ( buffer[0] == 0xF0 ) {
	  MidiAddField ( command, buffer[0] );
	}
	i = 1;
	while ( buffer[i] != 0xF7 && buffer[i] != 0xF0 && buffer[i] != 0xF4 ) {
	  MidiAddField ( command, buffer[i] );
	  i++;
	}
	if ( buffer[i] == 0xF7 || buffer[i] == 0xF4 ) {
	  MidiAddField ( command, buffer[i] );
	}
	if ( buffer[0] == 0xF0 && buffer[i] == 0xF7 ) {
	  // returns a real typeSysEx event
	  MidiEvPtr sysEx;
	  sysEx = MidiNewEv ( typeSysEx );
	  for ( i = 1 ; ( long ) i < MidiCountFields ( command ) - 1 ; i++ ) {
	    MidiAddField ( sysEx, MidiGetField ( command, i ) );
	  }
	  MidiFreeEv ( command );
	  ( * commandAddress ) = sysEx;
	  return i;
	}
	( * commandAddress ) = command;
      }
      return i + 1;
      
    }

  }

  cerr << "error while reading command from payload ( ";
  printBuffer ( buffer, 3, cerr );
  cerr << " ) " << endl;

  return 0;

}

bool equal ( MidiEvPtr command_1, MidiEvPtr command_2 )
{

  if ( EvType ( command_1 ) != EvType ( command_2 ) ) {
    return false;
  }

  switch EvType ( command_1 ) {

  case typeKeyOff :
    return ( Date ( command_1 ) == Date ( command_2 ) &&
	     Chan ( command_1 ) == Chan ( command_2 ) &&
	     Pitch ( command_1 ) == Pitch ( command_2 ) &&
	     Vel ( command_1 ) == Vel ( command_2 ) );

  case typeKeyOn :
    return ( Date ( command_1 ) == Date ( command_2 ) &&
	     Chan ( command_1 ) == Chan ( command_2 ) &&
	     Pitch ( command_1 ) == Pitch ( command_2 ) &&
	     Vel ( command_1 ) == Vel ( command_2 ) );

  case typeKeyPress :
    return ( Date ( command_1 ) == Date ( command_2 ) &&
	     Chan ( command_1 ) == Chan ( command_2 ) &&
	     Pitch ( command_1 ) == Pitch ( command_2 ) &&
	     Vel ( command_1 ) == Vel ( command_2 ) );

  case typeCtrlChange :
    return ( Date ( command_1 ) == Date ( command_2 ) &&
	     Chan ( command_1 ) == Chan ( command_2 ) &&
	     Data ( command_1 ) [0] == Data ( command_2 ) [0] &&
	     Data ( command_1 ) [1] == Data ( command_2 ) [1] );

  case typeProgChange :
    return ( Date ( command_1 ) == Date ( command_2 ) &&
	     Chan ( command_1 ) == Chan ( command_2 ) &&
	     Data ( command_1 ) [0] == Data ( command_2 ) [0] );

  case typeChanPress :
    return ( Date ( command_1 ) == Date ( command_2 ) &&
	     Chan ( command_1 ) == Chan ( command_2) &&
	     Data ( command_1 ) [0] == Data ( command_2 ) [0] );

  case typePitchWheel :
    return ( Date ( command_1 ) == Date ( command_2 ) &&
	     Chan ( command_1 ) == Chan ( command_2 ) &&
	     Data ( command_1 ) [0] == Data ( command_2 ) [0] &&
	     Data ( command_1 ) [1] == Data ( command_2 ) [1] );

  case typeSongPos :
    return ( Date ( command_1 ) == Date ( command_2 ) &&
	     Data ( command_1 ) [0] == Data ( command_2 ) [0] &&
	     Data ( command_1 ) [1] == Data ( command_2 ) [1] );

  case typeSongSel :
    return ( Date ( command_1 ) == Date ( command_1 ) &&
	     Data ( command_1 ) [0] == Data ( command_2 ) [0] );

  case typeClock :
    return ( Date ( command_1 ) == Date ( command_2 ) );

  case typeStart :
    return ( Date ( command_1 ) == Date ( command_2 ) );

  case typeContinue :
    return ( Date ( command_1 ) == Date ( command_2 ) );

  case typeStop :
    return ( Date ( command_1 ) == Date ( command_2 ) );

  case typeTune :
    return ( Date ( command_1 ) == Date ( command_2 ) );

  case typeActiveSens :
    return ( Date ( command_1 ) == Date ( command_2 ) );

  case typeReset :
    return ( Date ( command_1 ) == Date ( command_2 ) );

  case typeStream :
  case typeSysEx :
    if ( Date ( command_1 ) != Date ( command_2 ) ||
	 MidiCountFields ( command_1 ) != MidiCountFields ( command_2 ) ) {
      return false;
    }
    for ( long i = 0 ; i < MidiCountFields ( command_1 ) ; i++ ) {
      if ( MidiGetField ( command_1, i ) != MidiGetField ( command_2, i ) ) {
      return false;
      }
    }
    return true;

  case typeCtrl14b :
    return ( Date ( command_1 ) == Date ( command_2 ) &&
	     Chan ( command_1 ) == Chan ( command_2 ) &&
	     MidiGetField ( command_1, 0 ) == MidiGetField ( command_2, 0 ) &&
	     MidiGetField ( command_1, 1 ) == MidiGetField ( command_2, 1 ) );

  case typeRegParam :
    return ( Date ( command_1 ) == Date ( command_2 ) &&
	     Chan ( command_1 ) == Chan ( command_2 ) &&
	     MidiGetField ( command_1, 0 ) == MidiGetField ( command_2, 0 ) &&
	     MidiGetField ( command_1, 1 ) == MidiGetField ( command_2, 1 ) );

  case typeNonRegParam :
    return ( Date ( command_1 ) == Date ( command_2 ) &&
	     Chan ( command_1 ) == Chan ( command_2 ) &&
	     MidiGetField ( command_1, 0 ) == MidiGetField ( command_2, 0 ) &&
	     MidiGetField ( command_1, 1 ) == MidiGetField ( command_2, 1 ) );

  }

return false;

}
