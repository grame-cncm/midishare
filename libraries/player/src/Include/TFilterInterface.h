// ===========================================================================
// The Player Library is Copyright (c) Grame, Computer Music Research Laboratory 
// 1996-1999, and is distributed as Open Source software under the Artistic License;
// see the file "Artistic" that is included in the distribution for details.
//
// Grame : Computer Music Research Laboratory
// Web : http://www.grame.fr/Research
// E-mail : MidiShare@rd.grame.fr
// ===========================================================================


#ifndef __TFilterInterface__
#define __TFilterInterface__

#include "UTools.h"

//--------------------------
// Class TFilterInterface 
//--------------------------
// Base interface for events filters

class TFilterInterface {

	public :
 
 		virtual ~TFilterInterface() {}
 		virtual Boolean FilterEv(MidiEvPtr ) = 0;
 		
 };
 
 
typedef TFilterInterface FAR * TFilterInterfacePtr;


//----------------------
// Class TEventFilter 
//----------------------
//

class TEventFilter :public TFilterInterface{

	private:
	
		MidiEvPtr fEvent;

	public :
 
 		TEventFilter (MidiEvPtr event) {fEvent = event;}
 		virtual ~TEventFilter() {MidiFreeEv(fEvent);}
 		
  		Boolean FilterEv(MidiEvPtr e) 
  		{ 
  			return ((EvType(fEvent) == EvType(e))
  				&& (Chan (fEvent) == Chan (e))
  				&& (Port (fEvent) == Port (e)));
  		}
 		
 };
 
 
typedef TEventFilter FAR * TEventFilterPtr;


//-------------------------
// Class TFilterFactory
//--------------------------

 class TFilterFactory  {
 
 	public:

		static TFilterInterfacePtr MakeEventFilter(MidiEvPtr e) { return new TEventFilter(e);}
		
};



#endif