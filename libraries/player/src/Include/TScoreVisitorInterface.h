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
//	TScoreVisitorInterface.h			    
// ===========================================================================

/*!
 \brief A TScoreVisitor is used to visit a TScore object.
*/

#ifndef __TScoreVisitor__
#define __TScoreVisitor__

class TEvent;
class TNote;
class TKeyOn;
class TKeyOff;
class TKeyPress;
class TCtrlChange;
class TProgChange;
class TPitchBend;
class TChanPress;
class TTune;
class TSysEx;
class TTempo;
class TTimeSign;

//---------------------
// Class TScoreVisitor 
//---------------------

class TScoreVisitorInterface {

	public:
	
		virtual ~TScoreVisitorInterface() {}

		virtual void Visite (TNote* ev, Boolean  forward ){}  
		virtual void Visite (TKeyOn* ev, Boolean  forward ){} 
		virtual void Visite (TKeyOff* ev, Boolean  forward ){} 
		virtual void Visite (TKeyPress* ev, Boolean  forward ){}
		virtual void Visite (TCtrlChange* ev, Boolean  forward ){}
		virtual void Visite (TProgChange* ev, Boolean  forward ){}
		virtual void Visite (TChanPress* ev, Boolean  forward ){}
		virtual void Visite (TPitchBend* ev, Boolean  forward ){}
		virtual void Visite (TTune* ev, Boolean  forward ){}
		virtual void Visite (TSysEx* ev, Boolean  forward ){}
		virtual void Visite (TTempo* ev, Boolean  forward ){}
		virtual void Visite (TTimeSign* ev, Boolean  forward ){}
		
};

typedef class TScoreVisitorInterface  FAR * TScoreVisitorInterfacePtr;


#endif
