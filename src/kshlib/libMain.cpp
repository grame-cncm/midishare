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

#include "msKernelPrefs.h"
#include "msKernel.h"
#include "msEvents.h"

#include "msLog.h"

#define kDefaultCLientSpace	60000

static TMSGlobalPublic gPubMem;
static msKernelPrefs prefs = { 
	kDefaultCLientSpace,
#ifdef WIN32
	kTimeModeMMSystem,
#else
	kTimeModeAudio,
#endif
	1, 						// timeRes
	{ 0 },
	"",
	"Built-in audio controller"
};

#ifdef __cplusplus
extern "C" {
#endif

	msKernelPrefs * gPrefs = &prefs;
	extern TMSGlobalPtr gMem;
	short MidiGetLibVersion () ;
	void  MSInitialize () ;

	void   MidiShareSpecialInit(unsigned long defaultSpace, void *publicMemory);

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
static msKernelPrefs * init ()
{
	msCmdLinePrefs args;
	msKernelPrefs * prefs;
	
	ReadArgs (&args, 0, 0);
	prefs = ReadPrefs (args.conffile ? args.conffile : 0, &args);
	if (prefs->logfile[0]) OpenLog (prefs->logfile);
	CheckPrefs (prefs);
	return prefs;
}

/*____________________________________________________________________________*/
void  MSInitialize ()
{ 
//	gPrefs = init();
	MidiShareSpecialInit (kDefaultCLientSpace, &gPubMem);
}

