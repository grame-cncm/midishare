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
//	TChaserIterator.h			    
// ===========================================================================


#ifndef __TChaserIterator__
#define __TChaserIterator__

 
#include "TChaserVisitor.h"
#include "TScoreIterator.h"


/*--------------------------------------------------------------------------*/
// Class TChaserInterface
/*--------------------------------------------------------------------------*/

/*!
	\brief Interface for chase objects.
*/ 

class TChaserInterface {

	public:
	
		virtual ~TChaserInterface(){}
	
		virtual void Init() = 0;
		
		virtual void  ChaseOn (ULONG date_ticks) = 0;
		virtual void  ChaseOff (ULONG date_ticks) = 0;
			
		virtual ULONG GetPosTicks () = 0;
};

typedef TChaserInterface FAR * TChaserInterfacePtr;


/*--------------------------------------------------------------------------*/
// Class TChaserIterator
/*--------------------------------------------------------------------------*/

/*!
	\brief The chaser iterator use a chaser visitor to keep the state of KeyOn, CtrlChange, 
    ProgChange, KeyPress, ChanPress, PitchBend, Sysex and Tune events when moving 
    along the score.
    The chaser iterator is used when starting or stopping the Player and will send 
    events that occurred previously in the tracks.
*/ 

class TChaserIterator :public TChaserInterface{

	private:
	
		TChaserVisitorPtr 	fChaser;   		
		TScoreIteratorPtr 	fIterator;
		ULONG 				fCurdate_ticks;
		
		void SetPosTicks (ULONG date_ticks);
		void GoBeginDateTicks (ULONG date_ticks);
			
	public:
	
		TChaserIterator(TScorePtr score, TEventSenderInterfacePtr user);
		~TChaserIterator();
	
		void Init();
		
		void  ChaseOn (ULONG date_ticks);
		void  ChaseOff (ULONG date_ticks);
			
		ULONG GetPosTicks () {return fCurdate_ticks;}
};

typedef TChaserIterator FAR * TChaserIteratorPtr;


#endif
