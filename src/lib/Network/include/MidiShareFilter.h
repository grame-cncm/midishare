/*
  MidiShare Project
  Copyright (C) Grame 2001
  Copyright (C) Mil Productions 2001

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

 
 #ifndef __MidiShareFilter__
 #define __MidiShareFilter__

#include "MidiShare.h"
#include "FilterUtils.h"

typedef unsigned char uchar;

//_______________________________________________________________________________
class MidiShareFilter
{
	public:
				 MidiShareFilter ();
		virtual ~MidiShareFilter ();
		
		MidiFilterPtr	GetMidiFilter (void);
		bool		IsAcceptedEv  (MidiEvPtr e);
		
		void	AcceptAll	();
		void	RejectAll	();

		void	AcceptPort	(uchar port);
		void	RejectPort	(uchar port);
		void	InvertPort	(uchar port);
		bool	IsAcceptedPort	(uchar port);
		
		void	AcceptChan	(uchar chan);
		void	RejectChan	(uchar chan);
		void	InvertChan	(uchar chan);
		bool	IsAcceptedChan	(uchar chan);
		
		void	AcceptType	(uchar type);
		void	RejectType	(uchar type);
		void	InvertType	(uchar type);
		bool	IsAcceptedType	(uchar type);
		
	protected:
#if macintosh
		TFilter	fFilter;
#else
		MidiFilterPtr	fFilter;
#endif
};

//_______________________________________________________________________________
#if macintosh
inline MidiFilterPtr MidiShareFilter::GetMidiFilter()	{ return &fFilter; }

inline void	MidiShareFilter::AcceptPort	(uchar port)	{ AcceptBit(fFilter.port, port); }
inline void	MidiShareFilter::RejectPort	(uchar port)	{ RejectBit(fFilter.port, port); }
inline void	MidiShareFilter::InvertPort	(uchar port)	{ InvertBit(fFilter.port, port); }
inline bool	MidiShareFilter::IsAcceptedPort	(uchar port)
													{ return IsAcceptedBit(fFilter.port, port); }
inline void	MidiShareFilter::AcceptChan	(uchar chan)	{ AcceptBit(fFilter.channel, chan); }
inline void	MidiShareFilter::RejectChan	(uchar chan)	{ RejectBit(fFilter.channel, chan); }
inline void	MidiShareFilter::InvertChan	(uchar chan)	{ InvertBit(fFilter.channel, chan); }
inline bool	MidiShareFilter::IsAcceptedChan	(uchar chan)
													{ return IsAcceptedBit(fFilter.channel, chan); }
inline void	MidiShareFilter::AcceptType	(uchar type)	{ AcceptBit(fFilter.evType, type); }
inline void	MidiShareFilter::RejectType	(uchar type)	{ RejectBit(fFilter.evType, type); }
inline void	MidiShareFilter::InvertType	(uchar type)	{ InvertBit(fFilter.evType, type); }
inline bool	MidiShareFilter::IsAcceptedType	(uchar type)
													{ return IsAcceptedBit(fFilter.evType, type); }

#else

inline MidiFilterPtr MidiShareFilter::GetMidiFilter()	{ return fFilter; }

inline void	MidiShareFilter::AcceptPort	(uchar port)	{ MidiAcceptPort(fFilter, port, true); }
inline void	MidiShareFilter::RejectPort	(uchar port)	{ MidiAcceptPort(fFilter, port, false); }
inline void	MidiShareFilter::InvertPort	(uchar port)	
								{ MidiAcceptPort(fFilter, port, !IsAcceptedPort(port)); }
inline bool	MidiShareFilter::IsAcceptedPort	(uchar port)
								{ return MidiIsAcceptedPort(fFilter, port); }

inline void	MidiShareFilter::AcceptChan	(uchar chan)	{ MidiAcceptChan(fFilter, chan, true); }
inline void	MidiShareFilter::RejectChan	(uchar chan)	{ MidiAcceptChan(fFilter, chan, false); }
inline void	MidiShareFilter::InvertChan	(uchar chan)	
								{ MidiAcceptChan(fFilter, chan, !IsAcceptedChan(chan)); }
inline bool	MidiShareFilter::IsAcceptedChan	(uchar chan)
								{ return MidiIsAcceptedChan(fFilter, chan); }

inline void	MidiShareFilter::AcceptType	(uchar type)	{ MidiAcceptType(fFilter, type, true); }
inline void	MidiShareFilter::RejectType	(uchar type)	{ MidiAcceptType(fFilter, type, false); }
inline void	MidiShareFilter::InvertType	(uchar type)	
								{ MidiAcceptType(fFilter, type, !IsAcceptedType(type)); }
inline bool	MidiShareFilter::IsAcceptedType	(uchar type)
								{ return MidiIsAcceptedType(fFilter, type); }
#endif

#endif
