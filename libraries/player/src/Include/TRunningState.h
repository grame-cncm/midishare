/*

  Copyright © Grame 1996-2004

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
  research@grame.fr

*/

// ===========================================================================
//	TRunningState.h			    
// ===========================================================================

#ifndef __TRunningState__
#define __TRunningState__

//---------------------
// Class TRunningState
//---------------------
/*!
\brief	The Player running state.
*/


class TRunningState {

	private:

		short fStatus;
		
	public:
		
		TRunningState ():fStatus (kIdle){}
	
		Boolean IsIdle() 	{return  (fStatus == kIdle);}
		Boolean IsWaiting() {return  (fStatus == kWaiting);}
		Boolean IsRunning() {return  (fStatus == kPlaying);}
		Boolean IsPause()	{return  (fStatus == kPause);}
			
		void SetRunning() 	{fStatus = kPlaying; }
		void SetIdle() 		{fStatus = kIdle;}
		void SetWaiting() 	{fStatus = kWaiting;}
		void SetPause() 	{fStatus = kPause;}
		
		short GetState() {return fStatus;}
};

typedef TRunningState FAR * TRunningStatePtr;

#endif
