/*

  Copyright � Grame 2001

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
  grame@rd.grame.fr

*/


#ifndef __msPTDriver__
#define __msPTDriver__

#define PTDriverName	"\pPitchTracker Driver"
#define kPTDriverVersion	100


typedef struct state {
	Boolean		showWindow;
	Boolean		autoQuit;
	Boolean     sflag;
	long		tune;
	long 		buffers;
	long 		noisein;
	long 		noiseout;
	long 		bendlength;
	long 		vol;
	long 		bend;
	long		fftsize;
	long		dynamic;
	
} State, *StatePtr;


extern Boolean doneFlag;

Boolean SetUpMidi (StatePtr state);
Boolean AudioWakeUp (int fftsize);

void 	CloseMidi ();
void 	DoIdle ();

enum { kVolOff=0, kVolOn};
enum { kBendOff=0, kBendOn};
enum { kMinBendLength=0, kMaxBendLength = 24};
enum { kMinFFTSize=512, kMaxFFTSize=8192};


#endif