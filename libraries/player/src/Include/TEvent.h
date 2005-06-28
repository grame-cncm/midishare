/*

  Copyright � Grame 1996-2004

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
//	TEvent.h			    
// ===========================================================================

#ifndef __TEvent__
#define __TEvent__
 
#include <stddef.h>

#include "PlayerDefs.h"
#include "MidiShare.h"
#include "TScoreVisitorInterface.h"
#include "TPlayerConstants.h"

//--------------
// Class TEvent 
//--------------
/*!
	\brief The classes for all Midi events. All events use a TVisitor object to add additional methods
*/

class TEvent {

	public:

	 	TEvent* fNext;  // Pointers for double-linked list 
	 	TEvent* fPrev;	// Pointers for double-linked list 

	 	MidiEvPtr fEvent; // Internal MidiShare event
	 	
	 	// Use of the MidiShare memory allocator

		void*	operator new (size_t)				{return MidiNewCell();}  
		void 	operator delete(void* p, size_t) 	{MidiFreeCell((MidiEvPtr)p);}
			
		TEvent() 				{fNext = fPrev = 0;fEvent = 0;}
		TEvent(MidiEvPtr e) 	{fNext = fPrev = 0;fEvent = e;}

		virtual ~TEvent()  		{if (fEvent) MidiFreeEv(fEvent);} 
		
		Byte 		GetType() 	{return EvType(fEvent);}
		ULONG 		GetDate() 	{return Date(fEvent);}
		void 		SetDate(ULONG date) {Date(fEvent) = date;}
		
		short  		GetTrackNum() {return RefNum(fEvent);}
		void  		SetTrackNum(short tracknum) {RefNum(fEvent) = (unsigned char)tracknum;}
		MidiEvPtr  	MidiEvent() 	{return fEvent;}
		
		virtual void Accept(TScoreVisitorInterfacePtr v, Boolean forward) {}  // Score Visitor
};

typedef TEvent FAR * TEventPtr;
	
//-------------
// Class TNote 
//-------------
/*!
	\brief The class for Note events.
*/

class TNote : public TEvent {

	public:
 
 		TNote(MidiEvPtr e){fEvent = e;fNext = fPrev = 0;}
 		virtual ~TNote(){}
		void Accept(TScoreVisitorInterfacePtr v, Boolean forward) {v->Visite(this, forward);}  
};

typedef TNote FAR * TNotePtr;

//--------------
// Class TKeyOn 
//--------------
/*!
	\brief The class for KeyOn events.
*/

class TKeyOn : public TEvent {

	public:

		TKeyOn(MidiEvPtr e){fEvent = e;fNext = fPrev = 0;}
		virtual ~TKeyOn(){}
		void Accept(TScoreVisitorInterfacePtr v, Boolean forward) {v->Visite(this, forward);}  
};

typedef TKeyOn FAR * TKeyOnPtr;

//---------------
// Class TKeyOff 
//---------------
/*!
	\brief The class for KeyOff events.
*/

class TKeyOff : public TEvent {

	public:

		TKeyOff	(MidiEvPtr e){fEvent = e;fNext = fPrev = 0;}
		virtual ~TKeyOff(){}
		void Accept(TScoreVisitorInterfacePtr v, Boolean forward) {v->Visite(this, forward);}  
};

typedef TKeyOff FAR * TKeyOffPtr;

//-----------------
// Class TKeyPress 
//-----------------
/*!
	\brief The class for KeyPress events.
*/

class TKeyPress : public TEvent {

 	public:
	
		TKeyPress	(MidiEvPtr e){fEvent = e;fNext = fPrev = 0;}
		virtual ~TKeyPress(){}
		void Accept(TScoreVisitorInterfacePtr v, Boolean forward) {v->Visite(this, forward);}  
};

typedef TKeyPress FAR * TKeyPressPtr;

//-------------------
// Class TCtrlChange 
//-------------------
/*!
	\brief The class for CtrlChange events.
*/

class TCtrlChange : public TEvent {

 	public:
	
		TCtrlChange	(MidiEvPtr e){fEvent = e;fNext = fPrev = 0;}
		virtual ~TCtrlChange(){}
		void Accept(TScoreVisitorInterfacePtr v, Boolean forward) {v->Visite(this, forward);}   
 
};

typedef TCtrlChange FAR * TCtrlChangePtr;

//-------------------
// Class TProgChange 
//-------------------
/*!
	\brief The class for ProgChange events.
*/

class TProgChange : public TEvent {

 	public:
	
		TProgChange	(MidiEvPtr e){fEvent = e;fNext = fPrev = 0;}
		virtual ~TProgChange(){}
		void Accept(TScoreVisitorInterfacePtr v, Boolean forward) {v->Visite(this, forward);}  
};

typedef TProgChange FAR * TProgChangePtr;

//------------------
// Class TChanPress 
//------------------
/*!
	\brief The class for ChanPress events.
*/

class TChanPress : public TEvent {

 	public:
	
		TChanPress	(MidiEvPtr e){fEvent = e;fNext = fPrev = 0;}
		virtual ~TChanPress(){}
		void Accept(TScoreVisitorInterfacePtr v, Boolean forward) {v->Visite(this, forward);}  
};

typedef TChanPress FAR * TChanPressPtr;

//-------------------
// Class TPitchWheel 
//-------------------
/*!
	\brief The class for PitchWheel events.
*/

class TPitchBend : public TEvent {

 	public:
	
		TPitchBend	(MidiEvPtr e){fEvent = e;fNext = fPrev = 0;}
		virtual ~TPitchBend(){}
		void Accept(TScoreVisitorInterfacePtr v, Boolean forward) {v->Visite(this, forward);}   
};

typedef TPitchBend FAR * TPitchBendPtr;

//-------------
// Class TTune 
//-------------
/*!
	\brief The class for Tune events.
*/

class TTune : public TEvent {

	public:

		TTune (MidiEvPtr e){fEvent = e;fNext = fPrev = 0;}
		virtual ~TTune(){}
		void Accept(TScoreVisitorInterfacePtr v, Boolean forward) {v->Visite(this, forward);}  
};

typedef TTune FAR * TTunePtr;

//---------------
// Class TSysEx
//---------------
/*!
	\brief The class for SysEx events.
*/

class TSysEx : public TEvent {

	public:

		TSysEx (MidiEvPtr e){fEvent = e;fNext = fPrev = 0;}
		virtual ~TSysEx(){}
		void Accept(TScoreVisitorInterfacePtr v, Boolean forward) {v->Visite(this, forward);}  
};

typedef TSysEx FAR * TSysExPtr;

//--------------
// Class TTempo
//--------------
/*!
	\brief The class for Tempo events.
*/

class TTempo : public TEvent {

 	public:
	
		TTempo(MidiEvPtr e)
		{
			fEvent = e;
			fNext = fPrev = 0; 
			SetTempoBackward(kDefaultTempo);
		}
		virtual ~TTempo(){}
		void Accept(TScoreVisitorInterfacePtr v, Boolean forward) {v->Visite(this, forward);}   
		
		ULONG GetTempoForward()  {return Tempo(fEvent);}
		ULONG GetTempoBackward() {return ((ULONG *)&Link(fEvent))[0];}
		void SetTempoForward(ULONG val)  {Tempo(fEvent) = val;}
		void SetTempoBackward(ULONG val) {((ULONG *)&Link(fEvent))[0] = val;}
};

typedef TTempo FAR * TTempoPtr;

//------------------
// Class TTimeSign
//------------------
/*!
	\brief The class for TimeSign events.
*/

class TTimeSign : public TEvent {

 	public:
	
		TTimeSign(MidiEvPtr e)
		{
			fEvent = e;
			fNext = fPrev = 0;
			SetBNum(kDefaultNum);
			SetBDenom(kDefaultDenom);
			SetBnClocks(kDefaultClocks);
			SetBn32nd(kDefaultN32);
		}
		virtual ~TTimeSign(){}
		void Accept(TScoreVisitorInterfacePtr v, Boolean forward) {v->Visite(this, forward);}  
		
		Byte GetFNum() 		{return TSNum(fEvent);}
		Byte GetFDenom() 	{return TSDenom(fEvent);}
		Byte GetFnClocks() 	{return TSClocks(fEvent);}  
		Byte GetFn32nd() 	{return TS32nd(fEvent);}

		Byte GetBNum() 		{return ((char*)&Link(fEvent))[0];}
		Byte GetBDenom() 	{return ((char*)&Link(fEvent))[1];}
		Byte GetBnClocks() 	{return ((char*)&Link(fEvent))[2];}
		Byte GetBn32nd() 	{return ((char*)&Link(fEvent))[3];}
		
		void SetFNum(Byte val) 		{TSNum(fEvent)= val;}
		void SetFDenom(Byte val) 	{TSDenom(fEvent)= val;}
		void SetFnClocks(Byte val) 	{TSClocks(fEvent)= val;}
		void SetFn32nd(Byte val) 	{TS32nd(fEvent)= val;}
		
		void SetBNum(Byte val) 		{((char*)&Link(fEvent))[0] = val;}
		void SetBDenom(Byte val) 	{((char*)&Link(fEvent))[1] = val;}
		void SetBnClocks(Byte val) 	{((char*)&Link(fEvent))[2] = val;}
		void SetBn32nd(Byte val) 	{((char*)&Link(fEvent))[3] = val;}
};

typedef TTimeSign FAR * TTimeSignPtr;

#endif
