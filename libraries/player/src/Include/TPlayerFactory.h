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
//	TPlayerFactory.h			    
// ===========================================================================
/*!
	\brief Builds a Player depending of the current synchronization mode.
*/


#ifndef __TPlayerFactory__
#define __TPlayerFactory__

#include "TDestructor.h" 
#include "TGenericPlayerInterface.h" 

class TPlayer;

//-----------------------
// Class TPlayerFactory 
//-----------------------

class TPlayerFactory {

	private:
	
		TPlayer* 		fUser;
		TDestructorPtr  fDestructor;

	public:
		
		TPlayerFactory (TPlayer* user);
			
		// Creation and Destruction of the Player 
		TGenericPlayerInterfacePtr CreatePlayer ();
		void DestroyPlayer(TGenericPlayerInterfacePtr* player);
};

typedef TPlayerFactory FAR * TPlayerFactoryPtr;

#endif
