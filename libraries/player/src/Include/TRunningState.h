// ===========================================================================
// The Player Library is Copyright (c) Grame, Computer Music Research Laboratory 
// 1996-2003, and is distributed as Open Source software under the Artistic License;
// see the file "Artistic" that is included in the distribution for details.
//
// Grame : Computer Music Research Laboratory
// Web : http://www.grame.fr/Research
// E-mail : MidiShare@rd.grame.fr
// ===========================================================================


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
