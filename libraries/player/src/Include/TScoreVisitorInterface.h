/*

  Copyright © Grame 1996-2006

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
//	TScoreVisitorInterface.h			    
// ===========================================================================

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
/*!
 \brief A TScoreVisitor is used to visit a TScore object.
*/

class TScoreVisitorInterface {

	public:
	
		virtual ~TScoreVisitorInterface() {}

		virtual void Visite(TNote* ev, Boolean forward) {}  
		virtual void Visite(TKeyOn* ev, Boolean forward) {} 
		virtual void Visite(TKeyOff* ev, Boolean forward) {} 
		virtual void Visite(TKeyPress* ev, Boolean forward) {}
		virtual void Visite(TCtrlChange* ev, Boolean forward) {}
		virtual void Visite(TProgChange* ev, Boolean forward) {}
		virtual void Visite(TChanPress* ev, Boolean forward) {}
		virtual void Visite(TPitchBend* ev, Boolean forward) {}
		virtual void Visite(TTune* ev, Boolean forward) {}
		virtual void Visite(TSysEx* ev, Boolean forward) {}
		virtual void Visite(TTempo* ev, Boolean forward) {}
		virtual void Visite(TTimeSign* ev, Boolean forward) {}
};

typedef class TScoreVisitorInterface  FAR * TScoreVisitorInterfacePtr;

#endif
