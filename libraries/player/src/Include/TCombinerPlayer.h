// ===========================================================================
// The Player Library is Copyright (c) Grame, Computer Music Research Laboratory 
// 1996-1999, and is distributed as Open Source software under the Artistic License;
// see the file "Artistic" that is included in the distribution for details.
//
// Grame : Computer Music Research Laboratory
// Web : http://www.grame.fr/Research
// E-mail : MidiShare@rd.grame.fr
// ===========================================================================


// ===========================================================================
//	TCombinerPlayer.h		    
// ===========================================================================
//
//  A TCombinerPlayer combines two TTickPlayerInterface players.


#ifndef __TCombinerPlayer__
#define __TCombinerPlayer__


#include "TTickPlayer.h"

//-----------------------
// Class TCombinerPlayer
//-----------------------


class TCombinerPlayer : public TTickPlayerInterface {
	
	 private:
	 
	 	TTickPlayerInterfacePtr fPlayer1, fPlayer2;
	 	
	  public:
	  	
	 	TCombinerPlayer (TTickPlayerInterfacePtr player1, TTickPlayerInterfacePtr player2)
	 	{
	 		fPlayer1 = player1;
	 		fPlayer2 = player2;
	 	}
	 	~TCombinerPlayer () {}
	 	
	 	void SetPlayer1(TTickPlayerInterfacePtr player1) {fPlayer1 = player1;}
	 	void SetPlayer2(TTickPlayerInterfacePtr player2) {fPlayer2 = player2;}
	 	
	 	 void Init(){ 
			fPlayer1->Init();  
			fPlayer2->Init();
		}
  
		void Start(){ 
			fPlayer1->Start();  
			fPlayer2->Start();
		}
		void Stop(){ 
			fPlayer1->Stop();  
			fPlayer2->Stop();
		}
		void Cont(ULONG date_ticks){ 
			fPlayer1->Cont(date_ticks);  
			fPlayer2->Cont(date_ticks);
		}
		
		void PlaySliceForward(){ fPlayer1->PlaySliceForward();  fPlayer2->PlaySliceForward();}
 		void PlaySliceBackward(){ fPlayer1->PlaySliceBackward();  fPlayer2->PlaySliceBackward();}

		void SetPosTicks(ULONG date_ticks) { 
			fPlayer1->SetPosTicks(date_ticks);  
			fPlayer2->SetPosTicks(date_ticks);
		}
		ULONG GetPosTicks(){ return fPlayer1->GetPosTicks(); }
};
 
typedef TCombinerPlayer FAR * TCombinerPlayerPtr;


#endif
