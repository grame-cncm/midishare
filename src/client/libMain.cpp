/*
  Copyright © Grame 2002

  This library is free software; you can redistribute it and modify it under 
  the terms of the GNU Library General Public License as published by the 
  Free Software Foundation version 2 of the License, or any later version.

  This library is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public License 
  for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

  Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
  grame@grame.fr
  
*/


#include <stdio.h>

#include "msKernel.h"
#include "msEvents.h"
#include "msMem.h"
#include "msLibContext.h"

#define kDefaultCLientSpace	60000
#define kMidiShareLibVersion	004

#ifdef __cplusplus
extern "C" {
#endif

	short MidiGetLibVersion () ;
	void  MSInitialize () ;

#ifdef __cplusplus
}
#endif


#ifdef WIN32
void main () {}
#endif

#ifndef __MacOSX__
/*
  
  On MacOSx the constructor of the static LibMain class is not called
  unless a client call MidiGetLibVersion.
  This strange behavior is unexplained.
  
*/

class LibMain {
	public:
				 LibMain();
		virtual ~LibMain();
};

LibMain gMSInit;
	
/*____________________________________________________________________________*/
LibMain::LibMain ()
{
	MSInitialize ();
}

LibMain::~LibMain ()
{
	CloseMemory(Memory(gMem));
}
#endif

/*____________________________________________________________________________*/
static void  MSContextInitialize (TMSGlobalPtr g)
{
	msLibContextPtr c = (msLibContextPtr)AllocateMemory (sizeof(msLibContext));
	if (c) {
		c->cchan = 0;
		c->send  = StdSend;
		c->msMem = 0;
		c->RTThread = 0;
		fifoinit (&c->RTSnd);
		
        msStreamParseInitMthTbl (c->parseMthTable);
        msStreamInitMthTbl (c->streamMthTable);
		msStreamParseInit (&c->std.parse, c->parseMthTable, c->std.rbuff, kReadBuffSize);
		msStreamParseInit (&c->RT.parse, c->parseMthTable, c->RT.rbuff, kReadBuffSize);
        msStreamInit 	  (&c->std.stream, c->streamMthTable);
        msStreamInit 	  (&c->RT.stream, c->streamMthTable);
	}
	g->context = c;
}

/*____________________________________________________________________________*/
void  MSInitialize ()
{ 
//printf ("MidiShare library initialized\n");
    InitEvents ();
    InitMemory (Memory(gMem), kDefaultCLientSpace);
    OpenMemory (Memory(gMem));
    gMem->pub = 0;				/* shared memory not yet mapped */
	gMem->activesAppls[0] = 0;	/* no active application */
	MSContextInitialize (gMem);
}

/*____________________________________________________________________________*/
short MidiGetLibVersion () { return kMidiShareLibVersion; }

